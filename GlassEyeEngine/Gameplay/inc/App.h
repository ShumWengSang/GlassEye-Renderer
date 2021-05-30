//
// Created by user on 5/28/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: App.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 5/28/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GAMEPLAY_SRC_APP_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GAMEPLAY_SRC_APP_H_
#include "ThirdPartyIncludes.h"
namespace GlassEye {
class App {
  virtual void Init(entt::registry&) = 0;
  virtual void Shutdown(entt::registry&) = 0;
  virtual void Update(entt::registry&, float dt) = 0;
  bool WantsToQuit() const;
 protected:
  App() = default;
  virtual ~App() = default;
  bool wantsToQuit = false;

};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GAMEPLAY_SRC_APP_H_
