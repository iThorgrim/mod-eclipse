#ifndef ECLIPSE_PLAYER_METHODS_HPP
#define ECLIPSE_PLAYER_METHODS_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    namespace PlayerMethods
    {
        void SendMessage(Player* player, const std::string& message)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("{}", message);
        }
        
        void SendNotification(Player* player, const std::string& message)
        {
            ChatHandler(player->GetSession()).SendNotification("{}", message);
        }
        
        std::string GetName(Player* player)
        {
            return player->GetName();
        }
        
        uint8 GetLevel(Player* player)
        {
            return player->GetLevel();
        }
        
        uint32 GetGUID(Player* player)
        {
            return player->GetGUID().GetCounter();
        }
        
        uint32 GetHealth(Player* player)
        {
            return player->GetHealth();
        }
        
        uint32 GetMaxHealth(Player* player)
        {
            return player->GetMaxHealth();
        }
        
        uint32 GetPower(Player* player, uint8 powerType)
        {
            return player->GetPower(Powers(powerType));
        }
        
        uint32 GetMaxPower(Player* player, uint8 powerType)
        {
            return player->GetMaxPower(Powers(powerType));
        }
        
        float GetPositionX(Player* player)
        {
            return player->GetPositionX();
        }
        
        float GetPositionY(Player* player)
        {
            return player->GetPositionY();
        }
        
        float GetPositionZ(Player* player)
        {
            return player->GetPositionZ();
        }
        
        float GetOrientation(Player* player)
        {
            return player->GetOrientation();
        }
        
        uint32 GetMapId(Player* player)
        {
            return player->GetMapId();
        }
        
        uint8 GetClass(Player* player)
        {
            return player->getClass();
        }
        
        uint8 GetRace(Player* player)
        {                
            return player->getRace();
        }
        
        uint8 GetGender(Player* player)
        {                
            return player->getGender();
        }

        void SetLevel(Player* player, uint8 level)
        {
            player->GiveLevel(level);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);
        }

        void Register(sol::state& lua)
        {
            auto player_type = lua.new_usertype<Player>("Player");
            
            // Setters
            player_type["SetLevel"] = SetLevel;

            // Getters
            player_type["GetName"] = GetName;
            player_type["GetLevel"] = GetLevel;
            player_type["GetGUID"] = GetGUID;
            player_type["GetHealth"] = GetHealth;
            player_type["GetMaxHealth"] = GetMaxHealth;
            player_type["GetPower"] = GetPower;
            player_type["GetMaxPower"] = GetMaxPower;
            player_type["GetPositionX"] = GetPositionX;
            player_type["GetPositionY"] = GetPositionY;
            player_type["GetPositionZ"] = GetPositionZ;
            player_type["GetOrientation"] = GetOrientation;
            player_type["GetMapId"] = GetMapId;
            player_type["GetClass"] = GetClass;
            player_type["GetRace"] = GetRace;
            player_type["GetGender"] = GetGender;

            // Other
            player_type["SendMessage"] = SendMessage;
            player_type["SendNotification"] = SendNotification;
        }
    }
}

#endif // ECLIPSE_PLAYER_METHODS_HPP