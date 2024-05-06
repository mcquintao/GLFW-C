cmake_minimum_required(VERSION 3.5)
project(vulkan  LANGUAGES C VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(vulkan src/main.c ../include/glad/glad.h ../include/glad/glad.c)

target_link_libraries(vulkan glfw OpenGL::GL)