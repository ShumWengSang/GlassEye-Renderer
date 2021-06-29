//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Event.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/
#include "Event.h"

std::string GlassEye::Event::ToString() const {
  return GetName();
}
bool GlassEye::Event::IsInCategory(GlassEye::EventCategory cat) {
  return GetCategoryFlags() & cat;
}
GlassEye::EventDispatcher::EventDispatcher(GlassEye::Event &event) : event_(event) {

}
std::ostream &GlassEye::operator<<(std::ostream &os, const GlassEye::Event &e) {
  return os << e.ToString();
}
