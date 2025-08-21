#ifndef ECLIPSE_LUA_MANAGER_HPP
#define ECLIPSE_LUA_MANAGER_HPP

#include "LuaEngine.hpp"
#include <memory>
#include <mutex>

namespace Eclipse
{
    class LuaManager
    {
    public:
        static LuaEngine* GetInstance();
        static void CreateInstance();
        static void DestroyInstance();

    private:
        static std::unique_ptr<LuaEngine> instance;
        static std::once_flag initialized;
    };
}

#endif // ECLIPSE_LUA_MANAGER_HPP