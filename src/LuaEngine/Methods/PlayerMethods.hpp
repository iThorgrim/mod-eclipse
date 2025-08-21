#include "Player.h"
#include "Chat.h"

namespace Eclipse
{
    namespace PlayerMethods
    {
        void SendMessage(Player* player, const std::string& message)
        {
            if (!player || !player->GetSession())
                return;
                
            ChatHandler(player->GetSession()).PSendSysMessage("{}", message);
        }
        
        void SendNotification(Player* player, const std::string& message)
        {
            if (!player || !player->GetSession())
                return;
                
            ChatHandler(player->GetSession()).SendNotification("{}", message);
        }
        
        std::string GetName(Player* player)
        {
            if (!player)
                return "";
                
            return player->GetName();
        }
        
        uint8 GetLevel(Player* player)
        {
            if (!player)
                return 0;
                
            return player->GetLevel();
        }
        
        uint32 GetGUID(Player* player)
        {
            if (!player)
                return 0;
                
            return player->GetGUID().GetCounter();
        }
        
        uint32 GetHealth(Player* player)
        {
            if (!player)
                return 0;
                
            return player->GetHealth();
        }
        
        uint32 GetMaxHealth(Player* player)
        {
            if (!player)
                return 0;
                
            return player->GetMaxHealth();
        }
        
        uint32 GetPower(Player* player, uint8 powerType)
        {
            if (!player)
                return 0;
                
            return player->GetPower(Powers(powerType));
        }
        
        uint32 GetMaxPower(Player* player, uint8 powerType)
        {
            if (!player)
                return 0;
                
            return player->GetMaxPower(Powers(powerType));
        }
        
        float GetPositionX(Player* player)
        {
            if (!player)
                return 0.0f;
                
            return player->GetPositionX();
        }
        
        float GetPositionY(Player* player)
        {
            if (!player)
                return 0.0f;
                
            return player->GetPositionY();
        }
        
        float GetPositionZ(Player* player)
        {
            if (!player)
                return 0.0f;
                
            return player->GetPositionZ();
        }
        
        float GetOrientation(Player* player)
        {
            if (!player)
                return 0.0f;
                
            return player->GetOrientation();
        }
        
        uint32 GetMapId(Player* player)
        {
            if (!player)
                return 0;
                
            return player->GetMapId();
        }
        
        uint8 GetClass(Player* player)
        {
            if (!player)
                return 0;
                
            return player->getClass();
        }
        
        uint8 GetRace(Player* player)
        {
            if (!player)
                return 0;
                
            return player->getRace();
        }
        
        uint8 GetGender(Player* player)
        {
            if (!player)
                return 0;
                
            return player->getGender();
        }

        void Register(sol::state& lua)
        {
            auto player_type = lua.new_usertype<Player>("Player");
            
            // Communication methods
            player_type["SendMessage"] = SendMessage;
            player_type["SendNotification"] = &PlayerMethods::SendNotification;
            
            // Info methods
            player_type["GetName"] = &PlayerMethods::GetName;
            player_type["GetLevel"] = &PlayerMethods::GetLevel;
            player_type["GetGUID"] = &PlayerMethods::GetGUID;
            
            // Stats methods
            player_type["GetHealth"] = &PlayerMethods::GetHealth;
            player_type["GetMaxHealth"] = &PlayerMethods::GetMaxHealth;
            player_type["GetPower"] = &PlayerMethods::GetPower;
            player_type["GetMaxPower"] = &PlayerMethods::GetMaxPower;
            
            // Position methods
            player_type["GetPositionX"] = &PlayerMethods::GetPositionX;
            player_type["GetPositionY"] = &PlayerMethods::GetPositionY;
            player_type["GetPositionZ"] = &PlayerMethods::GetPositionZ;
            player_type["GetOrientation"] = &PlayerMethods::GetOrientation;
            player_type["GetMapId"] = &PlayerMethods::GetMapId;
            
            // Character info methods
            player_type["GetClass"] = &PlayerMethods::GetClass;
            player_type["GetRace"] = &PlayerMethods::GetRace;
            player_type["GetGender"] = &PlayerMethods::GetGender;
        }
    }
}