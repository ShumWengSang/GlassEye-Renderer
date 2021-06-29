#pragma once
/*
==============================================
Third Party Libs Include (Part of PCH!)
==============================================
*/

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <stb_image.h>

#include <tiny_obj_loader.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <entt/entt.hpp>
#include <entt/entity/registry.hpp>

#include <yaml-cpp/yaml.h>
#pragma warning(pop)