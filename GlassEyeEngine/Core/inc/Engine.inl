
#pragma once
#include <Engine.h>
namespace GlassEye {
template<typename GameClass>
uint32 Engine::Run() {
  app = new GameClass();

  Start();
  Update();
  Shutdown();

  delete app;

  return 0;
}

}