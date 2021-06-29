# Modified From FlingEngine https://github.com/flingengine/FlingEngine
MACRO(CONFIGURE_PLATFORM_ANNOYING_WARNINGS )
    if( MSVC )
        add_definitions( -D_CRT_SECURE_NO_WARNINGS )
        # GCC
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions( -Wno-class-memaccess )
        # Clang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    endif()
ENDMACRO(CONFIGURE_PLATFORM_ANNOYING_WARNINGS)

MACRO(GLASSEYE_ENGINE_INCLUDE EngineDir)
    # Take care of warnings about strcpy
    CONFIGURE_PLATFORM_ANNOYING_WARNINGS()

    # Set Build Dir
    set( CMAKE_BINARY_DIR    "/build" )
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_Output/$<CONFIG>)
    set(CMAKE_CXX_STANDARD 20)

    # Sets the output directory
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )

    # TODO Only include editor folder if we have enabled the editor!

    include_directories(
            ${EngineDir}Core/inc
            ${EngineDir}Graphics/inc
            ${EngineDir}Utils/inc
            ${EngineDir}Platform/inc
            ${EngineDir}Gameplay/inc
            ${EngineDir}Asset/inc
            ${EngineDir}PrecompiledHeader
            ${GENERATED_INC_FOLDER}		# Generated include files that cmake will handle (i.e. GitVersion)
    )

    if( WITH_EDITOR_FLAG )
        include_directories(
                ${EngineDir}Editor/inc
        )
    endif()

ENDMACRO(GLASSEYE_ENGINE_INCLUDE)