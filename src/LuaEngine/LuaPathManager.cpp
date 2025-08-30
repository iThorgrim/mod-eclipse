#include "LuaPathManager.hpp"
#include "EclipseIncludes.hpp"

namespace Eclipse
{
    void LuaPathManager::AddSearchPath(const std::string& path)
    {
        try
        {
            if (processedPaths.insert(path).second)
            {
                BuildPaths();
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to add search path '{}': {}", path, e.what());
            throw;
        }
    }

    void LuaPathManager::ApplyPaths(sol::state& lua)
    {
        try
        {
            lua["package"]["path"] = luaRequirePath;
            lua["package"]["cpath"] = luaRequireCPath;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("server.eclipse", "Failed to apply Lua paths: {}", e.what());
            throw;
        }
    }

    void LuaPathManager::Reset()
    {
        processedPaths.clear();
        luaRequirePath.clear();
        luaRequireCPath.clear();
    }

    bool LuaPathManager::HasPath(const std::string& path) const
    {
        return processedPaths.find(path) != processedPaths.end();
    }

    void LuaPathManager::BuildPaths()
    {
        luaRequirePath.clear();
        luaRequireCPath.clear();

        for (const auto& path : processedPaths)
        {
            luaRequirePath +=
                path + "/?.ext;" +
                path + "/?.lua;" +
                path + "/?.out;" +
                path + "/?.moon;";
            
            luaRequireCPath +=
                path + "/?.dll;" +
                path + "/?.so;";
        }
    }
}