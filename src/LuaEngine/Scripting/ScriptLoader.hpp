#ifndef ECLIPSE_SCRIPT_LOADER_HPP
#define ECLIPSE_SCRIPT_LOADER_HPP

#include "EclipseIncludes.hpp"
#include "LuaPathManager.hpp"

#include <string>
#include <vector>
#include <set>

namespace Eclipse
{
    struct LoadStatistics
    {
        int compiled = 0;
        int cached = 0;
        int precompiled = 0;
        int total = 0;
        int failed = 0;
        uint32 duration = 0;

        int GetSuccessful() const { return compiled + cached + precompiled; }
    };

    class ScriptLoader
    {
    public:
        // Script discovery and loading orchestration
        static bool LoadScript(sol::state& targetState, sol::state& compilerState, const std::string& filePath);
        static bool LoadDirectory(sol::state& targetState, sol::state& compilerState, const std::string& directoryPath, std::vector<std::string>& loadedScripts, LoadStatistics* stats = nullptr);

        // File discovery utilities
        static std::vector<std::string> DiscoverScripts(const std::string& directoryPath);
        static std::set<std::string> ExtractDirectoriesFromScripts(const std::vector<std::string>& scriptPaths);
        static bool IsValidScriptExtension(const std::string& extension);

        // Bytecode loading utility (public for LuaEngine use)
        static bool LoadBytecodeIntoState(sol::state& targetState, const std::vector<char>& bytecode, const std::string& chunkName);

    private:
        ScriptLoader() = delete;
        ~ScriptLoader() = default;
        ScriptLoader(const ScriptLoader&) = delete;
        ScriptLoader& operator=(const ScriptLoader&) = delete;
        static int LoadFiles(sol::state& targetState, sol::state& compilerState, const std::vector<std::string>& files, std::vector<std::string>& loadedScripts, LoadStatistics* stats = nullptr);
        static void ProcessSubdirectories(sol::state& targetState, sol::state& compilerState, const std::string& directoryPath, std::vector<std::string>& loadedScripts, LoadStatistics* stats = nullptr);
    };
}

#endif // ECLIPSE_SCRIPT_LOADER_HPP