#ifndef ECLIPSE_EVENT_MANAGER_HPP
#define ECLIPSE_EVENT_MANAGER_HPP

#include "Events.hpp"
#include "ObjectPools.hpp"
#include <unordered_map>
#include <vector>

namespace Eclipse
{
    class EventManager
    {
    public:
        EventManager() = default;
        ~EventManager() = default;
        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        
        template<EventType Type>
        void RegisterEvent(uint32 eventId, sol::function callback);
        
        template<typename... Args>
        void TriggerEvent(uint32 eventId, Args&&... args);
        
        template<EventType Type>
        void ClearEvents();
        
        template<EventType Type>
        void RegisterKeyedEvent(uint32 objectId, uint32 eventId, sol::function callback);
        
        template<EventType Type, typename... Args>
        void TriggerKeyedEvent(uint32 objectId, uint32 eventId, Args&&... args);
        
        template<EventType Type>
        void ClearKeyedEvents();
        
        template<EventType Type>
        bool HasKeyedEvents(uint32 objectId) const;
        
        void ClearAllEvents();
        void Register(sol::state& lua);
        
    private:
        std::unordered_map<EventType, std::unordered_map<uint32, std::vector<sol::function>>> events;
        std::unordered_map<EventType, std::unordered_map<uint32, std::unordered_map<uint32, std::vector<sol::function>>>> keyedEvents;
        
        template<typename CallbackContainer>
        PooledVector<sol::function> CollectCallbacks(const CallbackContainer& container) const;
        
        template<EventType Type>
        auto& GetEventContainer();
        
        template<EventType Type>
        auto& GetKeyedEventContainer();
    };

    // Template implementation
    template<EventType Type>
    auto& EventManager::GetEventContainer()
    {
        return events[Type];
    }

    template<EventType Type>
    void EventManager::RegisterEvent(uint32 eventId, sol::function callback)
    {
        if (callback.valid())
        {
            auto& eventContainer = GetEventContainer<Type>();
            auto& eventList = eventContainer[eventId];
            if (eventList.empty())
                eventList.reserve(4);
            eventList.push_back(callback);
        }
    }

    template<typename... Args>
    void EventManager::TriggerEvent(uint32 eventId, Args&&... args)
    {
        static_assert(sizeof...(args) > 0, "At least one argument required");
        
        constexpr auto eventType = get_event_type<std::tuple_element_t<0, std::tuple<Args...>>>();
        auto& eventContainer = GetEventContainer<eventType>();
        
        auto it = eventContainer.find(eventId);
        if (it != eventContainer.end())
        {
            for (auto& callback : it->second)
            {
                if (callback.valid())
                {
                    try
                    {
                        callback(std::forward<Args>(args)...);
                    }
                    catch (const std::exception&) {}
                }
            }
        }
    }

    template<EventType Type>
    void EventManager::ClearEvents()
    {
        GetEventContainer<Type>().clear();
    }

    template<EventType Type>
    auto& EventManager::GetKeyedEventContainer()
    {
        return keyedEvents[Type];
    }

    template<EventType Type>
    void EventManager::RegisterKeyedEvent(uint32 objectId, uint32 eventId, sol::function callback)
    {
        if (callback.valid())
        {
            auto& eventTypeContainer = GetKeyedEventContainer<Type>();
            auto& objectEventMap = eventTypeContainer[objectId];
            auto& eventList = objectEventMap[eventId];
            if (eventList.empty())
                eventList.reserve(4);
            eventList.push_back(callback);
        }
    }

    template<EventType Type, typename... Args>
    void EventManager::TriggerKeyedEvent(uint32 objectId, uint32 eventId, Args&&... args)
    {
        auto& eventTypeContainer = GetKeyedEventContainer<Type>();
        auto objectIt = eventTypeContainer.find(objectId);
        if (objectIt != eventTypeContainer.end())
        {
            auto eventIt = objectIt->second.find(eventId);
            if (eventIt != objectIt->second.end())
            {
                for (auto& callback : eventIt->second)
                {
                    if (callback.valid())
                    {
                        try
                        {
                            callback(std::forward<Args>(args)...);
                        }
                        catch (const std::exception&) {}
                    }
                }
            }
        }
    }

    template<EventType Type>
    void EventManager::ClearKeyedEvents()
    {
        GetKeyedEventContainer<Type>().clear();
    }

    template<EventType Type>
    bool EventManager::HasKeyedEvents(uint32 objectId) const
    {
        const auto& eventTypeContainer = keyedEvents.find(Type);
        if (eventTypeContainer != keyedEvents.end())
        {
            const auto& objectEvents = eventTypeContainer->second.find(objectId);
            if (objectEvents != eventTypeContainer->second.end())
            {
                // Check if there are any actual callbacks registered
                for (const auto& eventPair : objectEvents->second)
                {
                    if (!eventPair.second.empty())
                        return true;
                }
            }
        }
        return false;
    }

}

#endif // ECLIPSE_EVENT_MANAGER_HPP