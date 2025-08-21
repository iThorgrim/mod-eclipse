#include "LuaCache.hpp"

namespace Eclipse
{
    sol::function& LuaCache::GetFunction(const std::string& name, sol::state& lua)
    {
        auto it = functionCache.find(name);
        if (it != functionCache.end())
        {
            return it->second;
        }
        
        // Cache miss - get function and store it
        sol::function func = lua[name];
        if (func.valid())
        {
            functionCache[name] = func;
            return functionCache[name];
        }
        
        // Return invalid function if not found
        static sol::function invalidFunc;
        return invalidFunc;
    }
    
    void LuaCache::ClearFunctions()
    {
        functionCache.clear();
    }
    
    size_t LuaCache::GetFunctionCacheSize() const
    {
        return functionCache.size();
    }
    
    void LuaCache::SetScriptTimestamp(const std::string& path)
    {
        scriptTimestamps[path] = std::chrono::steady_clock::now();
    }
    
    bool LuaCache::IsScriptModified(const std::string& path) const
    {
        // For now, always return false (no file watching implemented)
        // Future: implement file modification time checking
        return false;
    }
    
    void LuaCache::ClearTimestamps()
    {
        scriptTimestamps.clear();
    }
    
    void LuaCache::Clear()
    {
        ClearFunctions();
        ClearTimestamps();
    }
}