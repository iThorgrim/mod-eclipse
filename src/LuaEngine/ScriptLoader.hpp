#ifndef ECLIPSE_SCRIPT_LOADER_HPP
#define ECLIPSE_SCRIPT_LOADER_HPP

#include "EclipseIncludes.hpp"
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
        
        static std::vector<std::string> ScanDirectory(const std::string& path);
        static std::vector<char> CompileToBytecode(sol::state& lua, const std::string& source);
        static std::string ReadFileContent(const std::string& path);
        static bool LoadPrecompiledFile(sol::state& lua, const std::string& filePath);
    };
}

#endif // ECLIPSE_SCRIPT_LOADER_HPP