#ifndef ECLIPSE_LUA_ENGINE_HPP
#define ECLIPSE_LUA_ENGINE_HPP

#include "sol.hpp"
#include "LuaCache.hpp"
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

        bool Initialize();
        void Shutdown();
        
        // Script management
        bool LoadScript(const std::string& scriptPath);
        bool ExecuteScript(const std::string& script);
        bool LoadDirectory(const std::string& directoryPath);
        void ReloadScripts();
        
        // Function execution
        template<typename... Args>
        auto CallFunction(const std::string& functionName, Args&&... args) -> decltype(auto)
        {
            return cache.GetFunction(functionName, luaState)(std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        bool TryCallFunction(const std::string& functionName, Args&&... args)
        {
            try 
            {
                CallFunction(functionName, std::forward<Args>(args)...);
                return true;
            }
            catch (const std::exception&)
            {
                return false;
            }
        }
        
        // Direct state access
        sol::state& GetState() { return luaState; }

    private:
        sol::state luaState;
        bool isInitialized;
        std::vector<std::string> loadedScripts;
        std::string scriptsDirectory;
        LuaCache cache;
        
        void InitializeState();
        void RegisterBindings();
        void ConfigureOptimizations();
    };
}

#endif // ECLIPSE_LUA_ENGINE_HPP