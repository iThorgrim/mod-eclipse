#ifndef ECLIPSE_SCRIPT_LOADER_HPP
#define ECLIPSE_SCRIPT_LOADER_HPP

#include "EclipseIncludes.hpp"
#include "LuaPathManager.hpp"

#include <string>
#include <vector>

namespace Eclipse
{
    class ScriptLoader
    {
    public:
        static bool LoadFile(sol::state& lua, const std::string& filePath);
        static bool LoadDirectory(sol::state& lua, const std::string& directoryPath, std::vector<std::string>& loadedScripts);

    private:
        ScriptLoader() = delete;
        ~ScriptLoader() = default;
        ScriptLoader(const ScriptLoader&) = delete;
        ScriptLoader& operator=(const ScriptLoader&) = delete;

        static std::vector<char> CompileToBytecode(sol::state& lua, const std::string& source);
        static std::string ReadFileContent(const std::string& path);
        static bool LoadPrecompiledFile(sol::state& lua, const std::string& filePath);
        static int LoadFiles(sol::state& lua, const std::vector<std::string>& files, std::vector<std::string>& loadedScripts);
        static void ProcessSubdirectories(sol::state& lua, const std::string& directoryPath, std::vector<std::string>& loadedScripts);
        static bool IsValidScriptExtension(const std::string& ext);
    };
}

#endif // ECLIPSE_SCRIPT_LOADER_HPP