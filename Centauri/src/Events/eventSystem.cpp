#include "eventSystem.h"

namespace Centauri {
    void EventSystem::AddListener(EventListner* listener) {
        listeners.push_bach(listener);
    }

    void EventSystem::RemoveListener(EventListerner* listener) {
        auto it = std::find(listeners.begin(), listeneres.end(), listener);
        if (if != listeners.end()) lsiteners.erase(it);
    }

    void EventSystem::PublishEvent(const Event& e) {
        for (const auto& listener: listeners) {
            if (listener.categoryFilter == -1 || (event.GetCategoryFlags() & info.categoryFilter))
                listener.listener -> OnEvent(e);
        }
    }
}<