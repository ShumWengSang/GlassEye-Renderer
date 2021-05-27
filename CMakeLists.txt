cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME GlassEye)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Source/deps/)

file(GLOB SOURCES "Source/src/*.c" "Source/src/*.cpp")
# Remove the stdafx from the glob
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/Source/src/stdafx.cpp")

# Glob the imple for imgui
# file(GLOB EXT_SOURCES "Source/externalSrc/*.c" "Source/externalSrc/*.cpp" "Source/externalSrc/*.hpp")

# We sometimes want to compile files as both C++ and GLASL.
# This allows us to access files from the shader directory.
file(GLOB SHADER_CROSS_SOURCES "Assets/Shaders/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${SHADER_CROSS_SOURCES} ${EXT_SOURCES})

source_group("Source" FILES ${SOURCES})
source_group("Cross Shaders" FILES ${SHADER_CROSS_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "Source/src/")
target_include_directories(${PROJECT_NAME} PRIVATE "Source/externalSrc/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Source/deps/stb_image/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Source/deps/tinyobjloader/include")


target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE stb_image)
target_link_libraries(${PROJECT_NAME} PRIVATE glm_static)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader)

# Vulkan: Find it first with find_package. If not found, look in internal directories
find_package(Vulkan)
IF (NOT Vulkan_FOUND)
    if (WIN32)
        find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/Source/externalSrc/vulkan)
    ELSEIF (LINUX)
        find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
    ELSE ()
        message("Error! Can't find Vulkan!")
    ENDIF ()
    IF (Vulkan_LIBRARY)
        set(Vulkan_FOUND ON)
        MESSAGE("Using bundled Vulkan library version")
    ENDIF ()
ENDIF ()
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE "Source/src/stdafx.cpp")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
        )
