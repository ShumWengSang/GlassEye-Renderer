# Modified From FlingEngine https://github.com/flingengine/FlingEngine
cmake_minimum_required(VERSION 3.17)
project("GlassEyeEngine")

set ( GLASSEYE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )

OPTION( DEFINE_SHIPPING "DEFINE_SHIPPING configuration will change asset paths to be relative." OFF ) # disabled by default
OPTION( WITH_IMGUI_FLAG "WITH_IMGUI_FLAG will enable or disable the addition of IMGUI to the rendering pipeline. " ON )
OPTION( WITH_EDITOR_FLAG "Enables or disables the editor in the Fling Engine!" ON )

# We can't have the editor without ImGUI!
IF( NOT WITH_IMGUI_FLAG AND WITH_EDITOR_FLAG )
    SET( WITH_EDITOR_FLAG OFF )
    message( FATAL_ERROR    "WITH_EDITOR_FLAG cannot be enabled because WITH_IMGUI_FLAG is OFF!" )
endif()

message( STATUS "----- Build Config ----" )
message( STATUS "Compiler=${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "WITH_EDITOR_FLAG=${WITH_EDITOR_FLAG}" )
message( STATUS "WITH_IMGUI_FLAG=${WITH_IMGUI_FLAG}" )
message( STATUS "DEFINE_SHIPPING=${DEFINE_SHIPPING}" )

IF( WITH_IMGUI_FLAG )
    ADD_DEFINITIONS ( -DWITH_IMGUI=1 )
else()
    ADD_DEFINITIONS ( -DWITH_IMGUI=0 )
endif()

IF( WITH_EDITOR_FLAG )
    ADD_DEFINITIONS ( -DWITH_EDITOR=1 )
else()
    ADD_DEFINITIONS ( -DWITH_EDITOR=0 )
endif()

IF( DEFINE_SHIPPING )
    message( STATUS "Build set to SHIPPING configuration!" )
    ADD_DEFINITIONS ( -DFLING_SHIPPING )
    SET( CMAKE_BUILD_TYPE Release )
ENDIF( DEFINE_SHIPPING )
message( STATUS "-----------------------" )

# C++ 17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generated folder used for Cmake generated files
set( GENERATED_INC_FOLDER ${CMAKE_BINARY_DIR}/Generated )
message( STATUS "Generated dir: ${GENERATED_INC_FOLDER}" )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeResources")
include(EngineInclude)

message( STATUS "Cmake mod path: " ${CMAKE_MODULE_PATH} )

CONFIGURE_PLATFORM_ANNOYING_WARNINGS()

# Enable multiple processor compilation
if( MSVC )
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Include external libraries
add_subdirectory( External/glfw )
include_directories( External/glfw/include )

if( WITH_IMGUI_FLAG )
    add_subdirectory( External/imgui )
    include_directories( External/imgui )
    include_directories( External/imgui_entt_entity_editor )
endif()

include_directories( External/glm )
include_directories( External/spdlog/include )
include_directories( External/entt/src )
include_directories( External/stb )
include_directories( External/tinyobjloader )
include_directories( External/imgui-filebrowser )



# Add the engine and executable
add_subdirectory ( "GlassEyeEngine" )
add_subdirectory ( "Sandbox" )


