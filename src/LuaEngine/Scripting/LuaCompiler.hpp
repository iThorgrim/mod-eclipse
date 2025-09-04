#ifndef ECLIPSE_LUA_COMPILER_HPP
#define ECLIPSE_LUA_COMPILER_HPP

#include "EclipseIncludes.hpp"
#include <string>
#include <vector>

namespace Eclipse
{
    class LuaCompiler
    {
    public:
        LuaCompiler() = default;
        ~LuaCompiler() = default;

        // Pure compilation interface - no state management
        static std::vector<char> CompileTobytecode(sol::state& compilerState, const std::string& luaSource, const std::string& chunkName = "chunk");
        static std::string CompileMoonScriptToLua(sol::state& compilerState, const std::string& moonFilePath);
        static std::string ReadFileContent(const std::string& filePath);

        // Full compilation chain for different file types
        static std::vector<char> CompileFileTobytecode(sol::state& compilerState, const std::string& filePath);

    private:
        static void HandleCompilationError(const std::string& source, const std::string& error);
    };
}

#endif // ECLIPSE_LUA_COMPILER_HPP