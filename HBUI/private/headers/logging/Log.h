//
// Created by Kasper de bruin on 03/04/2024.
//

#ifndef IMGUI_LOG_H
#define IMGUI_LOG_H
#include <memory>

#include "UIItems/Windows/LogWindow/LogWindow.h"
#include "spdlog/spdlog.h"

namespace HBUI{
  template<typename T>
  using Ref = std::shared_ptr<T>;
  template<typename T, typename... Args>
  constexpr Ref<T> createRef(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
}

namespace HBUI::Logging {

  class Log {
  public:
    static void init();
    static void log   (spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::string_view_t msg);

    template<typename... Args>
        inline static void log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(loc, lvl, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void log(spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::source_loc{}, lvl, fmt, std::forward<Args>(args)...);
    }

    template<typename T>
    inline static void log(spdlog::level::level_enum lvl, const T &msg) {
      if (!s_isInitialized)
        init();

      log(spdlog::source_loc{}, lvl, msg);
    }

    template<typename... Args>
    inline static void trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }


    template<typename T>
    inline static void trace(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::trace, msg);
    }

    template<typename T>
    inline static void debug(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::debug, msg);
    }

    template<typename T>
    inline static void info(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::info, msg);
    }

    template<typename T>
    inline static void warn(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::warn, msg);
    }

    template<typename T>
    inline static void error(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::err, msg);
    }

    template<typename T>
    inline static void critical(const T &msg) {
      if (!s_isInitialized)
        init();
      log(spdlog::level::critical, msg);
    }

    static Ref<spdlog::logger> &getCoreLogger() {
      if (!s_isInitialized)
        init();
      return s_coreLogger;
    }

    inline static void getCoreLogLevel(spdlog::level::level_enum level) {
      if (!s_isInitialized)
        init();
      getCoreLogger()->set_level(level);
    }

  private:
    static Ref<spdlog::logger> s_coreLogger;
    inline static std::vector<spdlog::sink_ptr> s_logSinks = {};
    inline static bool s_isInitialized = false;
  };

}// namespace HBUI
// Core log macros
#define CORE_TRACE(...) ::HBUI::Logging::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                    spdlog::level::trace, __VA_ARGS__)
#define CORE_INFO(...) ::HBUI::Logging::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                   spdlog::level::info, __VA_ARGS__)
#define CORE_WARN(...) ::HBUI::Logging::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                   spdlog::level::warn, __VA_ARGS__)
#define CORE_ERROR(...) ::HBUI::Logging::Log::log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), \
                                                    spdlog::level::err, __VA_ARGS__)

#endif//IMGUI_LOG_H
