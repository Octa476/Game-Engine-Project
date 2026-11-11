#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <mutex>
#include <source_location>
#include <string_view>
#include <utility>
#include <vector>

#define LOG_EXTENSTION ".log"

namespace engine {

enum class LogLevel : uint8_t { Trace, Debug, Info, Warn, Error, Fatal };

struct LogMessage {
    LogLevel level;
    double seconds;
    std::string_view text;
    std::source_location where;
};

class LogSink {
  public:
    virtual ~LogSink() = default;
    virtual void write(const LogMessage &msg) = 0;
    virtual void flush() {}
};

class Log {
  public:
    static void init();
    static void shutdown();

    template <typename Sink, typename... Args>
    static void addSink(Args &&...args) {
        static_assert(std::is_base_of<LogSink, Sink>());

        auto s = std::make_unique<Sink>(std::forward<Args>(args)...);

        std::lock_guard lock(s_mutex);

        sinks.push_back(std::move(s));
    }

    template <typename... Args>
    static void write(LogLevel level, std::source_location where,
                      std::format_string<Args...> fmt, Args &&...args) {
        if (!isValid(level)) // really shouldn't happen
            return;

        std::string text = std::format(fmt, std::forward<Args>(args)...);

        dispatch(level, text, where);
    }

    Log() = delete;
    ~Log() = delete;

  private:
    static void dispatch(LogLevel level, std::string_view text,
                         std::source_location where);
    static bool isValid(LogLevel level);

    static inline std::vector<std::unique_ptr<LogSink>> sinks;
    static inline std::mutex s_mutex;
};

class ConsoleSink : public LogSink {
  public:
    void write(const LogMessage &msg) override;
};

class FileSink : public LogSink {
  public:
    explicit FileSink(const char *path);
    ~FileSink() override;
    void flush() override;
    void write(const LogMessage &msg) override;

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    std::string_view m_path;
};
}; // namespace engine

#if defined(ENGINE_ENABLE_LOGGING)
#define ENGINE_LOG(level, ...)                                                 \
    ::engine::Log::write(level, std::source_location::current(), __VA_ARGS__)
#else
#define ENGINE_LOG(level, ...) ((void)0)
#endif

#define LOG_TRACE(...) ENGINE_LOG(::engine::LogLevel::Trace, __VA_ARGS__)
#define LOG_DEBUG(...) ENGINE_LOG(::engine::LogLevel::Debug, __VA_ARGS__)
#define LOG_ERROR(...) ENGINE_LOG(::engine::LogLevel::Error, __VA_ARGS__)
#define LOG_FATAL(...) ENGINE_LOG(::engine::LogLevel::Fatal, __VA_ARGS__)
#define LOG_INFO(...) ENGINE_LOG(::engine::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) ENGINE_LOG(::engine::LogLevel::Warn, __VA_ARGS__)
