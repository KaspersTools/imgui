//
// Created by Kasper de bruin on 03/04/2024.
//

#ifndef IMGUI_LOGWINDOW_H
#define IMGUI_LOGWINDOW_H
#include <HBUI/HBUI.h>

#include <imgui.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

namespace HBUI {
  namespace Logging {

    struct logItem {
      std::string               message;
      spdlog::level::level_enum level;
    };


    const ImColor TXT_DBG_CLR{0.5f, 0.5f, 0.5f, 1.0f};

    const ImColor LVL_DBG_CLR{0.0f, 1.0f, 0.8f, 1.0f};
    const ImColor LVL_LOG_CLR{0.65f, 0.2f, 1.0f, 1.0f};
    const ImColor LVL_ERR_CLR{1.0f, 0.0f, 0.0f, 1.0f};
    const ImColor LVL_WRN_CLR{1.0f, 0.5f, 0.0f, 1.0f};

    template<typename Mutex>
    class LogWindow : public spdlog::sinks::base_sink<Mutex> {

    public:
      LogWindow() {}
      ~LogWindow() {}

      void drawImgui(bool *p_open) {
        ImGui::Begin("Log", p_open);
        if (ImGui::Button("Clear")) {
          m_Messages.clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &m_autoScroll);
        ImGui::SameLine();
        ImGui::SliderInt("Max shown", &m_MaxShown, 0, 5000);
        ImGui::Separator();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (size_t i = m_Messages.size() > m_MaxShown ? m_Messages.size() - m_MaxShown : 0; i < m_Messages.size(); ++i) {
          auto const &msg = m_Messages[i];

          switch (msg.level) {
            case spdlog::level::debug:
              ImGui::TextColored(LVL_DBG_CLR, "DBG");
              ImGui::SameLine();
              ImGui::Text("|");
              ImGui::SameLine();
              ImGui::TextColored(LVL_DBG_CLR, "%s", msg.message.c_str());
              break;
            case spdlog::level::warn:
              ImGui::TextColored(LVL_WRN_CLR, "WRN");
              ImGui::SameLine();
              ImGui::Text("|");
              ImGui::SameLine();
              ImGui::Text("%s", msg.message.c_str());
              break;
            case spdlog::level::err:
              ImGui::TextColored(LVL_ERR_CLR, "ERR");
              ImGui::SameLine();
              ImGui::Text("|");
              ImGui::SameLine();
              ImGui::Text("%s", msg.message.c_str());
              break;
            default:
              ImGui::TextColored(LVL_LOG_CLR, "LOG");
              ImGui::SameLine();
              ImGui::Text("|");
              ImGui::SameLine();
              ImGui::TextUnformatted(msg.message.c_str());
              break;
          }
        }

        if (m_Scroll) {
          if(m_autoScroll) {
            ImGui::SetScrollHereY();
            m_Scroll = false;
          }
        }
        ImGui::EndChild();
        ImGui::End();
      }
      void setMaxShown(size_t maxShown) { m_MaxShown = maxShown; }

    protected:
      void sink_it_(const spdlog::details::log_msg &msg) override {
        logItem it;
        it.message = std::string(msg.payload.data(), msg.payload.size());
        it.level   = msg.level;
        m_Messages.push_back(it);
        m_Scroll = true;
      }
      void flush_() override {}

    protected:
      std::vector<logItem> m_Messages   = {};
      bool                 m_Scroll     = true;
      int                  m_MaxShown   = 1024;
      bool                 m_autoScroll = true;
    };

  }// namespace Logging
}// namespace HBUI

#endif//IMGUI_LOGWINDOW_H
