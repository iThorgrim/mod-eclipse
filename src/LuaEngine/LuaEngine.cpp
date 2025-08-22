#include "LuaEngine.hpp"
#include "EclipseIncludes.hpp"
#include "EventManager.hpp"
#include "LuaBindings.hpp"
#include "MessageManager.hpp"
#include "ScriptLoader.hpp"

namespace Eclipse
{
    LuaEngine::LuaEngine() : isInitialized(false), stateMapId(-1), scriptsDirectory("lua_scripts"), eventManager(std::make_unique<EventManager>())
    {
    }

    LuaEngine::~LuaEngine()
    {
        Shutdown();
    }

    bool LuaEngine::Initialize(int32 mapId)
    {
        if (isInitialized)
            return true;

        stateMapId = mapId;

        try
        {
            InitializeState();
            ConfigureOptimizations();
            RegisterBindings();
            
            isInitialized = true;
            
            LoadDirectory(scriptsDirectory);
            
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to initialize Eclipse Lua Engine: {}", e.what());
            return false;
        }
    }

    void LuaEngine::Shutdown()
    {
        if (isInitialized)
        {
            // Clean up message handlers for this state
            MessageManager::GetInstance().ClearStateHandlers(stateMapId);
            
            loadedScripts.clear();
            luaState = sol::state();
            isInitialized = false;
        }
    }

    bool LuaEngine::LoadScript(const std::string& scriptPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot load script: Engine not initialized");
            return false;
        }

        return ScriptLoader::LoadFile(luaState, scriptPath);
    }

    bool LuaEngine::ExecuteScript(const std::string& script)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot execute script: Engine not initialized");
            return false;
        }

        try
        {
            luaState.script(script);
            return true;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to execute script: {}", e.what());
            return false;
        }
    }

    void LuaEngine::RegisterBindings()
    {
        LuaBindings::Register(luaState, eventManager.get());
        
        luaState["GetStateMapId"] = [this]() -> int32 {
            return this->stateMapId;
        };
        
        MessageManager::GetInstance().RegisterBindings(luaState, stateMapId);
    }

    bool LuaEngine::LoadDirectory(const std::string& directoryPath)
    {
        if (!isInitialized)
        {
            LOG_ERROR("server.eclipse", "Cannot load scripts: Engine not initialized");
            return false;
        }

        luaState["_eclipse_stats"] = sol::lua_nil;
        bool success = ScriptLoader::LoadDirectory(luaState, directoryPath, loadedScripts);
        
        if (sol::optional<sol::table> stats = luaState["_eclipse_stats"])
        {
            const char* mapType = (stateMapId == -1) ? "global" : "map";
            LOG_INFO("server.eclipse", "[Eclipse]: {} scripts loaded for {} state {}",
                stats->get_or("loaded", 0), mapType, stateMapId);
        }
        
        luaState["_eclipse_stats"] = sol::lua_nil;
        
        return success;
    }

    void LuaEngine::ReloadScripts()
    {
        if (!isInitialized)
            return;
    
        if (eventManager) eventManager->ClearAllEvents();
        loadedScripts.clear();

        MessageManager::GetInstance().ClearStateHandlers(stateMapId);
        InitializeState();
        ConfigureOptimizations();
        RegisterBindings();
        
        LoadDirectory(scriptsDirectory);
    }

    void LuaEngine::ProcessMessages()
    {
        if (isInitialized)
        {
            MessageManager::GetInstance().ProcessMessages(stateMapId);
        }
    }

    void LuaEngine::InitializeState()
    {
        luaState = sol::state();
        luaState.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            sol::lib::debug,
            sol::lib::bit32,
            sol::lib::utf8
        );
        
#ifdef LUAJIT_VERSION
        try {
            lua_State* L = luaState.lua_state();
            lua_getglobal(L, "require");
            lua_pushstring(L, "jit");
            if (lua_pcall(L, 1, 1, 0) == 0) {
                lua_setglobal(L, "jit");
                luaState.script("if jit and jit.opt then jit.opt.start(3) end");
            } else {
                lua_pop(L, 1);
            }
        } catch (...) {}
#endif
    }

    void LuaEngine::ConfigureOptimizations()
    {
        luaState["_ECLIPSE_OPTIMIZED"] = true;
    }
}