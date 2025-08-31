#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>
#include <optional>

namespace Eclipse
{
    struct CacheEntry
    {
        std::vector<char> bytecode;
        std::string fileMD5;
        bool compilationSuccess = true;
        
        CacheEntry() = default;
        CacheEntry(std::vector<char>&& data, const std::string& md5)
            : bytecode(std::move(data)), fileMD5(md5) {}
    };

    class LuaCache
    {
    public:
        static LuaCache& GetInstance();
        
        // Core cache operations
        std::optional<std::vector<char>> GetBytecode(const std::string& filePath);
        void StoreBytecode(const std::string& filePath, std::vector<char>&& bytecode, bool success = true);
        void InvalidateScript(const std::string& filePath);
        void InvalidateAllScripts();
        bool IsScriptModified(const std::string& filePath) const;
        std::vector<std::string> GetModifiedScripts() const;
        
        // Get all cached scripts (for states that need to load from cache)
        std::vector<std::string> GetAllCachedScripts() const;
        
        void Clear();
        
    private:
        LuaCache() = default;
        ~LuaCache() = default;
        LuaCache(const LuaCache&) = delete;
        LuaCache& operator=(const LuaCache&) = delete;

        std::unordered_map<std::string, CacheEntry> cache_;

        // Helper methods
        std::string CalculateFileMD5(const std::string& filePath) const;
    };
}

#endif // ECLIPSE_LUA_CACHE_HPP