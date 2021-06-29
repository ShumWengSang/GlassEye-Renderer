//
// Created by user on 6/15/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Game.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/15/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_SANDBOX_GAMEPLAY_INC_SANDBOXGAME_H_
#define GLASSEYEENGINE_SANDBOX_GAMEPLAY_INC_SANDBOXGAME_H_
#include "App.h"

namespace Sandbox {
class SandboxGame : public GlassEye::App {
 public:
  virtual void Init(entt::registry &);
  virtual void Shutdown(entt::registry &);
  virtual void Update(entt::registry &, float dt);
};
}

#endif //GLASSEYEENGINE_SANDBOX_GAMEPLAY_INC_SANDBOXGAME_H_
