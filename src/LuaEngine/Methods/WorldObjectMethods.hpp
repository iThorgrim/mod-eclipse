#ifndef ECLIPSE_WORLDOBJECT_METHODS_HPP
#define ECLIPSE_WORLDOBJECT_METHODS_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    namespace WorldObjectMethods
    {
        // ========== BASIC OBJECT METHODS ==========
        
        uint64 GetGUID(WorldObject* obj)
        {
            return obj ? obj->GetGUID().GetRawValue() : 0;
        }
        
        uint32 GetEntry(WorldObject* obj)
        {
            if (!obj) return 0;
            
            if (auto* creature = obj->ToCreature())
                return creature->GetEntry();
            else if (auto* gameobject = obj->ToGameObject())
                return gameobject->GetEntry();
            else if (auto* item = dynamic_cast<Item*>(obj))
                return item->GetEntry();
            
            return 0;
        }
        
        uint32 GetMapId(WorldObject* obj)
        {
            return obj && obj->GetMap() ? obj->GetMap()->GetId() : 0;
        }
        
        float GetX(WorldObject* obj)
        {
            return obj ? obj->GetPositionX() : 0.0f;
        }
        
        float GetY(WorldObject* obj)
        {
            return obj ? obj->GetPositionY() : 0.0f;
        }
        
        float GetZ(WorldObject* obj)
        {
            return obj ? obj->GetPositionZ() : 0.0f;
        }
        
        float GetO(WorldObject* obj)
        {
            return obj ? obj->GetOrientation() : 0.0f;
        }
        
        bool IsInWorld(WorldObject* obj)
        {
            return obj ? obj->IsInWorld() : false;
        }
        
        std::string GetName(WorldObject* obj)
        {
            if (!obj) return "";
            
            if (auto* creature = obj->ToCreature())
                return creature->GetName();
            else if (auto* gameobject = obj->ToGameObject())
                return gameobject->GetName();
            else if (auto* player = obj->ToPlayer())
                return player->GetName();
            else if (auto* item = dynamic_cast<Item*>(obj))
                return item->GetTemplate()->Name1;
            
            return "";
        }
        
        // ========== LUA REGISTRATION ==========
        template<typename T>
        void RegisterWorldObjectMethods(sol::usertype<T>& type)
        {
            type["GetGUID"] = [](T* obj) { return GetGUID(obj); };
            type["GetEntry"] = [](T* obj) { return GetEntry(obj); };
            type["GetMapId"] = [](T* obj) { return GetMapId(obj); };
            type["GetX"] = [](T* obj) { return GetX(obj); };
            type["GetY"] = [](T* obj) { return GetY(obj); };
            type["GetZ"] = [](T* obj) { return GetZ(obj); };
            type["GetO"] = [](T* obj) { return GetO(obj); };
            type["IsInWorld"] = [](T* obj) { return IsInWorld(obj); };
            type["GetName"] = [](T* obj) { return GetName(obj); };
        }
    }
}

#endif // ECLIPSE_WORLDOBJECT_METHODS_HPP