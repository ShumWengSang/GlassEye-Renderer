# Modified From FlingEngine https://github.com/flingengine/FlingEngine
project("GlassEyeEngine")

############## Setup Git version numbers #######################
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Macro included from root CMake, that includes CMakeResources/EngineInclude.cmake
CONFIGURE_PLATFORM_ANNOYING_WARNINGS()

message( STATUS "Generated git file is : Core/inc/GitVersion_TEMPLATE.h" )

# Configure generated files from CMake
configure_file(
        Core/inc/GitVersion_TEMPLATE.h.in
        ${GENERATED_INC_FOLDER}/GitVersion.h
)

# Set this variable for configuring file later
set ( FLING_ENGINE_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" )

# Include the GlassEye Engine header files that we may need
GLASSEYE_ENGINE_INCLUDE("")

########### Linking ############################################
find_package(Vulkan REQUIRED)

## Ensure we have Vulkan
if(Vulkan_FOUND)
    message(STATUS "Vulkan found!" )
else()
    message(FATAL_ERROR "Vulkan NOT FOUND! Stopping" )
endif()

set ( LINK_LIBS
        glfw ${GLFW_LIBRARIES}
        Vulkan::Vulkan
        )

IF( WITH_IMGUI_FLAG )
    set ( LINK_LIBS ${LINK_LIBS}
            ImGui ${IMGUI_LIBRARIES}
            )
endif()

# link pthread if we need to
if ( NOT WIN32 )
    set( LINK_LIBS ${LINK_LIBS} pthread )
    message( STATUS "Added pthread!" )
endif()

message( STATUS "LINK_LIBS is : " ${LINK_LIBS} )

################## Complier Options ###########################
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wno-reorder -Wno-unknown-pragmas -Wno-multichar -Wno-missing-braces" )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE source_list
        *.cpp* src/*.h* src/*.hpp* *.h* ${GENERATED_INC_FOLDER}/*.h ${GENERATED_INC_FOLDER}/*.cpp *.inl
        )
if( MSVC )
    foreach( _source IN ITEMS ${source_list} )
        get_filename_component( _source_path "${_source}" PATH )
        string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
        string( REPLACE "/" "\\" _group_path "${_group_path}" )
        source_group( "${_group_path}" FILES "${_source}" )
    endforeach()
endif()

##################### Add library and link ########################
add_library ( ${PROJECT_NAME} ${source_list} )

# Make sure the compiler can find include files for our Engine library
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# link against the libs that the engine needs
target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS} )

# Precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE "PrecompiledHeader/EnginePCH.cpp")
