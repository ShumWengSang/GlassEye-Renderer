// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>  // Necessary for std::min/std::max
#include <array>
#include <cstdint>  // Necessary for UINT32_MAX
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <memory>

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
#pragma warning(pop)

#define UNREFERENCED_PARAMETER(x) (void)x

namespace GlassEye {
template <typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args&& ... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using float32 = std::float_t;
using double64 = std::double_t;
using s8 = char;
using u8 = unsigned char;


#define	EPSILON	  0.00001f
#define	PI	      3.1415926f
#define	HALF_PI	  (PI * 0.5f)
#define	TWO_PI	  (PI * 2.0f)
}