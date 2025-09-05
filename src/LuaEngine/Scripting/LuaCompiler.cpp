#include "LuaCompiler.hpp"
#include "EclipseLogger.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Eclipse
{
    std::string LuaCompiler::ReadFileContent(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            EclipseLogger::GetInstance().LogError("Failed to open file: " + filePath);
            return "";
        }

        auto size = file.tellg();
        std::string content(size, '\0');
        file.seekg(0);
        file.read(content.data(), size);

        return content;
    }

    std::string LuaCompiler::CompileMoonScriptToLua(sol::state& compilerState, const std::string& moonFilePath)
    {
        try
        {
            sol::table moonscript = compilerState["require"]("moonscript");
            sol::function loadfile = moonscript["loadfile"];
            sol::object result = loadfile(moonFilePath);

            if (result.valid())
            {
                sol::function tostring = compilerState["tostring"];
                return tostring(result).get<std::string>();
            }
            else
            {
                EclipseLogger::GetInstance().LogLuaCompilationError(moonFilePath, "Failed to compile MoonScript file");
                return "";
            }
        }
        catch (const std::exception& e)
        {
            HandleCompilationError(moonFilePath, e.what());
            return "";
        }
    }

    std::vector<char> LuaCompiler::CompileTobytecode(sol::state& compilerState, const std::string& luaSource, const std::string& chunkName)
    {
        try
        {
            lua_State* L = compilerState.lua_state();

            int result = luaL_loadbuffer(L, luaSource.c_str(), luaSource.length(), chunkName.c_str());
            if (result != LUA_OK)
            {
                std::string error = lua_tostring(L, -1);
                lua_pop(L, 1);
                HandleCompilationError(chunkName, error);
                return {};
            }

            // Use lua_dump to get bytecode from compiled function
            struct BytecodeWriter {
                std::vector<char> data;
                BytecodeWriter() { data.reserve(8192); }
            };

            BytecodeWriter writer;

            auto dumpWriter = [](lua_State* /*L*/, const void* p, size_t sz, void* ud) -> int {
                BytecodeWriter* w = static_cast<BytecodeWriter*>(ud);
                const char* bytes = static_cast<const char*>(p);
                w->data.insert(w->data.end(), bytes, bytes + sz);
                return 0;
            };

            if (lua_dump(L, dumpWriter, &writer, 0) != 0)
            {
                lua_pop(L, 1);
                HandleCompilationError(chunkName, "Failed to dump bytecode");
                return {};
            }

            lua_pop(L, 1); // Remove the function from stack
            return writer.data;
        }
        catch (const std::exception& e)
        {
            HandleCompilationError(chunkName, e.what());
            return {};
        }
    }

    std::vector<char> LuaCompiler::CompileFileTobytecode(sol::state& compilerState, const std::string& filePath)
    {
        if (!std::filesystem::exists(filePath))
        {
            EclipseLogger::GetInstance().LogScriptNotFound(filePath, false);
            return {};
        }

        std::filesystem::path path(filePath);
        std::string extension = path.extension().string();
        std::string luaSource;

        if (extension == ".moon")
        {
            luaSource = CompileMoonScriptToLua(compilerState, filePath);
            if (luaSource.empty())
            {
                return {};
            }
        }
        else if (extension == ".lua" || extension == ".ext")
        {
            luaSource = ReadFileContent(filePath);
            if (luaSource.empty())
            {
                return {};
            }
        }
        else if (extension == ".out")
        {
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open())
            {
                EclipseLogger::GetInstance().LogError("Failed to open precompiled file: " + filePath);
                return {};
            }

            file.seekg(0, std::ios::end);
            std::vector<char> bytecode;
            bytecode.reserve(file.tellg());
            file.seekg(0, std::ios::beg);
            bytecode.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            return bytecode;
        }
        else
        {
            EclipseLogger::GetInstance().LogError("Unsupported script file extension: " + extension);
            return {};
        }

        return CompileTobytecode(compilerState, luaSource, filePath);
    }

    void LuaCompiler::HandleCompilationError(const std::string& source, const std::string& error)
    {
        EclipseLogger::GetInstance().LogLuaCompilationError(source, error);
    }
}