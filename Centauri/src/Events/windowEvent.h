#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

namespace Centauri {
    class WindowResizedEvent: public Event {
      public:
        WindowResizedEvent(int w, int h) : width(w), height(h) {}

        int GetWidth() {return width;}
        int GetHeight() {return height;}

        virtual int GetCategoryFlags() const override {return Centauri::EventCategory::Window}
        DEFINE_EVENT_TYPE(WindowResizedEvent);

      private:
        int width, height;
    }

    class WindowClosedEvent: public Event {
      public:
        WindowResizedEvent() {}

        virtual int GetCategoryFlags() const override {return Centauri::EventCategory::Window}
        DEFINE_EVENT_TYPE(WindowClosedEvent);

      private:
        
    }
}

#endif //WINDOW_EVENT_H