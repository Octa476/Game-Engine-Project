cmake_minimum_required(VERSION 3.24)
project(GameEngine LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
  GIT_TAG release-3.4.20
  GIT_SHALLOW TRUE
)

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.3 
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3 glm)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(engine ${SOURCES})
target_include_directories(engine PRIVATE src)
target_link_libraries(engine PRIVATE SDL3::SDL3 glm::glm)
target_compile_options(engine PRIVATE -Wall -Wextra)
