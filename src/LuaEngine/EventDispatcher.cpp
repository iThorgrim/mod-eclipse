#include "EventDispatcher.hpp"
#include "MapStateManager.hpp"
#include "LuaEngine.hpp"
#include "EventManager.hpp"
#include <unordered_set>
#include <tuple>
#include <type_traits>

namespace Eclipse
{
    EventDispatcher& EventDispatcher::GetInstance()
    {
        static EventDispatcher instance;
        return instance;
    }
    
    template<typename... Args>
    void EventDispatcher::TriggerPlayerEvent(uint32 eventId, Args&&... args)
    {
        static_assert(sizeof...(args) > 0, "At least one argument (Player*) required");
        
        auto firstArg = std::get<0>(std::forward_as_tuple(args...));
        Player* player = nullptr;
        
        if constexpr (std::is_same_v<std::decay_t<decltype(firstArg)>, Player*>) {
            player = firstArg;
        }
        
        if (!player) return;
        
        auto engines = GetRelevantEngines(player);
        for (auto* engine : engines)
        {
            if (engine && engine->GetEventManager())
            {
                engine->GetEventManager()->TriggerPlayerEvent(eventId, std::forward<Args>(args)...);
            }
        }
    }
    
    void EventDispatcher::ExecuteOnRelevantEngines(Player* player, std::function<void(LuaEngine*)> callback)
    {
        auto engines = GetRelevantEngines(player);
        for (auto* engine : engines)
        {
            if (engine)
            {
                callback(engine);
            }
        }
    }
    
    void EventDispatcher::TriggerMapEvent(uint32 /*eventId*/, Map* /*map*/)
    {
    }
    
    void EventDispatcher::TriggerMapPlayerEvent(uint32 eventId, Map* map, Player* player)
    {
        auto engines = GetRelevantEngines(map);
        TriggerOnEngines(engines, eventId, player);
    }
    
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(Player* player)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(2);
        
        auto& manager = MapStateManager::GetInstance();
        
        // Always include global state (-1)
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.push_back(globalEngine);
        }
        
        if (player)
        {
            Map* playerMap = player->GetMap();
            if (playerMap)
            {
                uint32 mapId = playerMap->GetId();
                auto* mapEngine = manager.GetStateForMap(mapId);
                
                if (mapEngine && mapEngine != globalEngine)
                {
                    engines.push_back(mapEngine);
                }
            }
        }
        
        return engines;
    }
    
    std::vector<LuaEngine*> EventDispatcher::GetRelevantEngines(Map* map)
    {
        std::vector<LuaEngine*> engines;
        engines.reserve(2);
        
        auto& manager = MapStateManager::GetInstance();
        
        auto* globalEngine = manager.GetGlobalState();
        if (globalEngine)
        {
            engines.push_back(globalEngine);
        }
        
        if (map)
        {
            uint32 mapId = map->GetId();
            auto* mapEngine = manager.GetStateForMap(mapId);
            
            if (mapEngine && mapEngine != globalEngine)
            {
                engines.push_back(mapEngine);
            }
        }
        
        return engines;
    }
    
    void EventDispatcher::TriggerOnEngines(const std::vector<LuaEngine*>& engines, uint32 eventId, Player* player)
    {
        for (auto* engine : engines)
        {
            if (engine && engine->GetEventManager())
            {
                engine->GetEventManager()->TriggerPlayerEvent(eventId, player);
            }
        }
    }
}

// Explicit template instantiations for types used in EclipseScript.cpp
template void Eclipse::EventDispatcher::TriggerPlayerEvent<Player*&>(uint32, Player*&);
template void Eclipse::EventDispatcher::TriggerPlayerEvent<Player*&, Item*&, uint32&, ObjectGuid&>(uint32, Player*&, Item*&, uint32&, ObjectGuid&);
template void Eclipse::EventDispatcher::TriggerPlayerEvent<Player*&, uint8&>(uint32, Player*&, uint8&);
template void Eclipse::EventDispatcher::TriggerPlayerEvent<Player*&, Player*&>(uint32, Player*&, Player*&);
