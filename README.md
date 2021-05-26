# GlassEye-Renderer

My own implementation of a Vulkan renderer.

# Requirements

> \>= CMake 3.17 C++ 20 (project uses MSVC with CLion)

# Dependancies

~~Uses C++ 20. MSVC currently supports the most C++ 20 features out of all compilers~~
Reverted to C++17 as C++ 20 support is not mature enough.

## All of these libraries are embedded into the project for stability

- ENTT
- GLM
- STBI (texture loading)
- Tiny Obj Loader (obj loading)
- GLFW
- Luna SDK (Vulkan)
- ImGui

## You will need to download these yourself

- Vulkan SDK (Lunar) [Vulkan Development Environment Site](https://vulkan-tutorial.com/Development_environment)