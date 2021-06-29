//
// Created by user on 6/20/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_SINGLETON_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_SINGLETON_H_
namespace GlassEye {
template<typename T>
class Singleton {
 public:
  static T &Get();
  virtual void Init() {}
  virtual void Shutdown() {}
 protected:
  explicit Singleton<T>() = default;
};

template<typename T>
T &Singleton<T>::Get() {
  static_assert(std::is_default_constructible<T>::value,
                "T is required to be default constructible");

  static T m_Instance;

  return m_Instance;
}
}
#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_SINGLETON_H_
