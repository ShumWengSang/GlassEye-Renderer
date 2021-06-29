//
// Created by user on 6/15/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Window.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/15/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Window.h"

GlassEye::WindowProperties::WindowProperties(std::string title,
                                             uint32 width,
                                             uint32 height,
                                             bool fullscreen)
    : Title(title), Width(width), Height(height), Fullscreen(fullscreen) {}

GlassEye::Window::~Window() {

}