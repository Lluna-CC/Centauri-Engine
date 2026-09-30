#ifndef EVENT_H
#define EVENT_H

//This file defines multiple virtual classes regarding events

namespace Centauri {
    enum class EventCategory {
        None = 0,
        Application = 1 << 0,
        Input = 1 << 1,
        Keyboard = 1 << 2,
        Mouse = 1 << 3,
        MouseButton = 1 << 4,
        Window = 1 << 5
    };


    class Event {
    
      public:
        virtual ~Event() = default;

        virtual const char* GetType() const = 0;
        virtual Event* Clone() const = 0;

        virtual int GetCategoryFlags() const = 0;
        bool IsInCategory(EventCategory category) const {
            return GetCategoryFlags() & static_cast<int>(category);
        }
      
      protected:
        bool m_Handled = false; 
    
    
    };

    #define DEFINE_EVENT_TYPE(type) \
    static const char* GetStaticType() { return #type; } \
    virtual const char* GetType() const override { return GetStaticType(); } \
    virtual Event* Clone() const override { return new type(*this); }


    class EventListener {
      public:
        virtual ~EventListener() = default;
        virtual void OnEvent(const Event& e) = 0;
    };

    class EventDispatcher {
      private: 
        const Event& event;
      public: 
        explicit EventDispatcher(const Event& e) : event(e) {}

        template<typename T, typename F>
        bool Dispatch(const F& handler) {
            if (event.GetType() == T::GetStaticType()) {
                handler(static_cast<const T&>(event));
            }
            return false;
        }
    };
}

#endif //EVENT_H