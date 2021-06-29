//
// Created by user on 6/16/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_KEYEVENT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_KEYEVENT_H_
#include "Event.h"
#include "KeyCodes.h"

namespace GlassEye {
class KeyEvent : public Event {
 public:
  inline KeyCode GetKeyCode() const { return KeyCode_; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
 protected:
  explicit KeyEvent(KeyCode keycode)
      : KeyCode_(keycode) {}

  KeyCode KeyCode_;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(KeyCode keycode, int repeatCount)
      : KeyEvent(keycode), RepeatCount_(repeatCount) {}

  inline int GetRepeatCount() const { return RepeatCount_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << KeyCode_ << " (" << RepeatCount_ << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)
 private:
  int RepeatCount_;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(KeyCode keycode)
      : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << KeyCode_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
 public:
  KeyTypedEvent(KeyCode keycode)
      : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << KeyCode_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_KEYEVENT_H_
