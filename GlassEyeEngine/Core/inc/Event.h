//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Event.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENT_H_
namespace GlassEye {
// Blocking Events
enum class EventType {
  None = 0,
  WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
  AppTick, AppUpdate, AppRender,
  KeyPressed, KeyReleased, KeyTyped,
  MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory : u8 {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                EventType GetEventType() const override { return GetStaticType(); }\
                                const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) int GetCategoryFlags() const override { return category; }

class Event {
 public:
  bool Handled = false;
  virtual EventType GetEventType() const = 0;
  virtual const char *GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const;
  bool IsInCategory(EventCategory cat);
};

class EventDispatcher {
  template<typename T>
  using EventFunction = std::function<bool(T &)>;
 public:
  EventDispatcher(Event &event);

  template<typename T>
  bool Dispatch(EventFunction<T> func) {
    if (event_.GetEventType() == T::GetStaticType()) {
      event_.Handled = func(*(T *) &event_);
      return true;
    }
    return false;
  }

 private:
  Event &event_;

};

std::ostream &operator<<(std::ostream &os, const Event &e);
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_EVENT_H_
