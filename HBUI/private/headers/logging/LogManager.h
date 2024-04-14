//
// Created by Kasper de bruin on 05/04/2024.
//

#ifndef IMGUI_LOGMANAGER_H
#define IMGUI_LOGMANAGER_H

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class LogManager {
public:
  static void log(const spdlog::details::log_msg &msg) {
    for(auto& sink : s_logSinks) {
      sink->log(msg);
    }
  }
public:
  inline static std::shared_ptr<spdlog::logger> s_coreLogger = nullptr;
  inline static std::vector<spdlog::sink_ptr> s_logSinks = {};
};


#endif//IMGUI_LOGMANAGER_H
