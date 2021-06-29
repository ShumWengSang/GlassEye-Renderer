//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: WindowsWindow.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/

#include "EnginePCH.h"
#include "WindowsWindow.h"
#include "Logger.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Window.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDepthBuffer.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanPhysicalDevice.h"

namespace GlassEye {

static bool GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char *description) {
  GLASS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window *Window::Create(const WindowProperties &props) {
  return new GlassEye::WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProperties &properties) {
  Init(properties);
}
WindowsWindow::~WindowsWindow() {
  Shutdown();
}
void WindowsWindow::ProcessEvents() {
  glfwPollEvents();
  glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void WindowsWindow::SwapBuffers() {

}
uint32 WindowsWindow::GetWidth() const {
  return Data_.Width;
}
uint32 WindowsWindow::GetHeight() const {
  return Data_.Height;
}
std::pair<uint32, uint32> WindowsWindow::GetSize() const {
  return std::make_pair(Data_.Width, Data_.Height);
}
std::pair<int, int> WindowsWindow::GetWindowPos() const {
  int x, y;
  glfwGetWindowPos(Window_, &x, &y);
  return {x, y};
}
void WindowsWindow::Maximize() {
  glfwMaximizeWindow(Window_);
}
void WindowsWindow::SetEventCallback(const std::function<void(class Event &)> &callback) {
  Data_.EventCallback = callback;
}

// Todo: Implement VSync
void WindowsWindow::SetVSync(bool enabled) {
  Data_.VSync = enabled;
}
bool WindowsWindow::IsVSync() const {
  return Data_.VSync;
}
std::string_view WindowsWindow::GetTitle() const {
  return Data_.Title;
}
void WindowsWindow::SetTitle(std::string title) {
  Data_.Title = std::move(title);
  glfwSetWindowTitle(Window_, Data_.Title.c_str());
}
void *WindowsWindow::GetNativeWindow() const {
  return Window_;
}

void WindowsWindow::Init(const WindowProperties &properties) {
  // Create the glfw Window and blah blah blah
  Data_.Title = std::move(properties.Title);
  Data_.Width = properties.Width;
  Data_.Height = properties.Height;

  GLASS_CORE_INFO("Creating window {0} ({1}, {2}", Data_.Title, Data_.Width, Data_.Height);

  // Initialize glfw if needed

  if (!GLFWInitialized) {
    int success = glfwInit();
    if (!success) {
      GLASS_CORE_CRITICAL("Could not init GLFW!");
      GLASSEYE_BREAK();
    }
    glfwSetErrorCallback(GLFWErrorCallback);
    GLFWInitialized = true;
  }

  // Tell GLFW not to use OpenGL context with subsequent calls
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // Create window using given properties
  Window_ = glfwCreateWindow((int) Data_.Width, (int) Data_.Height, Data_.Title.c_str(), nullptr, nullptr);

  // Set window pointer to this WindowsWindow instantiation
  glfwSetWindowUserPointer(Window_, this);

  // Set resize callback
  glfwSetFramebufferSizeCallback(Window_, [](GLFWwindow *window, int width,
                                             int height) {
    auto &data = *(WindowsWindow *) glfwGetWindowUserPointer(window);
    WindowResizeEvent event(width, height);
    data.Data_.EventCallback(event);
    data.Data_.Width = width;
    data.Data_.Height = height;
  });

  // Set window closing callback
  glfwSetWindowCloseCallback(Window_, [](GLFWwindow *window) {
    auto &data = *(WindowsWindow *) glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.Data_.EventCallback(event);
  });

  glfwSetKeyCallback(Window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto &data = ((WindowsWindow *) glfwGetWindowUserPointer(window))->Data_;

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event((KeyCode) key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event((KeyCode) key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event((KeyCode) key, 1);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetCharCallback(Window_, [](GLFWwindow *window, unsigned int codepoint) {
    auto &data = ((WindowsWindow *) glfwGetWindowUserPointer(window))->Data_;

    KeyTypedEvent event((KeyCode) codepoint);
    data.EventCallback(event);
  });

  glfwSetMouseButtonCallback(Window_, [](GLFWwindow *window, int button, int action, int mods) {
    auto &data = ((WindowsWindow *) glfwGetWindowUserPointer(window))->Data_;

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(Window_, [](GLFWwindow *window, double xOffset, double yOffset) {
    auto &data = ((WindowsWindow *) glfwGetWindowUserPointer(window))->Data_;

    MouseScrolledEvent event((float) xOffset, (float) yOffset);
    data.EventCallback(event);
  });

  glfwSetCursorPosCallback(Window_, [](GLFWwindow *window, double x, double y) {
    auto &data = ((WindowsWindow *) glfwGetWindowUserPointer(window))->Data_;
    MouseMovedEvent event((float) x, (float) y);
    data.EventCallback(event);
  });

  VulkanRendererContext_ = MakeRef<VulkanContext>();
  VulkanRendererContext_->Create(Window_);
}
void WindowsWindow::Shutdown() {
  glfwDestroyWindow(Window_);
  glfwTerminate();
  GLFWInitialized = false;
}
GLFWwindow *WindowsWindow::GetGlfwWindow() const { return Window_; }
Ref <VulkanContext> WindowsWindow::GetRenderContext() {
  return VulkanRendererContext_;
}

}