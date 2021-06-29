//
// Created by user on 6/15/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Window.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/15/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_WINDOW_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_WINDOW_H_
namespace GlassEye {
struct WindowProperties {
  std::string Title;
  uint32 Width;
  uint32 Height;
  bool Fullscreen = false;
  explicit WindowProperties(std::string title = "GlassEye Engine",
                            uint32 width = 1280,
                            uint32 height = 720,
                            bool fullscreen = false);
};
class VulkanContext;
class Window {
 public:
  using EventCallbackFunction = std::function<void(class Event &)>;

  virtual ~Window();
  virtual void ProcessEvents() = 0;
  virtual void SwapBuffers() = 0;

  virtual uint32 GetWidth() const = 0;
  virtual uint32 GetHeight() const = 0;
  virtual std::pair<uint32, uint32> GetSize() const = 0;
  virtual std::pair<int, int> GetWindowPos() const = 0;

  virtual void Maximize() = 0;

  // Window attributes
  virtual void SetEventCallback(const EventCallbackFunction &callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual std::string_view GetTitle() const = 0;
  virtual void SetTitle(std::string title) = 0;

  virtual void *GetNativeWindow() const = 0;
  virtual Ref <VulkanContext> GetRenderContext() = 0;

  static Window *Create(const WindowProperties &props = WindowProperties());

};
}
#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_WINDOW_H_
