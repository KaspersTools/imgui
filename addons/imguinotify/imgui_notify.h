// imgui-notify by patrickcjk
// https://github.com/patrickcjk/imgui-notify

#pragma once
#pragma once
#include "fa_solid_900.h"
#include "font_awesome_5.h"
#include <chrono>
#include <imgui.h>
#include <string>
#include <vector>

#define NOTIFY_MAX_MSG_LENGTH 4096   // Max message content length
#define NOTIFY_PADDING_X 20.f        // Bottom-left X padding
#define NOTIFY_PADDING_Y 20.f        // Bottom-left Y padding
#define NOTIFY_PADDING_MESSAGE_Y 10.f// Padding Y between each message
#define NOTIFY_FADE_IN_OUT_TIME 150  // Fade in and out duration
#define NOTIFY_DEFAULT_DISMISS 3000  // Auto dismiss after X ms (default, applied only of no data provided in constructors)
#define NOTIFY_TOAST_FLAGS                                                  \
  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration |       \
          ImGuiWindowFlags_NoInputs |                                       \
          ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | \
          ImGuiWindowFlags_NoFocusOnAppearing

#define NOTIFY_OPACITY 0.6f// 0-1 Toast opacity
// Comment out if you don't want any separator between title and content
#define NOTIFY_USE_SEPARATOR

#define NOTIFY_INLINE inline
#define NOTIFY_NULL_OR_EMPTY(str) (!str || !strlen(str))
#define NOTIFY_FORMAT(fn, format, ...) \
  if (format) {                        \
    va_list args;                      \
    va_start(args, format);            \
    fn(format, args, ##__VA_ARGS__);   \
    va_end(args);                      \
  }

typedef int ImGuiToastType;
typedef int ImGuiToastPhase;
typedef int ImGuiToastPos;

enum ImGuiToastType_ {
  ImGuiToastType_None,
  ImGuiToastType_Success,
  ImGuiToastType_Warning,
  ImGuiToastType_Error,
  ImGuiToastType_Info,
  ImGuiToastType_COUNT
};

enum ImGuiToastPhase_ {
  ImGuiToastPhase_FadeIn,
  ImGuiToastPhase_Wait,
  ImGuiToastPhase_FadeOut,
  ImGuiToastPhase_Expired,
  ImGuiToastPhase_COUNT
};

enum ImGuiToastPos_ {
  ImGuiToastPos_TopLeft,
  ImGuiToastPos_TopCenter,
  ImGuiToastPos_TopRight,
  ImGuiToastPos_BottomLeft,
  ImGuiToastPos_BottomCenter,
  ImGuiToastPos_BottomRight,
  ImGuiToastPos_Center,
  ImGuiToastPos_COUNT
};

class ImGuiToast {
  private:
  ImGuiToastType type = ImGuiToastType_None;
  char title[NOTIFY_MAX_MSG_LENGTH];
  char content[NOTIFY_MAX_MSG_LENGTH];
  int dismiss_time = NOTIFY_DEFAULT_DISMISS;
  uint64_t creation_time = 0;

  private:
  // Setters

  NOTIFY_INLINE auto set_title(const char *format, va_list args) { vsnprintf(this->title, sizeof(this->title), format, args); }

  NOTIFY_INLINE auto set_content(const char *format, va_list args) { vsnprintf(this->content, sizeof(this->content), format, args); }

  public:
  NOTIFY_INLINE auto set_title(const char *format, ...) -> void { NOTIFY_FORMAT(this->set_title, format); }

  NOTIFY_INLINE auto set_content(const char *format, ...) -> void { NOTIFY_FORMAT(this->set_content, format); }

  NOTIFY_INLINE auto set_type(const ImGuiToastType &type) -> void {
    IM_ASSERT(type < ImGuiToastType_COUNT);
    this->type = type;
  };

  public:
  // Getters

  NOTIFY_INLINE auto get_title() -> char * { return this->title; };

  NOTIFY_INLINE auto get_default_title() -> const char * {
    if (!strlen(this->title)) {
      switch (this->type) {
        case ImGuiToastType_None:
          return NULL;
        case ImGuiToastType_Success:
          return "Success";
        case ImGuiToastType_Warning:
          return "Warning";
        case ImGuiToastType_Error:
          return "Error";
        case ImGuiToastType_Info:
          return "Info";
        default:
          return NULL;
      }
    }

    return this->title;
  };

  NOTIFY_INLINE auto get_type() -> const ImGuiToastType & { return this->type; };

  NOTIFY_INLINE auto get_color() -> const ImVec4 {
    switch (this->type) {
      case ImGuiToastType_None:
        return {255, 255, 255, 255};// White
      case ImGuiToastType_Success:
        return {0, 255, 0, 255};// Green
      case ImGuiToastType_Warning:
        return {255, 255, 0, 255};// Yellow
      case ImGuiToastType_Error:
        return {255, 0, 0, 255};// Error
      case ImGuiToastType_Info:
        return {0, 157, 255, 255};// Blue
      default:
        return {255, 255, 255, 255};// White
    }
  }

  NOTIFY_INLINE auto get_icon() -> const char * {
    switch (this->type) {
      case ImGuiToastType_None:
        return NULL;
      case ImGuiToastType_Success:
        return ICON_FA_CHECK_CIRCLE;
      case ImGuiToastType_Warning:
        return ICON_FA_EXCLAMATION_TRIANGLE;
      case ImGuiToastType_Error:
        return ICON_FA_TIMES_CIRCLE;
      case ImGuiToastType_Info:
        return ICON_FA_INFO_CIRCLE;
      default:
        return NULL;
    }
  }

  NOTIFY_INLINE auto get_content() -> char * { return this->content; };

  NOTIFY_INLINE auto get_elapsed_time() { return get_tick_count() - this->creation_time; }

  NOTIFY_INLINE auto get_phase() -> const ImGuiToastPhase {
    const auto elapsed = get_elapsed_time();

    if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time + NOTIFY_FADE_IN_OUT_TIME) {
      return ImGuiToastPhase_Expired;
    } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time) {
      return ImGuiToastPhase_FadeOut;
    } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME) {
      return ImGuiToastPhase_Wait;
    } else {
      return ImGuiToastPhase_FadeIn;
    }
  }

  NOTIFY_INLINE auto get_fade_percent() -> const float {
    const auto phase = get_phase();
    const auto elapsed = get_elapsed_time();

    if (phase == ImGuiToastPhase_FadeIn) {
      return ((float) elapsed / (float) NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
    } else if (phase == ImGuiToastPhase_FadeOut) {
      return (1.f - (((float) elapsed - (float) NOTIFY_FADE_IN_OUT_TIME - (float) this->dismiss_time) / (float) NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
    }

    return 1.f * NOTIFY_OPACITY;
  }

  NOTIFY_INLINE static auto get_tick_count() -> const unsigned long long {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
  }

  public:
  // Constructors
  ImGuiToast(ImGuiToastType type, std::string title, std::string content, int dismiss_time = NOTIFY_DEFAULT_DISMISS) : ImGuiToast(type, dismiss_time) {
    this->set_title(title.c_str());
    this->set_content(content.c_str());
  }

  ImGuiToast(ImGuiToastType type, int dismiss_time = NOTIFY_DEFAULT_DISMISS) {
    IM_ASSERT(type < ImGuiToastType_COUNT);

    this->type = type;
    this->dismiss_time = dismiss_time;
    this->creation_time = get_tick_count();

    memset(this->title, 0, sizeof(this->title));
    memset(this->content, 0, sizeof(this->content));
  }

  ImGuiToast(ImGuiToastType type, const char *format, ...) : ImGuiToast(type) { NOTIFY_FORMAT(this->set_content, format); }

  ImGuiToast(ImGuiToastType type, int dismiss_time, const char *format, ...) : ImGuiToast(type, dismiss_time) { NOTIFY_FORMAT(this->set_content, format); }
};

namespace ImGui {
  NOTIFY_INLINE std::vector<ImGuiToast> notifications;

  /// <summary>
  /// Insert a new toast in the list
  /// </summary>
  NOTIFY_INLINE void InsertNotification(const ImGuiToast &toast) {
    notifications.push_back(toast);
  }

  /// <summary>
  /// Remove a toast from the list by its index
  /// </summary>
  /// <param name="index">index of the toast to remove</param>
  NOTIFY_INLINE void RemoveNotification(int index) {
    notifications.erase(notifications.begin() + index);
  }

  /// <summary>
  /// Render toasts, call at the end of your rendering!
  /// </summary>
  NOTIFY_INLINE void RenderNotifications() {
    ImDrawList *draw_list = GetForegroundDrawList();
    const auto vp_size = GetMainViewport()->Size;

    float height = 0.f;

    for (auto i = 0; i < notifications.size(); i++) {
      auto *current_toast = &notifications[i];

      if (current_toast->get_phase() == ImGuiToastPhase_Expired) {
        RemoveNotification(i);
        continue;
      }

      const auto icon = current_toast->get_icon();
      const auto title = current_toast->get_title();
      const auto content = current_toast->get_content();
      const auto default_title = current_toast->get_default_title();
      const auto opacity = current_toast->get_fade_percent();

      auto text_color = current_toast->get_color();
      auto text_color_ = ImColor(255, 255, 255, 255);
      text_color.w = opacity;

      ImVec2 textSize = NOTIFY_NULL_OR_EMPTY(title) ? ImVec2(0, 0) : CalcTextSize(title);
      ImVec2 defaultTitleSize = NOTIFY_NULL_OR_EMPTY(default_title) ? ImVec2(0, 0) : CalcTextSize(default_title);
      ImVec2 iconSize = NOTIFY_NULL_OR_EMPTY(icon) ? ImVec2(0, 0) : CalcTextSize(icon);
      ImVec2 contentSize = NOTIFY_NULL_OR_EMPTY(content) ? ImVec2(0, 0) : CalcTextSize(content);

      float iconTextSpacing = NOTIFY_NULL_OR_EMPTY(icon) ? 0 : 10.0f;
      float textContentSpacing = NOTIFY_NULL_OR_EMPTY(content) ? 0 : 10.0f;

      float maxIconAndTextWidth = (iconSize.x + textSize.x) > (iconSize.x + defaultTitleSize.x) ? (iconSize.x + textSize.x) : (iconSize.x + defaultTitleSize.x);
      float maxContentWidth = maxIconAndTextWidth > contentSize.x ? maxIconAndTextWidth : contentSize.x;

      float totalWidth = NOTIFY_PADDING_X + maxContentWidth + iconTextSpacing + textContentSpacing + NOTIFY_PADDING_X;

      float maxTextHeight = textSize.y > defaultTitleSize.y ? textSize.y : defaultTitleSize.y;

      float totalHeight = NOTIFY_PADDING_MESSAGE_Y + iconSize.y + maxTextHeight + contentSize.y + textContentSpacing + NOTIFY_PADDING_MESSAGE_Y - 15.0f;


      ImVec2 pos = ImVec2(vp_size.x - totalWidth - NOTIFY_PADDING_X, vp_size.y - totalHeight - height - NOTIFY_PADDING_Y);

      draw_list->AddRectFilled(pos, ImVec2(pos.x + totalWidth, pos.y + totalHeight), IM_COL32(43, 43, 43, 255 * opacity), 5.0f);


      ImU32 im_text_color = ImColor(text_color);

      pos.x += NOTIFY_PADDING_X;
      pos.y += NOTIFY_PADDING_MESSAGE_Y;

      if (!NOTIFY_NULL_OR_EMPTY(icon)) {
        draw_list->AddText(pos, im_text_color, icon);
        pos.x += iconSize.x + iconTextSpacing;
      }

      if (!NOTIFY_NULL_OR_EMPTY(title)) {
        draw_list->AddText(pos, text_color_, title);
        pos.y += textSize.y + textContentSpacing;
        pos.x = vp_size.x - totalWidth - NOTIFY_PADDING_X + NOTIFY_PADDING_X;
      } else if (!NOTIFY_NULL_OR_EMPTY(default_title))// Check if the default title is not null here and render it
      {
        ImVec2 defaultTitleSize = CalcTextSize(default_title);

        draw_list->AddText(pos, text_color_, default_title);
        pos.y += defaultTitleSize.y + textContentSpacing;
        pos.x = vp_size.x - totalWidth - NOTIFY_PADDING_X + NOTIFY_PADDING_X;
      }

      if (!NOTIFY_NULL_OR_EMPTY(content)) {
        if (title) {
          draw_list->AddLine(ImVec2(pos.x, pos.y - 5.0f),
                             ImVec2(pos.x + totalWidth - NOTIFY_PADDING_X * 2, pos.y - 5.0f),
                             IM_COL32(100, 100, 100, 255 * opacity));
        }
        draw_list->AddText(pos, text_color_, content);
      }

      height += totalHeight + NOTIFY_PADDING_Y;
    }
  }

  /// <summary>
  /// Adds font-awesome font, must be called ONCE on initialization
  /// <param name="FontDataOwnedByAtlas">Fonts are loaded from read-only memory, should be set to false!</param>
  /// </summary>
  NOTIFY_INLINE void MergeIconsWithLatestFont(float font_size, bool FontDataOwnedByAtlas = false) {
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

    GetIO().Fonts->AddFontFromMemoryTTF((void *) fa_solid_900, sizeof(fa_solid_900), font_size, &icons_config, icons_ranges);
  }
}// namespace ImGui