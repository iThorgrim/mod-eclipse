#ifndef ECLIPSE_EVENT_DISPATCHER_HPP
#define ECLIPSE_EVENT_DISPATCHER_HPP

#include "EclipseIncludes.hpp"
#include "Events.hpp"
#include <functional>

namespace Eclipse
{
    class LuaEngine;
    
    class EventDispatcher
    {
    public:
        static EventDispatcher& GetInstance();
        
        template<typename... Args>
        void TriggerEvent(uint32 eventId, Args&&... args);
        
        template<typename... Args>
        void TriggerKeyedEvent(uint32 eventId, Args&&... args);
        
    private:
        EventDispatcher() = default;
        ~EventDispatcher() = default;
        
        template<typename T>
        std::vector<LuaEngine*> GetRelevantEngines(T* object);
        
        template<typename T, typename... Args>
        void TriggerOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Args&&... args);
        
        template<EventType Type, typename... Args>
        void TriggerKeyedEventHelper(const std::vector<LuaEngine*>& engines, uint32 objectId, uint32 eventId, Args&&... args);
    };
}

#endif // ECLIPSE_EVENT_DISPATCHER_HPP