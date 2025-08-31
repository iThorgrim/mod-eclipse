#ifndef ECLIPSE_LUA_PATH_MANAGER_HPP
#define ECLIPSE_LUA_PATH_MANAGER_HPP

#include "EclipseIncludes.hpp"
#include <string>
#include <set>
#include <vector>

namespace Eclipse
{
    class LuaPathManager
    {
    public:
        static LuaPathManager& GetInstance()
        {
            static LuaPathManager instance;
            return instance;
        }

        void InitializeDefaultPaths();
        void AddSearchPath(const std::string& path);
        void ApplyPaths(sol::state& lua);
        void Reset();
        bool HasPath(const std::string& path) const;
        
        // Get the computed paths without applying them
        const std::string& GetLuaPath() const { return luaRequirePath; }
        const std::string& GetLuaCPath() const { return luaRequireCPath; }

    private:
        LuaPathManager() = default;
        ~LuaPathManager() = default;
        LuaPathManager(const LuaPathManager&) = delete;
        LuaPathManager& operator=(const LuaPathManager&) = delete;

        std::set<std::string> processedPaths;
        std::string luaRequirePath;
        std::string luaRequireCPath;
        std::string customRequirePath;
        std::string customRequireCPath;
        bool initialized = false;

        void BuildPaths();
        void DiscoverLuaScriptDirectories();
        void AddConfigPaths();
    };
}

#endif // ECLIPSE_LUA_PATH_MANAGER_HPP