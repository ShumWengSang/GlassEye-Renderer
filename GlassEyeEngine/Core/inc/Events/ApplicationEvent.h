//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: ApplicationEvent.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_APPLICATIONEVENT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_APPLICATIONEVENT_H_
#include "Event.h"
namespace GlassEye {
class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height) : Width_(width), Height_(height) {}

  unsigned int GetWidth() const { return Width_; }
  unsigned int GetHeight() const { return Height_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << Width_ << ", " << Height_;
    return ss.str();
  }
  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
 private:
  unsigned int Width_, Height_;
};

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_APPLICATIONEVENT_H_
