//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: WindowsWindow.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_INC_WINDOWSWINDOW_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_INC_WINDOWSWINDOW_H_
#include "Window.h"
namespace GlassEye {
class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProperties &properties);
  ~WindowsWindow() override;

  void ProcessEvents() override;
  void SwapBuffers() override;

  uint32 GetWidth() const override;
  uint32 GetHeight() const override;
  std::pair<uint32, uint32> GetSize() const override;
  std::pair<int, int> GetWindowPos() const override;

  void Maximize() override;
  void SetEventCallback(const std::function<void(class Event &)> &callback) override;
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  std::string_view GetTitle() const override;
  void SetTitle(std::string title) override;

  void *GetNativeWindow() const override;
  /** get the current GLFW window */
  GLFWwindow *GetGlfwWindow() const;

  Ref <VulkanContext> GetRenderContext() override;

 private:
  void Init(const WindowProperties &properties);
  void Shutdown();
 private:
  GLFWwindow *Window_;
  GLFWcursor *ImGuiMouseCursor_[9] = {nullptr};

  struct WindowData {
    std::string Title;
    uint32 Width;
    uint32 Height;
    bool VSync;
    EventCallbackFunction EventCallback;
  } Data_;

  float LastFrameTime_ = 0.0f;
  Ref <VulkanContext> VulkanRendererContext_;
};

}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_INC_WINDOWSWINDOW_H_
