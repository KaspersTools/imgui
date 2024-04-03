//
// Created by Kasper de bruin on 03/04/2024.
//

#include "LogWindow.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace Logging {

}// namespace Logging

logWindow_t logWindow() {
  auto llog = spdlog::create<HBUI::Logging::LogWindow<std::mutex>>(std::string("LogWindow"));

  // I don't understand this step, shouldn't it log to all registered loggers?
  spdlog::set_default_logger(llog);

  return std::dynamic_pointer_cast<HBUI::Logging::LogWindow<std::mutex>>(llog->sinks()[0]);
}