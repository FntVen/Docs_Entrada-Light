cmake_minimum_required(VERSION 4.4.2)
set(CMAKE_C_STANDARD 23)
project(HE-LightBulb)

add_executable(${PROJECT_NAME} EntryPoint.c clay.h)
add_library(RayRender ${CMAKE_INCLUDE_CURRENT_DIR}/Lib/clay_renderer_raylib.c)
target_include_directories(RayRender PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR}/Lib)
target_link_libraries(${PROJECT_NAME} RayRender)
