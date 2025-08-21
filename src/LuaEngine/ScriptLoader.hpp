#ifndef ECLIPSE_SCRIPT_LOADER_HPP
#define ECLIPSE_SCRIPT_LOADER_HPP

#include "sol.hpp"
#include <string>
#include <vector>

namespace Eclipse
{
    class ScriptLoader
    {
    public:
        static bool LoadFile(sol::state& lua, const std::string& filePath);
        static bool LoadString(sol::state& lua, const std::string& script);
        static bool LoadDirectory(sol::state& lua, const std::string& directoryPath, 
                                 std::vector<std::string>& loadedScripts);
        
        static std::vector<std::string> ScanDirectory(const std::string& path);
        
    private:
        ScriptLoader() = delete; // Static-only class
    };
}

#endif // ECLIPSE_SCRIPT_LOADER_HPP