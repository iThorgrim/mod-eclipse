#ifndef ECLIPSE_LUA_CACHE_HPP
#define ECLIPSE_LUA_CACHE_HPP

#include "EclipseIncludes.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <boost/filesystem.hpp>

namespace Eclipse
{
    struct CacheEntry
    {
        std::vector<char> bytecode;
        boost::filesystem::path::time_type lastWriteTime;
        bool compilationSuccess = true;

        CacheEntry() = default;
        CacheEntry(std::vector<char>&& data, boost::filesystem::path::time_type writeTime)
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
        boost::filesystem::path::time_type GetFileWriteTime(const std::string& filePath) const;
    };
}

#endif // ECLIPSE_LUA_CACHE_HPP