cmake_minimum_required(VERSION 3.15)
project(ProiectulMeu CXX C) # Avem și C++ (*.cpp) și C (glad/*.c)

# 1. Păstrăm flag-ul de debug (-g) și activăm culorile în diagnostice
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fdiagnostics-color=always")

# Setăm standardul C++ (opțional, ex: C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Căutăm automat fișierele sursă din proiect

# Caută fișierele .cpp doar în folderul rădăcină (fără a intra recursiv în build)
file(GLOB SOURCES "*.cpp")

# Caută fișierele .c din folderul glad
file(GLOB GLAD_SOURCES "glad/*.c")

# Combină-le într-o singură listă
list(APPEND SOURCES ${GLAD_SOURCES})

# 3. Detectăm GLFW3 pe sistem (Mac, Linux sau Windows)
find_package(glfw3 REQUIRED)

# 4. Detectăm OpenGL pe sistem (Agnostic: va găsi -lGL pe Linux și Framework-ul pe Mac)
find_package(OpenGL REQUIRED)

# 5. Definim executabilul final numit 'main'
add_executable(main ${SOURCES})

# 6. Include automat folderele necesare (ex: folderul 'glad' ca să poți scrie #include <glad/glad.h>)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/glad)

# 7. Linkare inteligentă cross-platform
target_link_libraries(main PRIVATE 
    glfw 
    OpenGL::GL
)

