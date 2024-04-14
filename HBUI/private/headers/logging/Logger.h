//
// Created by Kasper de bruin on 04/04/2024.
//

#ifndef IMGUI_LOGGER_H
#define IMGUI_LOGGER_H

#include <HBUI/HBUI.h>

#include <imgui.h>
#include <logging/LogManager.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

namespace HBUI::Logging {
  struct logItem {
    std::string               message;
    spdlog::level::level_enum level;
  };

  template<typename Mutex>
  class Logger : public spdlog::sinks::base_sink<Mutex> {
  public:
    Logger() {
    }

    ~Logger() {}

    std::shared_ptr<spdlog::logger> convertToSpdLogger(
        const std::shared_ptr<HBUI::Logging::Logger<std::mutex>> &originalLogger) {
      // Create a new spdlog::logger instance
      auto spdLogger = std::make_shared<spdlog::logger>("logger_name");


      return spdLogger;
    }


  public:
//    void addMessage(const spdlog::details::log_msg &msg) {
//      LogManager::log(msg);
//    }

    void                  clear() {}
    std::vector<logItem> &getMessages() {
      return m_Messages;
    }

    [[nodiscard]] const std::string &getName() const {
      return m_Name;
    }

    void setName(const std::string &name) {
      m_Name = name;
    }

  protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
      logItem it = {
          .message = std::string(msg.payload.data(), msg.payload.size()),
          .level   = msg.level};
     //format the message
//DEFAULT_LOG_PATTERN

      m_Messages.push_back(it);
    }
    void flush_() override {}

  private:
    std::vector<logItem>                                          m_Messages = {};
    std::string                                                   m_Name;
  };

}// namespace HBUI::Logging


#endif//IMGUI_LOGGER_H
