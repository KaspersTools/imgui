//
// Created by Kasper de bruin on 03/04/2024.
//

#ifndef IMGUI_LOGWINDOW_H
#define IMGUI_LOGWINDOW_H
#include <HBUI/HBUI.h>
#include <imgui.h>
#include <logging/Logger.h>
#include <spdlog/spdlog.h>

#include <utility>

namespace HBUI::Windows::Logging {
  inline static ImColor TXT_DBG_CLR{0.5f, 0.5f, 0.5f, 1.0f};

  inline static ImColor LVL_DBG_CLR{0.0f, 1.0f, 0.8f, 1.0f};
  inline static ImColor LVL_LOG_CLR{0.65f, 0.2f, 1.0f, 1.0f};
  inline static ImColor LVL_ERR_CLR{1.0f, 0.0f, 0.0f, 1.0f};
  inline static ImColor LVL_WRN_CLR{1.0f, 0.5f, 0.0f, 1.0f};

  class LogWindow {
  public:
    LogWindow(std::shared_ptr<HBUI::Logging::Logger<std::mutex>> logger) : m_logger(logger) {
    }
    ~LogWindow() = default;

    void draw(bool *p_open) {
      const std::string &title = m_logger->getName();
      ImGui::Begin(title.c_str(), p_open);
      if (ImGui::Button("Clear")) {
        m_logger->flush();
      }
      ImGui::Text("Auto-scroll:");
      ImGui::SameLine();
      ImGui::Checkbox("##Auto-scroll", &m_autoScroll);
      ImGui::SameLine();
      ImGui::Text("Max shown:");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
      ImGui::SliderInt("##Max shown", &m_MaxShown, 0, 5000);
      ImGui::Separator();

      ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
      for (size_t i = m_logger->getMessages().size() > m_MaxShown ? m_logger->getMessages().size() - m_MaxShown : 0;
           i < m_logger->getMessages().size(); i++) {
        const auto &msg = m_logger->getMessages()[i];

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
            ImGui::TextColored(LVL_WRN_CLR, "%s", msg.message.c_str());
            break;
          case spdlog::level::err:
            ImGui::TextColored(LVL_ERR_CLR, "ERR");
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SameLine();
            ImGui::TextColored(LVL_ERR_CLR, "%s", msg.message.c_str());
            break;
          default:
            ImGui::TextColored(LVL_LOG_CLR, "LOG");
            ImGui::SameLine();
            ImGui::Text("|");
            ImGui::SameLine();
            ImGui::TextColored(LVL_LOG_CLR, "%s", msg.message.c_str());
            break;
        }
      }
      if (m_Scroll) {
        if (m_autoScroll) {
          ImGui::SetScrollHereY();
          m_Scroll = false;
        }
      }
      ImGui::EndChild();
      ImGui::End();
    }
    void setMaxShown(size_t maxShown) { m_MaxShown = maxShown; }

  protected:
    bool                               m_Scroll     = true;
    int                                m_MaxShown   = 1024;
    bool                               m_autoScroll = true;
    std::shared_ptr<HBUI::Logging::Logger<std::mutex>> m_logger     = nullptr;
  };

}// namespace HBUI::Windows::Logging


#endif//IMGUI_LOGWINDOW_H
