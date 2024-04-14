//
// Created by Kasper de bruin on 04/04/2024.
//

#include "Logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"


#include <spdlog/details/log_msg.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define DEFAULT_LOG_PATTERN "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] [%s:%#] %v"

logger_t createLogger(const std::string &name, bool isDefault) {
  auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  consoleSink->set_pattern(DEFAULT_LOG_PATTERN);

  auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + name + ".log", true);
  fileSink->set_pattern(DEFAULT_LOG_PATTERN);

  std::shared_ptr<spdlog::logger> llog = spdlog::create<HBUI::Logging::Logger<std::mutex>>(name + " imgui");
  llog->set_pattern(DEFAULT_LOG_PATTERN);

  auto coreLogger = std::make_shared<spdlog::logger>(spdlog::logger(name, {llog->sinks()[0], consoleSink}));
  coreLogger->set_pattern(DEFAULT_LOG_PATTERN);

  std::shared_ptr<HBUI::Logging::Logger<std::mutex>> found = std::dynamic_pointer_cast<HBUI::Logging::Logger<std::mutex>>(coreLogger->sinks()[0]);
  found->setName(name);
  found->set_pattern(DEFAULT_LOG_PATTERN);

  spdlog::register_logger(coreLogger);
  spdlog::set_default_logger(coreLogger);
  spdlog::log(spdlog::level::info, "Logger created: {}", name);
  spdlog::warn("Logger created: {}", name);
  return coreLogger;
}
//
//logger_t createLogger(const std::string& name, bool isDefault) {
//  auto llog = spdlog::create<HBUI::Logging::Logger<std::mutex>>(name);
//
//  // I don't understand this step, shouldn't it log to all registered loggers?
//  if(isDefault) {
//    spdlog::set_default_logger(llog);
//  }
//
//  //add console log
//  llog->sinks().push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
//
//  auto logger = std::dynamic_pointer_cast<HBUI::Logging::Logger<std::mutex>>(llog->sinks()[0]);
//  logger->setName(name);
//  return logger;
//}