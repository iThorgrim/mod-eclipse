#include "LuaCache.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>

namespace Eclipse
{
    LuaCache& LuaCache::GetInstance()
    {
        static LuaCache instance;
        return instance;
    }

    std::string LuaCache::CalculateFileMD5(const std::string& filePath) const
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
        {
            return "";
        }

        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
        if (!mdctx)
        {
            return "";
        }

        if (EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr) != 1)
        {
            EVP_MD_CTX_free(mdctx);
            return "";
        }

        char buffer[8192];
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
        {
            if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1)
            {
                EVP_MD_CTX_free(mdctx);
                return "";
            }
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_len;
        if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1)
        {
            EVP_MD_CTX_free(mdctx);
            return "";
        }

        EVP_MD_CTX_free(mdctx);

        std::stringstream hashStream;
        hashStream << std::hex << std::setfill('0');
        for (unsigned int i = 0; i < hash_len; ++i)
        {
            hashStream << std::setw(2) << static_cast<int>(hash[i]);
        }

        return hashStream.str();
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
        std::string md5 = CalculateFileMD5(filePath);
        
        CacheEntry entry(std::move(bytecode), md5);
        entry.compilationSuccess = success;

        cache_[filePath] = std::move(entry);

        LOG_TRACE("server.eclipse", "[Eclipse]: Cached script: {} (success: {}, MD5: {})", filePath, success, md5.substr(0, 8));
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
        
        std::string currentMD5 = CalculateFileMD5(filePath);
        if (currentMD5.empty() || currentMD5 != entry.fileMD5)
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