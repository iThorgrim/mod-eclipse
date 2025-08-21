#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "sol.hpp"
#include <unordered_map>
#include <string>
#include <chrono>

namespace Eclipse
{
    class LuaCache
    {
    public:
        LuaCache() = default;
        ~LuaCache() = default;
        
        // Function caching
        sol::function& GetFunction(const std::string& name, sol::state& lua);
        void ClearFunctions();
        size_t GetFunctionCacheSize() const;
        
        // Script timestamp tracking
        void SetScriptTimestamp(const std::string& path);
        bool IsScriptModified(const std::string& path) const;
        void ClearTimestamps();
        
        // Full cache clear
        void Clear();
        
    private:
        std::unordered_map<std::string, sol::function> functionCache;
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> scriptTimestamps;
    };
}

#endif // ECLIPSE_LUA_CACHE_HPP