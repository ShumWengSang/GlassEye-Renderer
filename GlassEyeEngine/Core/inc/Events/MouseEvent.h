//
// Created by user on 6/16/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENTS_MOUSEEVENT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENTS_MOUSEEVENT_H_
#include "Event.h"

namespace GlassEye {
class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(float x, float y)
      : MouseX_(x), MouseY_(y) {}

  float GetX() const { return MouseX_; }
  float GetY() const { return MouseY_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << MouseX_ << ", " << MouseY_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
 private:
  float MouseX_, MouseY_;
};

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : XOffset_(xOffset), YOffset_(yOffset) {}

  float GetXOffset() const { return XOffset_; }
  float GetYOffset() const { return YOffset_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
 private:
  float XOffset_, YOffset_;
};

class MouseButtonEvent : public Event {
 public:
  inline int GetMouseButton() const { return Button_; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
 protected:
  MouseButtonEvent(int button)
      : Button_(button) {}

  int Button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(int button)
      : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << Button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(int button)
      : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << Button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENTS_MOUSEEVENT_H_
