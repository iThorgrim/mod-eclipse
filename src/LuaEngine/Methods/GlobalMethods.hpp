#ifndef ECLIPSE_GLOBAL_METHODS_HPP
#define ECLIPSE_GLOBAL_METHODS_HPP

#include "LuaEngine.hpp"
#include "MessageManager.hpp"

namespace Eclipse
{
    namespace GlobalMethods
    {
        template <typename Ret, typename... Args>
        auto Bind(Ret(*func)(LuaEngine*, Args...), LuaEngine* engine) {
            return [engine, func](Args&&... args) -> Ret {
                return func(engine, std::forward<Args>(args)...);
            };
        }

        inline int32 GetStateMapId(LuaEngine* lua)
        {
            return lua->GetStateMapId();
        }

        inline void SendStateMessage(LuaEngine* lua, int32 toStateId, const std::string& messageType, sol::object data)
        {
            MessageManager::GetInstance().SendMessage(lua->GetStateMapId(), toStateId, messageType, data);
        }

        // ========== LUA REGISTRATION ==========
        void Register(LuaEngine* lua_engine, sol::state& lua)
        {           
            lua["GetStateMapId"] = Bind(&GetStateMapId, lua_engine);

            lua["SendStateMessage"] = Bind(&SendStateMessage, lua_engine);
        }
    }
}

#endif // ECLIPSE_GLOBAL_METHODS_HPP