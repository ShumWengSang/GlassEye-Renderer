#pragma once
/*
==============================================
Custom Types (Part of PCH!)
==============================================
*/

namespace GlassEye {
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args &&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> MakeUnique(Args &&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

using namespace entt::literals;
using GUID = entt::hashed_string;
using GUID_Handle = entt::hashed_string::hash_type;
using AssetHandle = GUID_Handle;
static GUID INVALID_GUID = {"INVALID_GUID"_hs};

using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uint8 = std::uint8_t;
using float32 = std::float_t;
using double64 = std::double_t;
using s8 = char;
using u8 = unsigned char;

}