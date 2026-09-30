#ifndef EVENT_SYSYEM_H
#define EVENT_SYSYEM_H

#include "event.h"
#include <vector>
#include <queue> 

namespace Centauri {
    class EventSystem {
      public:

        void AddListener(EventListner* listener);
        void RemoveListener(EventListerner* listener);
        void PublishEvent(const Event& e);

          //ADD QUEUED MODE

      private:
        struct ListenerInfo {
          EventListener* listener;
          int categoryFilter;
        }

        std::vector<ListenerInfo> listeners;
        //std::queue<std::unique_ptr<Event>> eventQueue;
        //std::mutex queueMutex;
        //bool inmediate mode = true;
      
      }
}

#endif //EVENT_SYSYEM_H 