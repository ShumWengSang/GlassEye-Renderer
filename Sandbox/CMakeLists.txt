# Modified From FlingEngine https://github.com/flingengine/FlingEngine
project ("Sandbox")

##################### Engine Setup #########################
set(ENGINE_DIR ../GlassEyeEngine/)
GLASSEYE_ENGINE_INCLUDE(${ENGINE_DIR})

### Including our own directories
include_directories(Gameplay/inc)

# If editor
if(FLAG_WITH_EDITOR)
    include_directories(
            Editor/inc
    )
endif()

######################## Linking #######################
set (LINK_LIBS "GlassEyeEngine")

# Link pthread
if ( NOT WIN32 )
    set( LINK_LIBS ${LINK_LIBS} pthread )
    message( STATUS "Added pthread!" )
endif()
message( STATUS "LINK_LIBS is : " ${LINK_LIBS} )


###################### Compiler Options ########################
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wno-reorder -Wno-unknown-pragmas -Wno-multichar" )
endif()

##################### Setup source files #############################
file (GLOB_RECURSE source_list
        *.cpp* src/*.h src/*.hpp *.inl)

if( MSVC )
    foreach( _source IN ITEMS ${source_list} )
        get_filename_component( _source_path "${_source}" PATH )
        string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
        string( REPLACE "/" "\\" _group_path "${_group_path}" )
        source_group( "${_group_path}" FILES "${_source}" )
    endforeach()
endif()

################### Add EXE and Link ################
# Link our normal libraries and create an executeable
add_executable( ${PROJECT_NAME} ${source_list} )

# Link libs
target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS} )

# Post build command
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets
        )

# Precompiled header
# target_precompile_headers(${PROJECT_NAME} PRIVATE "stdafx.cpp") (Disabled as there isn't much use for now)