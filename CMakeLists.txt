cmake_minimum_required(VERSION 3.15)
project(GameEngine LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(external/glad)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE
    glad
    glfw
    OpenGL::GL
)

target_compile_options(main PRIVATE -Wall -Wextra -fdiagnostics-color=always)
