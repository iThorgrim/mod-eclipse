#ifndef ECLIPSE_EVENT_MANAGER_HPP
#define ECLIPSE_EVENT_MANAGER_HPP

#include "sol.hpp"
#include "Events.hpp"
#include <unordered_map>
#include <vector>

namespace Eclipse
{
    class EventManager
    {
    public:
        static EventManager& GetInstance();
        
        // Event registration
        void RegisterPlayerEvent(uint32 eventId, sol::function callback);
        
        // Event triggering
        template<typename... Args>
        void TriggerPlayerEvent(uint32 eventId, Args&&... args);
        
        // Cleanup
        void ClearAllEvents();
        void ClearPlayerEvents();
        
        // Lua bindings registration
        static void Register(sol::state& lua);
        
    private:
        EventManager() = default;
        ~EventManager() = default;
        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        
        std::unordered_map<uint32, std::vector<sol::function>> playerEvents;
    };

    // Template implementation
    template<typename... Args>
    void EventManager::TriggerPlayerEvent(uint32 eventId, Args&&... args)
    {
        auto it = playerEvents.find(eventId);
        if (it != playerEvents.end())
        {
            for (auto& callback : it->second)
            {
                if (callback.valid())
                {
                    try
                    {
                        callback(std::forward<Args>(args)...);
                    }
                    catch (const std::exception&)
                    {
                        // Log error but continue with other callbacks
                    }
                }
            }
        }
    }
}

#endif // ECLIPSE_EVENT_MANAGER_HPP