//
// Created by user on 6/3/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Engine.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/3/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Engine.h"
#include "Engine.inl"
#include "App.h"

namespace GlassEye {
void Engine::Start() {
  Logger::Init();

  GLASS_CORE_TRACE("======== GLASS EYE ENGINE INITIALIZED ===========");
  app->Init(Registry);
}
void Engine::Update() {
  app->Update(Registry, 0.16f);
}
void Engine::Shutdown() {
  app->Shutdown(Registry);
}
}