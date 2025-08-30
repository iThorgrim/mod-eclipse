#include "LuaCompiler.hpp"
#include <filesystem>

namespace Eclipse
{
    LuaCompiler::LuaCompiler() : cache()
    {
    }

    bool LuaCompiler::CompileAndExecute(sol::state& luaState, const std::string& scriptPath)
    {
        try
        {
            return CompileFromFile(luaState, scriptPath);
        }
        catch (const std::exception& e)
        {
            HandleCompilationError(scriptPath, e.what());
            return false;
        }
    }

    bool LuaCompiler::ExecuteScript(sol::state& luaState, const std::string& script)
    {
        try
        {
            return CompileFromString(luaState, script);
        }
        catch (const std::exception& e)
        {
            HandleCompilationError("inline_script", e.what());
            return false;
        }
    }

    bool LuaCompiler::CompileFromFile(sol::state& luaState, const std::string& scriptPath)
    {
        if (!std::filesystem::exists(scriptPath))
        {
            LOG_ERROR("server.eclipse", "Script file not found: {}", scriptPath);
            return false;
        }

        auto cacheResult = cache.LoadScript(luaState, scriptPath);
        if (cacheResult.has_value())
        {
            if (cacheResult.value())
            {
                LOG_TRACE("server.eclipse", "Loaded script from optimized cache: {}", scriptPath);
                return true;
            }
            else
            {
                LOG_TRACE("server.eclipse", "Script compilation failed (cached): {}", scriptPath);
                return false;
            }
        }

        // This should not happen as LoadScript handles compilation internally
        LOG_TRACE("server.eclipse", "Cache miss handled by LoadScript: {}", scriptPath);
        return false;
    }

    bool LuaCompiler::CompileFromString(sol::state& luaState, const std::string& script, const std::string& name)
    {
        try
        {
            auto result = luaState.script(script, name);
            LOG_TRACE("server.eclipse", "Compiled and executed script: {}", name);
            return true;
        }
        catch (const sol::error& e)
        {
            HandleCompilationError(name, e.what());
            return false;
        }
    }

    void LuaCompiler::ClearCache()
    {
        cache.Clear();
        LOG_DEBUG("server.eclipse", "LuaCompiler cache cleared");
    }

    void LuaCompiler::HandleCompilationError(const std::string& script, const std::string& error)
    {
        LOG_ERROR("server.eclipse", "Lua compilation error in '{}': {}", script, error);
    }
}