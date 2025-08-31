#ifndef ECLIPSE_LUA_ENGINE_HPP
#define ECLIPSE_LUA_ENGINE_HPP

#include "LuaState.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Eclipse { struct LoadStatistics; }

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
        
        // Access to global compiler state (state -1)
        static sol::state& GetGlobalCompilerState();
        
        sol::state& GetState() { return luaState.GetState(); }
        class EventManager* GetEventManager() const noexcept { return eventManager.get(); }
        
        void ProcessMessages();


    private:
        LuaState luaState;
        bool isInitialized;
        std::vector<std::string> loadedScripts;
        std::string scriptsDirectory;
        int32 stateMapId; // -1 = global/world state, >=0 = specific map
        std::unique_ptr<class EventManager> eventManager;
        
        void RegisterBindings();
        void ShutdownComponents();
        void ClearStateData();
        void LoadScriptsForState();
        bool LoadCachedScriptsFromGlobalState();
        
        // Helper methods
        static std::pair<uint32, std::string> FormatDuration(uint32 microseconds);
        void LogScriptLoadStats(const LoadStatistics& stats) const;
        void LogCacheLoadStats(size_t scriptCount, uint32 durationUs) const;
    };
}

#endif // ECLIPSE_LUA_ENGINE_HPP