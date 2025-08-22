#ifndef ECLIPSE_MAP_STATE_MANAGER_HPP
#define ECLIPSE_MAP_STATE_MANAGER_HPP

#include "EclipseIncludes.hpp"
#include "LuaEngine.hpp"

#include <unordered_map>
#include <memory>

namespace Eclipse
{
    class MapStateManager
    {
    public:
        static MapStateManager& GetInstance();
        
        LuaEngine* GetStateForMap(int32 mapId);
        LuaEngine* GetGlobalState();
        
        void UnloadMapState(int32 mapId);
        void UnloadAllStates();
        void ReloadAllScripts();
        
    private:
        MapStateManager() = default;
        ~MapStateManager() = default;
        MapStateManager(const MapStateManager&) = delete;
        MapStateManager& operator=(const MapStateManager&) = delete;
        
        std::unordered_map<int32, std::unique_ptr<LuaEngine>> mapStates;
    };
}

#endif // ECLIPSE_MAP_STATE_MANAGER_HPP