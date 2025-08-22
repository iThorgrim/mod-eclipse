#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>

namespace Eclipse
{
    struct BytecodeEntry
    {
        std::vector<char> bytecode;
        std::filesystem::file_time_type lastModified;
        
        BytecodeEntry() = default;
        BytecodeEntry(std::vector<char>&& data, const std::filesystem::file_time_type& time)
            : bytecode(std::move(data)), lastModified(time) {}
    };

    class LuaCache
    {
    public:
        LuaCache() = default;
        ~LuaCache() = default;
        
        bool IsCacheValid(const std::string& path) const;
        bool LoadFromCache(sol::state& lua, const std::string& path);
        void CacheBytecode(const std::string& path, std::vector<char>&& bytecode);
        void InvalidateCache(const std::string& path);
        void Clear();
        
        size_t GetCacheSize() const;
        void LogCacheStats() const;
        
    private:
        std::unordered_map<std::string, BytecodeEntry> cache;
        
        std::filesystem::file_time_type GetFileTimestamp(const std::string& path) const;
    };
    
    LuaCache& GetGlobalCache();
}

#endif // ECLIPSE_LUA_CACHE_HPP