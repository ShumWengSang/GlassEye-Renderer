# GlassEye-Renderer

My own implementation of a Engine using a Vulkan renderer. 

The project is managed through CMake, and the engine is split from the sandbox application as a library.

# Requirements

> \>= CMake 3.17 C++ 17 (project uses MSVC with CLion)

# Dependancies


## The following third party libraries are submoduled

- ENTT
- imgui_entt_entity_editor
- GLM
- STB (texture loading)
- Tiny Obj Loader (obj loading)
- GLFW
- ImGui
- spdlog

## You will need to download these yourself

- Vulkan SDK (Lunar) [Vulkan Development Environment Site](https://vulkan-tutorial.com/Development_environment)