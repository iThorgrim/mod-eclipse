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

        void AddSearchPath(const std::string& path);
        void ApplyPaths(sol::state& lua);
        void Reset();
        bool HasPath(const std::string& path) const;

    private:
        LuaPathManager() = default;
        ~LuaPathManager() = default;
        LuaPathManager(const LuaPathManager&) = delete;
        LuaPathManager& operator=(const LuaPathManager&) = delete;

        std::set<std::string> processedPaths;
        std::string luaRequirePath;
        std::string luaRequireCPath;

        void BuildPaths();
    };
}

#endif // ECLIPSE_LUA_PATH_MANAGER_HPP