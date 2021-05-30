#include "HelloTriangleApplication.h"

int main(){
  entt::registry registry;

  Sandbox::HelloTriangleApplication app;

  try
  {
    app.Init(registry);
    app.Update(registry, 0.16f);
    app.Shutdown(registry);
  }
  catch (const std::exception& e) {

  }
}