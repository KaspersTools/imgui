#pragma once
#include <iostream>
#include <cmath>
#include <unordered_map>
#include "../HBUI.h"
#include "../HBTime.h"
#include "../types/HBFloat.h"

struct GlobalAnimState {
  static inline hb_u_float playbackSpeed = 1.0f;
};

enum HB_AnimEffect_ {
  HB_AnimEffect_None,
  HB_AnimEffect_Expand,
  HB_AnimEffect_Slide,
};

enum HB_AnimState_ {
  HB_AnimState_Idle,      // No animation is currently playing
  HB_AnimState_Playing,   // An animation is currently playing
  HB_AnimState_Paused,    // The animation is paused
  HB_AnimState_Stopped,   // The animation is stopped
  HB_AnimState_Finished   // The animation has finished playing
};

enum HB_AnimType_ {
  HB_AnimType_Linear,// Default
  HB_AnimType_EaseIn,
  HB_AnimType_EaseOut,
  HB_AnimType_EaseInOut
};

enum HB_AnimDirection_ {
  HB_AnimDirection_Forward,
  HB_AnimDirection_Backward
};

template<typename T>
struct HBAnimProps {
  T start;
  T end;
  T current = start;

  float duration      = 10.f;
  hb_u_float playbackSpeed = 1.f;

  bool looping = false;

  HB_AnimType_      type      = HB_AnimType_Linear;
  HB_AnimDirection_ direction = HB_AnimDirection_Forward;
  HB_AnimState_     state     = HB_AnimState_Idle;
  HB_AnimEffect_    effect    = HB_AnimEffect_Slide;
};

class HBAnimBase {
public:
  HBAnimBase(const std::string &id) : m_strId(id) {
    m_id = ImGui::GetID(id.c_str());
  }

  const std::string &getId() const {
    return m_strId;
  }

  void setId(const std::string &id) {
    m_strId = id;
  }

public:
  virtual ~HBAnimBase() = default;

  virtual void update() = 0;

  virtual void pause() = 0;

  virtual void play() = 0;

  virtual void stop() = 0;

  virtual void reset() = 0;

  virtual void reverse() = 0;

private:
  std::string m_strId = "";
  ImGuiID     m_id    = 0;
};


template<typename T>
class HBAnim : public HBAnimBase {
public:
  HBAnim(const std::string &id, HBAnimProps<T> props)
      : HBAnimBase(id),
        m_startingProps(props),
        m_currentProps(props) {
  }

  void update() {

    if (m_currentProps.state == HB_AnimState_Playing) {
      m_currentTime += HBTime::deltaTime * (GlobalAnimState::playbackSpeed * m_currentProps.playbackSpeed);
      float t = m_currentTime / m_currentProps.duration;

      if (m_currentProps.direction == HB_AnimDirection_Backward) {
        t = 1.0f - t;
      }

      switch (m_currentProps.type) {
        case HB_AnimType_Linear:
          m_currentProps.current = Lerp(m_currentProps.start, m_currentProps.end, t);
          break;
        default:
          m_currentProps.current = Lerp(m_currentProps.start, m_currentProps.end, t);
          break;
      }

      if (m_currentTime >= m_currentProps.duration) {
        if (m_currentProps.looping) {
          m_currentProps.state = HB_AnimState_Playing;
          m_currentTime = 0;
        } else {
          m_currentProps.state = HB_AnimState_Finished;
        }
      }
    }
  }

  void pause() {

  }

  void play() {
    m_currentProps.state = HB_AnimState_Playing;
  }

  void stop() {

  }

  void reset() {

  }

  void reverse() {

  }

  float getDuration() const {
    return m_currentProps.duration;
  }

  float getPlaybackSpeed() const {
    return m_currentProps.playbackSpeed;
  }

  T getCurrentValue() {
    return m_currentProps.current;
  }

  T getStartValue() {
    return m_currentProps.start;
  }

  T getEndValue() {
    return m_currentProps.end;
  }

  bool isLooping() const {
    return m_currentProps.looping;
  }

  void setDuration(float duration) {
    m_currentProps.duration = duration;
  }

  void setPlaybackSpeed(float speed) {
    m_currentProps.playbackSpeed = speed;
  }

  std::string getDirection() {
    return m_currentProps.direction == HB_AnimDirection_Forward ? "Forward" : "Backward";
  }

  void setDirection(HB_AnimDirection_ direction) {
    if(direction == m_currentProps.direction) return;

    m_currentProps.direction = direction;
    // Reverse the animation
    m_currentTime = m_currentProps.duration - m_currentTime;
  }

  void setStartValue(T value) {
    m_currentProps.start = value;
  }

  void setEndValue(T value) {
    m_currentProps.end = value;
  }

  void setLooping(bool looping) {
    m_currentProps.looping = looping;
  }


  HB_AnimState_ getState() {
    return m_currentProps.state;
  }


  static inline T Clamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

  static inline ImVec2 Lerp(const ImVec2 &a, const ImVec2 &b, float t) {
    return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
  }

  static inline ImVec2 Lerp(const ImVec2 &a, const ImVec2 &b, const ImVec2 &t) {
    return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y);
  }


private:
  const HBAnimProps<T> m_startingProps;

private:
  HBAnimProps<T> m_currentProps;
  float          m_currentTime = 0;

};

class HBAnimManager {

public:
  HBAnimManager() = default;

  bool hasAnimation(const ImGuiID &id) const {
    return animations.find(id) != animations.end();
  }

  template<typename T>
  void addAnimation(const std::string &id, HBAnimProps<T> props) {
    auto    anim = std::make_shared<HBAnim<T>>(id, props);
    ImGuiID imID = ImGui::GetID(id.c_str());
    animations[imID] = anim;
  }

  template<typename T>
  T getAnimation(const std::string &id) {
    const ImGuiID imID = ImGui::GetID(id.c_str());
    return getAnimation<T>(imID);
  }

  template<typename T>
  T getAnimation(const ImGuiID &id) {
    auto it = animations.find(id);
    if (it != animations.end()) {
      auto anim = std::dynamic_pointer_cast<HBAnim<T>>(it->second);
      if (anim) {
        return anim;
      } else {
        IM_ASSERT(false && "Animation type mismatch.");
      }
    } else {
      IM_ASSERT(false && "Animation not found.");
    }
  }

  template<typename T>
  T getCurrentValue(const std::string &id) {
    const ImGuiID imID = ImGui::GetID(id.c_str());
    return getCurrentValue<T>(imID);
  }

  template<typename T>
  T getCurrentValue(const ImGuiID &id) {
    auto it = animations.find(id);
    if (it != animations.end()) {
      auto anim = std::dynamic_pointer_cast<HBAnim<T>>(it->second);
      if (anim) {
        return anim->getCurrentValue();
      } else {
        IM_ASSERT(false && "Animation type mismatch.");
      }
    } else {
      IM_ASSERT(false && "Animation not found.");
    }
  }

  template<typename T>
  T getValue(const ImGuiID &id) const {
    auto it = animations.find(id);
    if (it != animations.end()) {
      auto anim = std::dynamic_pointer_cast<HBAnim<T>>(it->second);
      if (anim) {
        return anim->getCurrentValue();
      } else {
        IM_ASSERT(false && "Animation type mismatch.");
      }
    } else {
      IM_ASSERT(false && "Animation not found.");
    }
  }

  void update() {
    for (auto &anim: animations) {
      anim.second->update();
    }
  }

  std::unordered_map<ImGuiID, std::shared_ptr<HBAnimBase>> getAnimations() const {
    return animations;
  }

private:
  std::unordered_map<ImGuiID, std::shared_ptr<HBAnimBase>> animations = {};
};

