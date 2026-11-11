#include "core/log.hpp"

#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>

namespace engine {
std::chrono::steady_clock::time_point g_start;

namespace {

// "project/src/renderer/mesh.cpp" -> "mesh.cpp"
std::string_view shortFile(const char *path) {
    std::string_view p{path};
    auto slash = p.find_last_of("/\\");
    return slash == std::string_view::npos ? p : p.substr(slash + 1);
}

std::string_view levelName(LogLevel l) {
    switch (l) {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO ";
    case LogLevel::Warn:
        return "WARN ";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    }
    return "?????";
}

std::string_view levelColor(LogLevel l) {
    switch (l) {
    case LogLevel::Trace:
        return "\x1b[90m"; // grey
    case LogLevel::Debug:
        return "\x1b[36m"; // cyan
    case LogLevel::Info:
        return "\x1b[32m"; // green
    case LogLevel::Warn:
        return "\x1b[33m"; // yellow
    case LogLevel::Error:
        return "\x1b[31m"; // red
    case LogLevel::Fatal:
        return "\x1b[41m"; // red background
    }
    return "";
}

std::string formatLine(const LogMessage &m) {
    return std::format("[{:9.3f}] [{}] {}:{}: {}", m.seconds,
                       levelName(m.level), shortFile(m.where.file_name()),
                       m.where.line(), m.text);
}
} // namespace

bool Log::isValid(LogLevel level) {
    auto value = static_cast<uint8_t>(level);

    return value <= static_cast<uint8_t>(LogLevel::Fatal);
}

void Log::init() { g_start = std::chrono::steady_clock::now(); }

void Log::shutdown() {
    std::lock_guard lock(s_mutex);

    for (auto &sink : sinks)
        sink->flush();

    sinks.clear();
}

void Log::dispatch(LogLevel level, std::string_view text,
                   std::source_location where) {
    double seconds = std::chrono::duration<double>(
                         std::chrono::steady_clock::now() - g_start)
                         .count();

    LogMessage msg{level, seconds, text, where};

    std::lock_guard lock(s_mutex);

    for (auto &sink : sinks) {
        sink->write(msg);

        if (level >= LogLevel::Error)
            sink->flush();
    }
}

void ConsoleSink::write(const LogMessage &msg) {
    fprintf(stdout, "%s%s\x1b[0m\n", levelColor(msg.level).data(),
            formatLine(msg).c_str());
}

struct FileSink::Impl {
    std::ofstream file;
};

FileSink::FileSink(const char *path) : m_impl{std::make_unique<Impl>()} {
    std::filesystem::path p{path};

    if (p.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(p.parent_path(), ec);
        if (ec) {
            std::fprintf(stderr, "FileSink: cannot create folder '%s': %s\n",
                         p.parent_path().string().c_str(),
                         ec.message().c_str());
        }
    }

    m_impl->file.open(p, std::ios::out | std::ios::trunc);
    if (!m_impl->file) {
        std::fprintf(stderr, "FileSink: cannot open '%s'\n", path);
    }
}

FileSink::~FileSink() = default;

void FileSink::flush() { m_impl->file.flush(); }

void FileSink::write(const LogMessage &msg) {
    if (m_impl->file)
        m_impl->file << formatLine(msg) << '\n';
}

} // namespace engine
