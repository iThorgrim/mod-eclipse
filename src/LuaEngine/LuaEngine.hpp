#ifndef ECLIPSE_LUA_ENGINE_HPP
#define ECLIPSE_LUA_ENGINE_HPP

#include "sol.hpp"
#include "LuaCache.hpp"
#include "Common.h"

#include <string>
#include <memory>
#include <vector>

namespace Eclipse
{
    class LuaEngine
    {
    public:
        LuaEngine();
        ~LuaEngine();

        bool Initialize(int32 mapId = -1);
        void Shutdown();
        void ReloadScripts();
        
        int32 GetStateMapId() const { return stateMapId; }
        
        bool LoadScript(const std::string& scriptPath);
        bool ExecuteScript(const std::string& script);
        bool LoadDirectory(const std::string& directoryPath);
        
        sol::state& GetState() { return luaState; }
        class EventManager* GetEventManager() const { return eventManager.get(); }
        
        void ProcessMessages(); // Process pending inter-state messages

    private:
        sol::state luaState;
        bool isInitialized;
        std::vector<std::string> loadedScripts;
        std::string scriptsDirectory;
        LuaCache cache;
        int32 stateMapId; // -1 = global/world state, >=0 = specific map
        std::unique_ptr<class EventManager> eventManager;
        
        void InitializeState();
        void RegisterBindings();
        void ConfigureOptimizations();
    };
}

#endif // ECLIPSE_LUA_ENGINE_HPP