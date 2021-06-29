//
// Created by user on 6/3/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Engine.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/3/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ENGINE_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ENGINE_H_

namespace GlassEye {
class App;

class Engine {
 public:
  Engine() = default;
  ~Engine() = default;

  template<typename GameClass>
  uint32 Run(); // You might get a warning/error if you don't include EnginePCH in your .cpp!

  // Non copyable
  Engine(const Engine &) = delete;
  Engine(Engine &&) = delete;
  Engine &operator=(const Engine &) = delete;
  Engine &operator=(Engine &) = delete;

 private:
  void Start();
  void Update();
  void Shutdown();

 private:
  // Members
  App *app = nullptr; /* This can't be std::unique because of privacy issues */
  entt::registry Registry;

};

}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ENGINE_H_
