#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <boost/filesystem.hpp>
#include <chrono>

namespace Eclipse
{
    struct CacheEntry
    {
        std::vector<char> bytecode;
        std::chrono::system_clock::time_point lastWriteTime;
        bool compilationSuccess = true;

        CacheEntry() = default;
        CacheEntry(std::vector<char>&& data, std::chrono::system_clock::time_point writeTime)
            : bytecode(std::move(data)), lastWriteTime(writeTime) {}
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

        std::vector<std::string> GetAllCachedScripts() const;

        void Clear();

    private:
        LuaCache() = default;
        ~LuaCache() = default;
        LuaCache(const LuaCache&) = delete;
        LuaCache& operator=(const LuaCache&) = delete;

        std::unordered_map<std::string, CacheEntry> cache_;

        // Helper methods
        std::chrono::system_clock::time_point GetFileWriteTime(const std::string& filePath) const;
    };
}

#endif // ECLIPSE_LUA_CACHE_HPP