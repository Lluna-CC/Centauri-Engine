#include "eventSystem.h"

namespace Centauri {
    void EventSystem::AddListener(EventListener* listener, int categoryFilter) {
        listeners.push_back({listener, categoryFilter});
    }

    void EventSystem::RemoveListener(EventListener* listener) {
        auto it = std::ranges::find_if(listeners.begin(), listeners.end(), [listener](const ListenerInfo& info) {
                                                                    return info.listener == listener;
                                                                            });
        if (it != listeners.end()) listeners.erase(it);
    }

    void EventSystem::PublishEvent(const Event& e) {
        for (const auto& listener: listeners) {
            if (listener.categoryFilter == -1 || (e.GetCategoryFlags() & listener.categoryFilter))
                listener.listener -> OnEvent(e);
        }
    }
}