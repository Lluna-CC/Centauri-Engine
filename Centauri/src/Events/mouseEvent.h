#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

namespace Centauri {
    class MouseMovedEvent: public Event {
      public:    
        MouseMovedEvent(float x_pos, float y_pos) : x(x_pos), y(y_pos) {}

        float GetXPosition() {return x;}
        float GetYPosition() {return y;}

        virtual int GetCategoryFlags() const override {
          return static_cast<int>(Centauri::EventCategory::Mouse) | static_cast<int>(Centauri::EventCategory::Input);
        }
        DEFINE_EVENT_TYPE(MouseMovedEvent);

      private:
        float x,y;
    };

    class MouseButtonPressEvent: public Event {
      public:
        MouseButtonPressEvent(int button) : buttonCode(button) {}

        int GetButton() {return buttonCode;}

        virtual int GetCategoryFlags() const override {
          return static_cast<int>(Centauri::EventCategory::Mouse) | static_cast<int>(Centauri::EventCategory::Input);
        }

        DEFINE_EVENT_TYPE(MouseButtonPressEvent);

      private:
        int buttonCode;
        
    };

    class MouseButtonReleasedEvent: public Event {
      public:
        MouseButtonReleasedEvent(int button) : buttonCode(button) {}

        int GetButton() {return buttonCode;}

        virtual int GetCategoryFlags() const override {
          return static_cast<int>(Centauri::EventCategory::Mouse) | static_cast<int>(Centauri::EventCategory::Input);
        }

        DEFINE_EVENT_TYPE(MouseButtonReleasedEvent);

      private:
        int buttonCode;
        
    };
}

#endif //MOUSE_EVENT_H