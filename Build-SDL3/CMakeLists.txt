cmake_minimum_required(VERSION 3.31.6)
set(CMAKE_C_STANDARD 23)
project(HE-LightBulb)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Lib/SDL3_INCLUDE/SDL3/" CACHE PATH "Path to SDL3 directory")
# Safety checks to ensure headers exist at the configured locations before building

#-----------------------------------------------------------------------------
# 2. Target Setup
#-----------------------------------------------------------------------------
# Define your application executable and its main implementation source file
add_executable(${PROJECT_NAME} EntryPoint.c)

# Configure header paths pointing to your local system or repository directories
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL_INCLUDE})
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
if(APPLE)
    # macOS Configuration: Native Metal Backend
    target_compile_definitions(${PROJECT_NAME} PRIVATE SOKOL_METAL)

    # Resolve system frameworks required for window creation and Metal rendering
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(METAL_FRAMEWORK Metal REQUIRED)
    find_library(METALKIT_FRAMEWORK MetalKit REQUIRED)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_FRAMEWORK}
        ${METAL_FRAMEWORK}
        ${METALKIT_FRAMEWORK}
        ${QUARTZCORE_FRAMEWORK}
    )

elseif(WIN32)
    # Windows Configuration: Native Direct3D11 Backend
    target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_METAL)

    # System APIs required by sokol_app and sokol_gfx
    target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        dxgi
        kernel32
        user32
        gdi32
    )

else()
    # Linux / Unix Configuration: OpenGL 3.3 Core Profile Backend
    target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_GLCORE33)

    # Discover system multi-threading and core OpenGL libraries
    find_package(Threads REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)

    # Sokol windowing targets X11 extension libraries on Linux
    find_library(XI_LIB Xi REQUIRED)
    find_library(XCURSOR_LIB Xcursor REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        Threads::Threads
        OpenGL::GL
        ${X11_LIBRARIES}
        ${XI_LIB}
        ${XCURSOR_LIB}
        m   # Unix Math Library
        dl  # Dynamic Loading Library interface
    )
endif()
