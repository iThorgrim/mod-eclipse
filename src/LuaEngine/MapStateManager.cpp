#include "MapStateManager.hpp"
#include "Log.h"

namespace Eclipse
{
    MapStateManager& MapStateManager::GetInstance()
    {
        static MapStateManager instance;
        return instance;
    }

    LuaEngine* MapStateManager::GetStateForMap(int32 mapId)
    {
        auto it = mapStates.find(mapId);
        if (it != mapStates.end())
        {
            return it->second.get();
        }
        
        return CreateStateForMap(mapId);
    }

    LuaEngine* MapStateManager::GetGlobalState()
    {
        return GetStateForMap(-1); // -1 = global state
    }

    LuaEngine* MapStateManager::CreateStateForMap(int32 mapId)
    {
        auto engine = std::make_unique<LuaEngine>();
        if (engine->Initialize(mapId))
        {
            LuaEngine* enginePtr = engine.get();
            mapStates[mapId] = std::move(engine);
            
            const char* stateType = (mapId == -1) ? "global" : "map";
            LOG_INFO("server.eclipse", "Created {} Lua state for ID: {}", stateType, mapId);
            
            return enginePtr;
        }
        
        LOG_ERROR("server.eclipse", "Failed to create Lua state for map: {}", mapId);
        return nullptr;
    }

    void MapStateManager::UnloadMapState(int32 mapId)
    {
        auto it = mapStates.find(mapId);
        if (it != mapStates.end())
        {
            it->second->Shutdown();
            mapStates.erase(it);
            LOG_INFO("server.eclipse", "Unloaded Lua state for map: {}", mapId);
        }
    }

    void MapStateManager::UnloadAllStates()
    {
        for (auto& [mapId, engine] : mapStates)
        {
            engine->Shutdown();
        }
        mapStates.clear();
        LOG_INFO("server.eclipse", "Unloaded all Lua states");
    }

    void MapStateManager::ReloadAllScripts()
    {
        LOG_INFO("server.eclipse", "Reloading scripts for all Lua states...");
        for (auto& [mapId, engine] : mapStates)
        {
            engine->ReloadScripts();
        }
        LOG_INFO("server.eclipse", "All scripts reloaded successfully");
    }
}