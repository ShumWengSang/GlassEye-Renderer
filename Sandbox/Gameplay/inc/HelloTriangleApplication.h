//
// Created by user on 5/28/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: HelloTriangleApplication.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 5/28/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_SANDBOX_GAMEPLAY_SRC_HELLOTRIANGLEAPPLICATION_H_
#define GLASSEYEENGINE_SANDBOX_GAMEPLAY_SRC_HELLOTRIANGLEAPPLICATION_H_
#include "App.h"

namespace Sandbox {
class HelloTriangleApplication : public GlassEye::App {
 public:
  HelloTriangleApplication();
  virtual void Init(entt::registry &);
  virtual void Shutdown(entt::registry&);
  virtual void Update(entt::registry&, float dt);

};
}

#endif //GLASSEYEENGINE_SANDBOX_GAMEPLAY_SRC_HELLOTRIANGLEAPPLICATION_H_
