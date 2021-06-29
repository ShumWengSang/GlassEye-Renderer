#include "EnginePCH.h"
#include "Engine.h"
#include "Engine.inl"

#include "HelloTriangleApplication.h"

int main() {
  GlassEye::Engine engine;

  engine.Run<Sandbox::HelloTriangleApplication>();

}