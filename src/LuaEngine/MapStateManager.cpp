#include "MapStateManager.hpp"
#include "EclipseIncludes.hpp"
#include <chrono>

namespace Eclipse
{
    MapStateManager& MapStateManager::GetInstance()
    {
        static MapStateManager instance;
        return instance;
    }

    LuaEngine* MapStateManager::GetStateForMap(int32 mapId)
    {
        auto [it, inserted] = mapStates.try_emplace(mapId, nullptr);
        if (!inserted) return it->second.get();
        
        auto engine = std::make_unique<LuaEngine>();
        if (engine->Initialize(mapId))
        {
            LuaEngine* enginePtr = engine.get();
            it->second = std::move(engine);
            return enginePtr;
        }
        
        mapStates.erase(it);
        LOG_ERROR("server.eclipse", "Failed to create Lua state for map: {}", mapId);
        return nullptr;
    }

    LuaEngine* MapStateManager::GetGlobalState()
    {
        return GetStateForMap(-1);
    }


    void MapStateManager::UnloadMapState(int32 mapId)
    {
        auto it = mapStates.find(mapId);
        if (it != mapStates.end())
        {
            it->second->Shutdown();
            mapStates.erase(it);
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
        LOG_INFO("server.eclipse", "[Eclipse]: Searching scripts from `lua_scripts`");
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        auto* globalEngine = GetGlobalState();
        if (globalEngine) globalEngine->ReloadScripts();
        
        for (auto& [mapId, engine] : mapStates)
        {
            if (mapId != -1) engine->ReloadScripts();
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        LOG_INFO("server.eclipse", "[Eclipse]: All scripts reloaded successfully in {} ms", totalDuration.count());
    }
}