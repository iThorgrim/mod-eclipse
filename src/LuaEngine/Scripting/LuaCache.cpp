#include "LuaCache.hpp"
#include <boost/filesystem.hpp>

namespace Eclipse
{
    LuaCache& LuaCache::GetInstance()
    {
        static LuaCache instance;
        return instance;
    }

    boost::filesystem::path::time_type LuaCache::GetFileWriteTime(const std::string& filePath) const
    {
        boost::system::error_code ec;
        auto writeTime = boost::filesystem::last_write_time(filePath, ec);
        
        if (ec)
        {
            LOG_ERROR("server.eclipse", "[Eclipse]: Failed to get write time for file {}: {}", filePath, ec.message());
            return boost::filesystem::path::time_type();
        }

        return writeTime;
    }

    std::optional<std::vector<char>> LuaCache::GetBytecode(const std::string& filePath)
    {
        auto it = cache_.find(filePath);
        if (it == cache_.end())
        {
            return std::nullopt;
        }

        auto& entry = it->second;

        if (IsScriptModified(filePath))
        {
            InvalidateScript(filePath);
            return std::nullopt;
        }

        if (!entry.compilationSuccess)
        {
            return std::nullopt;
        }

        return entry.bytecode;
    }

    void LuaCache::StoreBytecode(const std::string& filePath, std::vector<char>&& bytecode, bool success)
    {
        auto writeTime = GetFileWriteTime(filePath);

        CacheEntry entry(std::move(bytecode), writeTime);
        entry.compilationSuccess = success;

        cache_[filePath] = std::move(entry);

        LOG_DEBUG("server.eclipse", "[Eclipse]: Cached script: {} (success: {})", filePath, success);
    }

    void LuaCache::InvalidateScript(const std::string& filePath)
    {
        auto it = cache_.find(filePath);
        if (it != cache_.end())
        {
            cache_.erase(it);

            LOG_TRACE("server.eclipse", "[Eclipse]: Invalidated cache for script: {}", filePath);
        }
    }

    void LuaCache::InvalidateAllScripts()
    {
        cache_.clear();
    }

    bool LuaCache::IsScriptModified(const std::string& filePath) const
    {
        auto it = cache_.find(filePath);
        if (it == cache_.end())
        {
            return true;
        }

        const auto& entry = it->second;

        auto currentWriteTime = GetFileWriteTime(filePath);
        if (currentWriteTime == boost::filesystem::path::time_type() || currentWriteTime != entry.lastWriteTime)
        {
            return true;
        }

        return false;
    }

    std::vector<std::string> LuaCache::GetModifiedScripts() const
    {
        std::vector<std::string> modifiedScripts;

        for (const auto& [filePath, entry] : cache_)
        {
            if (IsScriptModified(filePath))
            {
                modifiedScripts.push_back(filePath);
            }
        }

        return modifiedScripts;
    }

    std::vector<std::string> LuaCache::GetAllCachedScripts() const
    {
        std::vector<std::string> scripts;
        scripts.reserve(cache_.size());

        for (const auto& [filePath, entry] : cache_)
        {
            if (entry.compilationSuccess)
            {
                scripts.push_back(filePath);
            }
        }

        return scripts;
    }

    void LuaCache::Clear()
    {
        InvalidateAllScripts();
    }
}