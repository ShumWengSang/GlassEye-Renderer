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

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


#include "stb_image.h"


#include <tiny_obj_loader.h>