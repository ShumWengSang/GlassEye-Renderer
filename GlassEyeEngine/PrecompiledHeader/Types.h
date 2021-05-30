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
constexpr Ref<T> CreateRef(Args &&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args &&... args) {
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

}