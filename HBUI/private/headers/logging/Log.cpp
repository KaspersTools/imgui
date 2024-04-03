//
// Created by Kasper de bruin on 03/04/2024.
//

#include "Log.h"

#include "spdlog/details/log_msg.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace HBUI::Logging {
  std::shared_ptr<spdlog::logger> Log::s_coreLogger = nullptr;

  void Log::init() {
    if (s_isInitialized)
      return;
    s_isInitialized = true;
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] %n | %s-%!():%# | %v%$");
    s_logSinks.emplace_back(consoleSink);

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("HummingBirdCore.log", true);
    fileSink->set_pattern("[source %s] [function %!] [line %#] %v");
    s_logSinks.emplace_back(fileSink);

    s_coreLogger = std::make_shared<spdlog::logger>("HummingBirdCore", begin(s_logSinks), end(s_logSinks));
    s_coreLogger->set_level(spdlog::level::trace);

    spdlog::register_logger(s_coreLogger);

  }

  void Log::log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
    s_coreLogger->log(loc, lvl, msg);
  }
}// namespace HBUI