#include "core/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#define ENGINE_LOG_FILE "logs/engine_logs.log"

int main() {
    engine::Log::init();
    engine::Log::addSink<engine::ConsoleSink>();
    engine::Log::addSink<engine::FileSink>(
        engine::SinkFilePath{ENGINE_LOG_FILE});

    LOG_INFO("Engine starting");
    LOG_DEBUG("Window size: {}x{}", 1280, 720);
    LOG_ERROR("Failed to load {}", "player.gltf");

    engine::Log::shutdown();
    return 0;
}
