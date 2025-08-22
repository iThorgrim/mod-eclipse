#ifndef ECLIPSE_EVENT_DISPATCHER_HPP
#define ECLIPSE_EVENT_DISPATCHER_HPP

#include "EclipseIncludes.hpp"
#include "Events.hpp"
#include <functional>

namespace Eclipse
{
    class LuaEngine; // Forward declaration
    
    class EventDispatcher
    {
    public:
        static EventDispatcher& GetInstance();
        
        // Universal template declaration (implementation in .cpp)
        template<typename... Args>
        void TriggerPlayerEvent(uint32 eventId, Args&&... args);
         
        void TriggerMapEvent(uint32 eventId, Map* map);
        void TriggerMapPlayerEvent(uint32 eventId, Map* map, Player* player);
        
    private:
        EventDispatcher() = default;
        ~EventDispatcher() = default;
        
        std::vector<LuaEngine*> GetRelevantEngines(Player* player);
        std::vector<LuaEngine*> GetRelevantEngines(Map* map);
        
        void TriggerOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Player* player);
        
        // Type-erased callback execution
        void ExecuteOnRelevantEngines(Player* player, std::function<void(LuaEngine*)> callback);
    };
}

#endif // ECLIPSE_EVENT_DISPATCHER_HPP