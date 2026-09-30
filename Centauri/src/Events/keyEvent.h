#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "event.h"

namespace Centauri {
    class KeyPressEvent: public Event {
      public:
        KeyPressEvent(int key, bool isRepeat) : keyCode(key), repeat(isRepeat) {} 

        int GetKeyCode() const {return keyCode;}
        bool IsRepeat() const {return repeat;}
        virtual int GetCategoryFlags() const override {return Centauri::EventCategory::Keyboard | Centauri::EventCategory::Input}

        DEFINE_EVENT_TYPE(KeyPressEvent);
    
      private:
        int keyCode;
        bool repeat;
    };

    class KeyReleaseEvent: public Event {
      public: 
        KeyReleaseEvent(int key) : keyCode(key) {}

        int GetKeyCode() const {return keyCode;}
        virtual int GetCategoryFlags() const override {return Centauri::EventCategory::Keyboard | Centauri::EventCategory::Input}

        DEFINE_EVENT_TYPE(KeyReleaseEvent);
      private: 
        keyCode;
    };
}

#endif KEY_EVENT_H