#ifndef ECLIPSE_LUA_COMPILER_HPP
#define ECLIPSE_LUA_COMPILER_HPP

#include "EclipseIncludes.hpp"
#include "LuaCache.hpp"
#include <memory>

namespace Eclipse
{
    class LuaCompiler
    {
    public:
        LuaCompiler();
        ~LuaCompiler() = default;

        // Script compilation and execution
        bool CompileAndExecute(sol::state& luaState, const std::string& scriptPath);
        bool ExecuteScript(sol::state& luaState, const std::string& script);
        
        // Cache management
        void ClearCache();
        LuaCache& GetCache() { return cache; }
        const LuaCache& GetCache() const { return cache; }

    private:
        LuaCache cache;
        
        // Internal compilation helpers
        bool CompileFromFile(sol::state& luaState, const std::string& scriptPath);
        bool CompileFromString(sol::state& luaState, const std::string& script, const std::string& name = "chunk");
        
        // Error handling
        void HandleCompilationError(const std::string& script, const std::string& error);
    };
}

#endif // ECLIPSE_LUA_COMPILER_HPP