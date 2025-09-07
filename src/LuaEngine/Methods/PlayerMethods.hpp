#ifndef ECLIPSE_PLAYER_METHODS_HPP
#define ECLIPSE_PLAYER_METHODS_HPP

#include "EclipseIncludes.hpp"
#include "Pet.h"
#include "SocialMgr.h"

namespace Eclipse
{
    namespace PlayerMethods
    {

        /**
         * @brief Teleports [Player] to a specific map coordinate with various options.
         * @brief Can handle combat state, vehicles, pets and other special conditions.
         *
         * @enum TeleportToOptions
         * @enum {
         * @enum    TELE_TO_GM_MODE             = 0x01,
         * @enum    TELE_TO_NOT_LEAVE_TRANSPORT = 0x02,
         * @enum    TELE_TO_NOT_LEAVE_COMBAT    = 0x04,
         * @enum    TELE_TO_NOT_UNSUMMON_PET    = 0x08,
         * @enum    TELE_TO_SPELL               = 0x10,
         * @enum    TELE_TO_NOT_LEAVE_VEHICLE   = 0x20,
         * @enum    TELE_TO_WITH_PET            = 0x40,
         * @enum    TELE_TO_NOT_LEAVE_TAXI      = 0x80
         * @enum }
         *
         * @code {.lua}
         * player:Teleport(mapId, x, y, z, o)
         * player:Teleport(mapId, x, y, z, o, TELE_TO_NOT_LEAVE_COMBAT, target, true)
         * @endcode
         *
         * @param uint32 mapId
         * @param float xCoord
         * @param float yCoord
         * @param float zCoord
         * @param float orientation
         * @param uint32 options = 0 (optional teleportation flags)
         * @param [Unit] target = nullptr (optional target for spell-based teleports)
         * @param bool forced = false (force teleport even if conditions aren't met)
         *
         * @return bool success : true if the player was teleported
         */
        inline bool TeleportTo(Player* player, uint32 mapId, float x, float y, float z, float orientation, uint32 options, Unit* target, bool forced)
        {
            return player->TeleportTo(mapId, x, y, z, orientation, options, target, forced);
        }

        /**
         *
         */
        inline bool TeleportToEntryPoint(Player* player)
        {
            return player->TeleportToEntryPoint();
        }

        /**
         *
         */
        inline void SetSummonPoint(Player* player, uint32 mapId, float x, float y, float z, uint32 zoneId, bool asSpectator)
        {
            player->SetSummonPoint(mapId, x, y, z, zoneId, asSpectator);
        }

        /**
         *
         */
        inline bool IsSummonAsSpectator(Player* player)
        {
            return player->IsSummonAsSpectator();
        }

        /**
         *
         */
        inline void SetSummonAsSpectator(Player* player, bool spectator)
        {
            player->SetSummonAsSpectator(spectator);
        }

        /**
         *
         */
        inline void SummonIfPossible(Player* player, bool asSpectator, ObjectGuid summoner)
        {
            player->SummonIfPossible(asSpectator, summoner);
        }

        /**
         *
         */
        inline time_t GetSummonExpireTimer(Player* player)
        {
            return player->GetSummonExpireTimer();
        }

        /**
         *
         */
        inline PlayerFlags GetPlayerFlags(Player* player)
        {
            return player->GetPlayerFlags();
        }

        /**
         *
         */
        inline bool HasPlayerFlag(Player* player, PlayerFlags flag)
        {
            return player->HasPlayerFlag(flag);
        }

        /**
         *
         */
        inline void SetPlayerFlag(Player* player, PlayerFlags flag)
        {
            player->SetPlayerFlag(flag);
        }

        /**
         *
         */
        inline void RemovePlayerFlag(Player* player, PlayerFlags flag)
        {
            player->RemovePlayerFlag(flag);
        }

        /**
         *
         */
        inline void ReplaceAllPlayerFlags(Player* player, PlayerFlags flags)
        {
            player->ReplaceAllPlayerFlags(flags);
        }

        /**
         *
         */
        inline bool IsClass(Player* player, Classes clss, ClassContext context)
        {
            return player->IsClass(clss, context);
        }

        /**
         *
         */
        inline void SetInWater(Player* player, bool apply)
        {
            player->SetInWater(apply);
        }

        /**
         *
         */
        inline bool IsInWater(Player* player)
        {
            return player->IsInWater();
        }

        /**
         *
         */
        inline bool IsFalling(Player* player)
        {
            return player->IsFalling();
        }

        /**
         *
         */
        inline bool IsInAreaTriggerRadius(Player* player, const AreaTrigger* at, float delta)
        {
            return player->IsInAreaTriggerRadius(at, delta);
        }

        /**
         *
         */
        inline bool CanInteractWithQuestGiver(Player* player, Object* questGiver)
        {
            return player->CanInteractWithQuestGiver(questGiver);
        }

        /**
         *
         */
        inline Creature* GetNPCIfCanInteractWith(Player* player, ObjectGuid guid, uint32 npcflagmask)
        {
            return player->GetNPCIfCanInteractWith(guid, npcflagmask);
        }

        /**
         *
         */
        inline GameObject* GetGameObjectIfCanInteractWith(Player* player, ObjectGuid guid, GameobjectTypes type)
        {
            return player->GetGameObjectIfCanInteractWith(guid, type);
        }

        /**
         *
         */
        inline void ToggleAFK(Player* player)
        {
            player->ToggleAFK();
        }

        /**
         *
         */
        inline void ToggleDND(Player* player)
        {
            player->ToggleDND();
        }

        /**
         *
         */
        inline bool IsAFK(Player* player)
        {
            return player->isAFK();
        }

        /**
         *
         */
        inline bool IsDND(Player* player)
        {
            return player->isDND();
        }

        /**
         *
         */
        inline uint8 GetChatTag(Player* player)
        {
            return player->GetChatTag();
        }

        /**
         *
         */
        inline uint32 GetBarberShopCost(Player* player, uint8 newHairStyle, uint8 newHairColor, uint8 newFacialHair, const BarberShopStyleEntry* newSkin)
        {
            return player->GetBarberShopCost(newHairStyle, newHairColor, newFacialHair, newSkin);
        }

        /**
         *
         */
        inline PlayerSocial* GetSocial(Player* player)
        {
            return player->GetSocial();
        }

        /**
         *
         */
        inline void InitTaxiNodesForLevel(Player* player)
        {
            player->InitTaxiNodesForLevel();
        }

        /**
         *
         */
        inline bool ActivateTaxiPathToByVector(Player* player, const std::vector<uint32>& nodes, Creature* npc, uint32 spellid)
        {
            return player->ActivateTaxiPathTo(nodes, npc, spellid);
        }

        /**
         *
         */
        inline bool ActivateTaxiPathToByNodes(Player* player, uint32 taxi_path_id, uint32 spellid)
        {
            return player->ActivateTaxiPathTo(taxi_path_id, spellid);
        }

        /**
         *
         */
        inline void CleanupAfterTaxiFlight(Player* player)
        {
            player->CleanupAfterTaxiFlight();
        }

        /**
         *
         */
        inline void ContinueTaxiFlight(Player* player)
        {
            player->ContinueTaxiFlight();
        }

        /**
         *
         */
        inline void SendTaxiNodeStatusMultiple(Player* player)
        {
            player->SendTaxiNodeStatusMultiple();
        }

        /**
         *
         */
        inline bool IsCommentator(Player* player)
        {
            return player->IsCommentator();
        }

        /**
         *
         */
        inline void SetCommentator(Player* player, bool on)
        {
            player->SetCommentator(on);
        }

        /**
         *
         */
        inline bool IsDeveloper(Player* player)
        {
            return player->IsDeveloper();
        }

        /**
         *
         */
        inline void SetDeveloper(Player* player, bool on)
        {
            player->SetDeveloper(on);
        }

        /**
         *
         */
        inline bool IsAcceptWhispers(Player* player)
        {
            return player->isAcceptWhispers();
        }

        /**
         *
         */
        inline void SetAcceptWhispers(Player* player, bool on)
        {
            player->SetAcceptWhispers(on);
        }

        /**
         *
         */
        inline bool IsGameMaster(Player* player)
        {
            return player->IsGameMaster();
        }

        /**
         *
         */
        inline void SetGameMaster(Player* player, bool on)
        {
            player->SetGameMaster(on);
        }

        /**
         *
         */
        inline bool IsGMChat(Player* player)
        {
            return player->isGMChat();
        }

        /**
         *
         */
        inline void SetGMChat(Player* player, bool on)
        {
            player->SetGMChat(on);
        }

        /**
         *
         */
        inline bool IsGMSpectator(Player* player)
        {
            return player->IsGMSpectator();
        }

        /**
         *
         */
        inline void SetGMSpectator(Player* player, bool on)
        {
            player->SetGMSpectator(on);
        }

        /**
         *
         */
        inline bool IsTaxiCheater(Player* player)
        {
            return player->isTaxiCheater();
        }

        /**
         *
         */
        inline void SetTaxiCheater(Player* player, bool on)
        {
            player->SetTaxiCheater(on);
        }

        /**
         *
         */
        inline bool IsGMVisible(Player* player)
        {
            return player->isGMVisible();
        }

        /**
         *
         */
        inline void SetGMVisible(Player* player, bool on)
        {
            player->SetGMVisible(on);
        }

        /**
         *
         */
        inline bool Has310Flyer(Player* player, bool checkAllSpells, uint32 excludeSpellId)
        {
            return player->Has310Flyer(checkAllSpells, excludeSpellId);
        }

        /**
         *
         */
        inline void SetHas310Flyer(Player* player, bool on)
        {
            player->SetHas310Flyer(on);
        }

        /**
         *
         */
        inline void SetPvPDeath(Player* player, bool on)
        {
            player->SetPvPDeath(on);
        }

        /**
         *
         */
        inline void GiveXP(Player* player, uint32 xp, Unit* victim, float group_rate, bool use_group_rate)
        {
            player->GiveXP(xp, victim, group_rate, use_group_rate);
        }

        /**
         *
         */
        inline void GiveLevel(Player* player, uint8 level)
        {
            player->GiveLevel(level);
        }

        /**
         *
         */
        inline void InitStatsForLevel(Player* player, bool reapplyMods)
        {
            player->InitStatsForLevel(reapplyMods);
        }

        /**
         *
         */
        inline bool HasActivePowerType(Player* player, Powers power)
        {
            return player->HasActivePowerType(power);
        }

        /**
         *
         */
        inline bool GetCommandStatus(Player* player, uint32 command)
        {
            return player->GetCommandStatus(command);
        }

        /**
         *
         */
        inline void SetCommandStatusOn(Player* player, uint32 command)
        {
            player->SetCommandStatusOn(command);
        }

        /**
         *
         */
        inline void SetCommandStatusOff(Player* player, uint32 command)
        {
            player->SetCommandStatusOff(command);
        }

        /**
         *
         */
        inline uint32 GetTotalPlayedTime(Player* player)
        {
            return player->GetTotalPlayedTime();
        }

        /**
         *
         */
        inline uint32 GetLevelPlayedTime(Player* player)
        {
            return player->GetLevelPlayedTime();
        }

        /**
         *
         */
        inline void SetDeathState(Player* player, DeathState s, bool despawn)
        {
            player->setDeathState(s, despawn);
        }

        /**
         *
         */
        inline void SetRestState(Player* player, uint32 triggerId)
        {
            player->SetRestState(triggerId);
        }

        /**
         *
         */
        inline void RemoveRestState(Player* player)
        {
            player->RemoveRestState();
        }

        /**
         *
         */
        inline uint32 GetXPRestBonus(Player* player, uint32 xp)
        {
            return player->GetXPRestBonus(xp);
        }

        /**
         *
         */
        inline float GetRestBonus(Player* player)
        {
            return player->GetRestBonus();
        }

        /**
         *
         */
        inline void SetRestBonus(Player* player, float rest_bonus_new)
        {
            player->SetRestBonus(rest_bonus_new);
        }

        /**
         *
         */
        inline bool HasRestFlag(Player* player, RestFlag flag)
        {
            return player->HasRestFlag(flag);
        }

        /**
         *
         */
        inline void SetRestFlag(Player* player, RestFlag flag, uint32 triggerId)
        {
            player->SetRestFlag(flag, triggerId);
        }

        /**
         *
         */
        inline void RemoveRestFlag(Player* player, RestFlag flag)
        {
            player->RemoveRestFlag(flag);
        }

        /**
         *
         */
        inline uint32 GetInnTriggerId(Player* player)
        {
            return player->GetInnTriggerId();
        }

        /**
         *
         */
        inline PetStable* GetPetStable(Player* player)
        {
            return player->GetPetStable();
        }

        /**
         *
         */
        inline const PetStable* GetPetStableConst(Player* player)
        {
            return player->GetPetStable();
        }

        /**
         *
         */
        inline PetStable& GetOrInitPetStable(Player* player)
        {
            return player->GetOrInitPetStable();
        }

        /**
         *
         */
        inline Pet* GetPet(Player* player)
        {
            return player->GetPet();
        }

        /**
         *
         */
        inline Pet* SummonPet(Player* player, uint32 entry, float x, float y, float z, float ang, PetType petType, Milliseconds duration, uint32 spellId)
        {
            return player->SummonPet(entry, x, y, z, ang, petType, duration, spellId);
        }

        /**
         *
         */
        inline void RemovePet(Player* player, Pet* pet, PetSaveMode mode, bool returnreagent)
        {
            player->RemovePet(pet, mode, returnreagent);
        }

        /**
         *
         */
        inline bool CanPetResurrect(Player* player)
        {
            return player->CanPetResurrect();
        }

        /**
         *
         */
        inline bool IsExistPet(Player* player)
        {
            return player->IsExistPet();
        }

        /**
         *
         */
        inline Pet* CreatePetFromCreature(Player* player, Creature* creature, uint32 spell_id)
        {
            return player->CreatePet(creature, spell_id);
        }

        /**
         *
         */
        inline Pet* CreatePetFromEntry(Player* player, uint32 entry, uint32 spell_id)
        {
            return player->CreatePet(entry, spell_id);
        }

        /**
         *
         */
        inline uint32 GetPhaseMaskForSpawn(Player* player)
        {
            return player->GetPhaseMaskForSpawn();
        }

        /**
         *
         */
        inline void SetVirtualItemSlot(Player* player, uint8 i, Item* item)
        {
            player->SetVirtualItemSlot(i, item);
        }

        /**
         *
         */
        inline uint8 FindEquipSlot(Player* player, const ItemTemplate* proto, uint32 slot, bool swap)
        {
            return player->FindEquipSlot(proto, slot, swap);
        }

        /**
         *
         */
        inline uint32 GetItemCount(Player* player, uint32 item, bool inBankAlso, Item* skipItem)
        {
            return player->GetItemCount(item, inBankAlso, skipItem);
        }

        /**
         *
         */
        inline uint32 GetItemCountWithLimitCategory(Player* player, uint32 limitCategory, Item* skipItem)
        {
            return player->GetItemCountWithLimitCategory(limitCategory, skipItem);
        }

        /**
         *
         */
        inline Item* GetItemByGuid(Player* player, ObjectGuid itemGuid)
        {
            return player->GetItemByGuid(itemGuid);
        }

        /**
         *
         */
        inline Item* GetItemByEntry(Player* player, uint32 item)
        {
            return player->GetItemByEntry(item);
        }

        /**
         *
         */
        inline Item* GetItemByPosSlot(Player* player, uint16 pos)
        {
            return player->GetItemByPos(pos);
        }

        /**
         *
         */
        inline Item* GetItemByPosBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            return player->GetItemByPos(bag, slot);
        }

        /**
         *
         */
        inline Bag* GetBagByPos(Player* player, uint8 slot)
        {
            return player->GetBagByPos(slot);
        }

        /**
         *
         */
        inline uint32 GetFreeInventorySpace(Player* player)
        {
            return player->GetFreeInventorySpace();
        }

        /**
         *
         */
        inline Item* GetUseableItemByPos(Player* player, uint8 bag, uint8 slot)
        {
            return player->GetUseableItemByPos(bag, slot);
        }

        /**
         *
         */
        inline Item* GetWeaponForAttack(Player* player, WeaponAttackType attackType, bool useable)
        {
            return player->GetWeaponForAttack(attackType, useable);
        }

        /**
         *
         */
        inline bool HasWeapon(Player* player, WeaponAttackType attackType)
        {
            return player->HasWeapon(attackType);
        }

        /**
         *
         */
        inline bool HasWeaponForAttack(Player* player, WeaponAttackType attackType)
        {
            return player->HasWeaponForAttack(attackType);
        }

        /**
         *
         */
        inline Item* GetShield(Player* player, bool useable)
        {
            return player->GetShield(useable);
        }

        /**
         *
         */
        inline uint8 GetAttackBySlot(Player* player, uint8 slot)
        {
            (void)player; // unused parameter
            return Player::GetAttackBySlot(slot);
        }

        /**
         *
         */
        inline std::vector<Item*>& GetItemUpdateQueue(Player* player)
        {
            return player->GetItemUpdateQueue();
        }

        /**
         *
         */
        inline bool IsInventoryPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsInventoryPos(pos);
        }

        /**
         *
         */
        inline bool IsInventoryPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsInventoryPos(bag, slot);
        }

        /**
         *
         */
        inline bool IsEquipmentPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsEquipmentPos(pos);
        }

        /**
         *
         */
        inline bool IsEquipmentPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsEquipmentPos(bag, slot);
        }

        /**
         *
         */
        inline bool IsBagPos(Player* player, uint8 pos)
        {
            return player->IsBagPos(pos);
        }

        /**
         *
         */
        inline bool IsBankPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsBankPos(pos);
        }

        /**
         *
         */
        inline bool IsBankPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsBankPos(bag, slot);
        }

        /**
         *
         */
        inline bool IsValidPosBySlot(Player* player, uint16 pos, bool explicit_pos)
        {
            return player->IsValidPos(pos, explicit_pos);
        }

        /**
         *
         */
        inline bool IsValidPosByBagSlot(Player* player, uint8 bag, uint8 slot, bool explicit_pos)
        {
            return player->IsValidPos(bag, slot, explicit_pos);
        }

        /**
         *
         */
        inline uint8 GetBankBagSlotCount(Player* player)
        {
            return player->GetBankBagSlotCount();
        }

        /**
         *
         */
        inline void SetBankBagSlotCount(Player* player, uint8 count)
        {
            player->SetBankBagSlotCount(count);
        }

        /**
         *
         */
        inline bool HasItemCount(Player* player, uint32 item, uint32 count, bool inBankAlso)
        {
            return player->HasItemCount(item, count, inBankAlso);
        }

        /**
         *
         */
        inline bool HasItemFitToSpellRequirements(Player* player, const SpellInfo* spellInfo, const Item* ignoreItem)
        {
            return player->HasItemFitToSpellRequirements(spellInfo, ignoreItem);
        }

        /**
         *
         */
        inline bool CanNoReagentCast(Player* player, const SpellInfo* spellInfo)
        {
            return player->CanNoReagentCast(spellInfo);
        }

        /**
         *
         */
        inline bool HasItemOrGemWithIdEquipped(Player* player, uint32 item, uint32 count, uint8 except_slot)
        {
            return player->HasItemOrGemWithIdEquipped(item, count, except_slot);
        }

        /**
         *
         */
        inline bool HasItemOrGemWithLimitCategoryEquipped(Player* player, uint32 limitCategory, uint32 count, uint8 except_slot)
        {
            return player->HasItemOrGemWithLimitCategoryEquipped(limitCategory, count, except_slot);
        }

        /**
         *
         */
        inline uint8 CanTakeMoreSimilarItemsByItem(Player* player, Item* pItem)
        {
            return static_cast<uint8>(player->CanTakeMoreSimilarItems(pItem));
        }

        /**
         *
         */
        inline uint8 CanTakeMoreSimilarItemsByEntry(Player* player, uint32 entry, uint32 count)
        {
            return static_cast<uint8>(player->CanTakeMoreSimilarItems(entry, count));
        }

        /**
         *
         */
        inline std::tuple<uint8, uint32, sol::table> CanStoreNewItem(Player* player, uint8 bag, uint8 slot, uint32 entry, uint32 count, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            uint32 no_space_count = 0;
            uint8 result = static_cast<uint8>(player->CanStoreNewItem(bag, slot, dest, entry, count, &no_space_count));

            sol::table dest_table = lua.create_table();
            for (size_t i = 0; i < dest.size(); ++i) {
                sol::table pos_info = lua.create_table();
                pos_info["pos"] = dest[i].pos;
                pos_info["count"] = dest[i].count;
                dest_table[i + 1] = pos_info;
            }

            return std::make_tuple(result, no_space_count, dest_table);
        }

        /**
         *
         */
        inline std::tuple<uint8, sol::table> CanStoreItem(Player* player, uint8 bag, uint8 slot, Item* pItem, bool swap, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            uint8 result = static_cast<uint8>(player->CanStoreItem(bag, slot, dest, pItem, swap));

            sol::table dest_table = lua.create_table();
            for (size_t i = 0; i < dest.size(); ++i) {
                sol::table pos_info = lua.create_table();
                pos_info["pos"] = dest[i].pos;
                pos_info["count"] = dest[i].count;
                dest_table[i + 1] = pos_info;
            }

            return std::make_tuple(result, dest_table);
        }

        /**
         *
         */
        inline std::tuple<uint8, sol::table> CanStoreItemWithEntry(Player* player, uint8 bag, uint8 slot, uint32 pItem, bool swap, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            uint8 result = static_cast<uint8>(player->CanStoreItem(bag, slot, dest, pItem, swap));

            sol::table dest_table = lua.create_table();
            for (size_t i = 0; i < dest.size(); ++i) {
                sol::table pos_info = lua.create_table();
                pos_info["pos"] = dest[i].pos;
                pos_info["count"] = dest[i].count;
                dest_table[i + 1] = pos_info;
            }

            return std::make_tuple(result, dest_table);
        }

        /**
         *
         */
        inline uint8 CanStoreItems(Player* player, const std::vector<Item*>& items, int count)
        {
            std::vector<Item*> item_ptrs = items;
            return static_cast<uint8>(player->CanStoreItems(item_ptrs.data(), count));
        }

        /**
         *
         */
        inline std::tuple<uint8, uint16> CanEquipNewItem(Player* player, uint8 slot, uint32 item, bool swap)
        {
            uint16 dest = 0;
            uint8 result = static_cast<uint8>(player->CanEquipNewItem(slot, dest, item, swap));
            return std::make_tuple(result, dest);
        }

        /**
         *
         */
        inline std::tuple<uint8, uint16> CanEquipItem(Player* player, uint8 slot, Item* pItem, bool swap, bool not_loading)
        {
            uint16 dest = 0;
            uint8 result = static_cast<uint8>(player->CanEquipItem(slot, dest, pItem, swap, not_loading));
            return std::make_tuple(result, dest);
        }

        /**
         *
         */
        inline uint8 CanEquipUniqueItem(Player* player, Item* pItem, uint8 except_slot, uint32 limit_count)
        {
            return static_cast<uint8>(player->CanEquipUniqueItem(pItem, except_slot, limit_count));
        }

        /**
         *
         */
        inline uint8 CanEquipUniqueItemTemplate(Player* player, ItemTemplate const* itemProto, uint8 except_slot, uint32 limit_count)
        {
            return static_cast<uint8>(player->CanEquipUniqueItem(itemProto, except_slot, limit_count));
        }

        /**
         *
         */
        inline uint8 CanUnequipItems(Player* player, uint32 item, uint32 count)
        {
            return static_cast<uint8>(player->CanUnequipItems(item, count));
        }

        /**
         *
         */
        inline uint8 CanUnequipItem(Player* player, uint16 src, bool swap)
        {
            return static_cast<uint8>(player->CanUnequipItem(src, swap));
        }

        /**
         *
         */
        inline std::tuple<uint8, sol::table> CanBankItem(Player* player, uint8 bag, uint8 slot, Item* pItem, bool swap, bool not_loading, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            uint8 result = static_cast<uint8>(player->CanBankItem(bag, slot, dest, pItem, swap, not_loading));

            sol::table dest_table = lua.create_table();
            for (size_t i = 0; i < dest.size(); ++i) {
                sol::table pos_info = lua.create_table();
                pos_info["pos"] = dest[i].pos;
                pos_info["count"] = dest[i].count;
                dest_table[i + 1] = pos_info;
            }

            return std::make_tuple(result, dest_table);
        }

        /**
         *
         */
        inline uint8 CanUseItem(Player* player, Item* pItem, bool not_loading)
        {
            return static_cast<uint8>(player->CanUseItem(pItem, not_loading));
        }

        /**
         *
         */
        inline uint8 CanUseItemByTemplate(Player* player, ItemTemplate const* pItem)
        {
            return static_cast<uint8>(player->CanUseItem(pItem));
        }

        /**
         *
         */
        inline bool HasItemTotemCategory(Player* player, uint32 totemCategory)
        {
            return player->HasItemTotemCategory(totemCategory);
        }

        /**
         *
         */
        inline bool IsTotemCategoryCompatiableWith(Player* player, ItemTemplate const* pProto, uint32 requiredTotemCategoryId)
        {
            return player->IsTotemCategoryCompatiableWith(pProto, requiredTotemCategoryId);
        }

        /**
         *
         */
        inline uint8 CanUseAmmo(Player* player, uint32 item)
        {
            return static_cast<uint8>(player->CanUseAmmo(item));
        }

        /**
         *
         */
        inline uint8 CanRollForItemInLFG(Player* player, ItemTemplate const* item, WorldObject const* lootedObject)
        {
            return static_cast<uint8>(player->CanRollForItemInLFG(item, lootedObject));
        }

        /**
         *
         */
        inline Item* StoreNewItem(Player* player, sol::table pos_table, uint32 item, bool update, int32 randomPropertyId, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec pos;
            for (auto& pair : pos_table) {
                if (pair.second.is<sol::table>()) {
                    sol::table pos_info = pair.second;
                    uint16 pos_val = pos_info["pos"];
                    uint32 count_val = pos_info["count"];
                    pos.push_back(ItemPosCount(pos_val, count_val));
                }
            }

            return player->StoreNewItem(pos, item, update, randomPropertyId);
        }

        /**
         *
         */
        inline Item* StoreNewItemWithAllowedLooters(Player* player, sol::table pos_table, uint32 item, bool update, int32 randomPropertyId, sol::table allowedLooters_table, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec pos;
            for (auto& pair : pos_table) {
                if (pair.second.is<sol::table>()) {
                    sol::table pos_info = pair.second;
                    uint16 pos_val = pos_info["pos"];
                    uint32 count_val = pos_info["count"];
                    pos.push_back(ItemPosCount(pos_val, count_val));
                }
            }

            AllowedLooterSet allowedLooters;
            for (auto& pair : allowedLooters_table) {
                if (pair.second.is<ObjectGuid>()) {
                    allowedLooters.insert(pair.second.as<ObjectGuid>());
                }
            }

            return player->StoreNewItem(pos, item, update, randomPropertyId, allowedLooters);
        }

        /**
         *
         */
        inline Item* StoreItem(Player* player, sol::table pos_table, Item* pItem, bool update, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec pos;
            for (auto& pair : pos_table) {
                if (pair.second.is<sol::table>()) {
                    sol::table pos_info = pair.second;
                    uint16 pos_val = pos_info["pos"];
                    uint32 count_val = pos_info["count"];
                    pos.push_back(ItemPosCount(pos_val, count_val));
                }
            }

            return player->StoreItem(pos, pItem, update);
        }

        /**
         *
         */
        inline Item* EquipNewItem(Player* player, uint16 pos, uint32 item, bool update)
        {
            return player->EquipNewItem(pos, item, update);
        }

        /**
         *
         */
        inline Item* EquipItem(Player* player, uint16 pos, Item* pItem, bool update)
        {
            return player->EquipItem(pos, pItem, update);
        }

        /**
         *
         */
        inline void AutoUnequipOffhandIfNeed(Player* player, bool force)
        {
            player->AutoUnequipOffhandIfNeed(force);
        }

        /**
         *
         */
        inline bool StoreNewItemInBestSlots(Player* player, uint32 item_id, uint32 item_count)
        {
            return player->StoreNewItemInBestSlots(item_id, item_count);
        }

        /**
         *
         */
        inline void AutoStoreLootWithBagSlot(Player* player, uint8 bag, uint8 slot, uint32 loot_id, LootStore const& store, bool broadcast)
        {
            player->AutoStoreLoot(bag, slot, loot_id, store, broadcast);
        }

        /**
         *
         */
        inline void AutoStoreLoot(Player* player, uint32 loot_id, LootStore const& store, bool broadcast)
        {
            player->AutoStoreLoot(loot_id, store, broadcast);
        }

        /**
         *
         */
        inline std::tuple<LootItem*, uint8> StoreLootItem(Player* player, uint8 lootSlot, Loot* loot)
        {
            InventoryResult msg = EQUIP_ERR_OK;
            LootItem* result = player->StoreLootItem(lootSlot, loot, msg);
            return std::make_tuple(result, static_cast<uint8>(msg));
        }

        /**
         *
         */
        inline void UpdateLootAchievements(Player* player, LootItem* item, Loot* loot)
        {
            player->UpdateLootAchievements(item, loot);
        }

        /**
         *
         */
        inline void UpdateTitansGrip(Player* player)
        {
            player->UpdateTitansGrip();
        }

        /**
         * Check if player can store item in specific bag and slot
         */
        inline uint8 CanStoreItemInSlot(Player* player, uint8 bag, uint8 slot, uint32 entry, uint32 count, Item* pItem, bool swap)
        {
            ItemPosCountVec dest;
            return static_cast<uint8>(player->CanStoreItem(bag, slot, dest, entry, count, pItem, swap, nullptr));
        }

        /**
         * Check if player can store item anywhere
         */
        inline uint8 CanStoreItemAnywhere(Player* player, uint32 entry, uint32 count, Item* pItem, bool swap)
        {
            ItemPosCountVec dest;
            return static_cast<uint8>(player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, entry, count, pItem, swap, nullptr));
        }

        /**
         * Add refund reference for item
         */
        inline void AddRefundReference(Player* player, ObjectGuid itemGUID)
        {
            player->AddRefundReference(itemGUID);
        }

        /**
         * Delete refund reference for item
         */
        inline void DeleteRefundReference(Player* player, ObjectGuid itemGUID)
        {
            player->DeleteRefundReference(itemGUID);
        }

        /**
         * Apply equip cooldown for item
         */
        inline void ApplyEquipCooldown(Player* player, Item* pItem)
        {
            player->ApplyEquipCooldown(pItem);
        }

        /**
         * Set ammo
         */
        inline void SetAmmo(Player* player, uint32 item)
        {
            player->SetAmmo(item);
        }

        /**
         * Remove ammo
         */
        inline void RemoveAmmo(Player* player)
        {
            player->RemoveAmmo();
        }

        /**
         * Get ammo DPS
         */
        inline float GetAmmoDPS(Player* player)
        {
            return player->GetAmmoDPS();
        }

        /**
         * Check ammo compatibility
         */
        inline bool CheckAmmoCompatibility(Player* player, ItemTemplate const* ammo_proto)
        {
            return player->CheckAmmoCompatibility(ammo_proto);
        }

        /**
         * Quick equip item
         */
        inline void QuickEquipItem(Player* player, uint16 pos, Item* pItem)
        {
            player->QuickEquipItem(pos, pItem);
        }

        /**
         * Visualize item
         */
        inline void VisualizeItem(Player* player, uint8 slot, Item* pItem)
        {
            player->VisualizeItem(slot, pItem);
        }

        /**
         * Set visible item slot
         */
        inline void SetVisibleItemSlot(Player* player, uint8 slot, Item* pItem)
        {
            player->SetVisibleItemSlot(slot, pItem);
        }

        /**
         * Bank item at position
         */
        inline Item* BankItem(Player* player, Item* pItem, uint32 count, bool update, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            return player->BankItem(dest, pItem, update);
        }

        /**
         * Remove item from bag/slot
         */
        inline void RemoveItem(Player* player, uint8 bag, uint8 slot, bool update, bool swap)
        {
            player->RemoveItem(bag, slot, update, swap);
        }

        /**
         * Remove item from bag/slot without swap
         */
        inline void RemoveItemSimple(Player* player, uint8 bag, uint8 slot, bool update)
        {
            player->RemoveItem(bag, slot, update, false);
        }

        /**
         * Move item from inventory
         */
        inline void MoveItemFromInventory(Player* player, uint8 bag, uint8 slot, bool update)
        {
            player->MoveItemFromInventory(bag, slot, update);
        }



        // ========== LUA REGISTRATION ==========
        template<typename T>
        void RegisterPlayerMethods(sol::usertype<T>& type)
        {

            // Getters
            type["GetSummonExpireTimer"] = &GetSummonExpireTimer;
            type["GetPlayerFlags"] = &GetPlayerFlags;
            type["GetNPCIfCanInteractWith"] = &GetNPCIfCanInteractWith;
            type["GetGameObjectIfCanInteractWith"] = &GetGameObjectIfCanInteractWith;
            type["GetChatTag"] = &GetChatTag;
            type["GetBarberShopCost"] = &GetBarberShopCost;
            type["GetSocial"] = &GetSocial;
            type["GetCommandStatus"] = &GetCommandStatus;
            type["GetTotalPlayedTime"] = &GetTotalPlayedTime;
            type["GetLevelPlayedTime"] = &GetLevelPlayedTime;
            type["GetXPRestBonus"] = &GetXPRestBonus;
            type["GetRestBonus"] = &GetRestBonus;
            type["GetInnTriggerId"] = &GetInnTriggerId;
            type["GetPetStable"] = sol::overload(&GetPetStable, &GetPetStableConst);
            type["GetOrInitPetStable"] = &GetOrInitPetStable;
            type["GetPet"] = &GetPet;
            type["GetPhaseMaskForSpawn"] = &GetPhaseMaskForSpawn;
            type["GetItemCount"] = &GetItemCount;
            type["GetItemCountWithLimitCategory"] = &GetItemCountWithLimitCategory;
            type["GetItemByGuid"] = &GetItemByGuid;
            type["GetItemByEntry"] = &GetItemByEntry;
            type["GetItemByPos"] = sol::overload(&GetItemByPosSlot, &GetItemByPosBagSlot);
            type["GetBagByPos"] = &GetBagByPos;
            type["GetFreeInventorySpace"] = &GetFreeInventorySpace;
            type["GetUseableItemByPos"] = &GetUseableItemByPos;
            type["GetWeaponForAttack"] = &GetWeaponForAttack;
            type["GetShield"] = &GetShield;
            type["GetAttackBySlot"] = &GetAttackBySlot;
            type["GetItemUpdateQueue"] = &GetItemUpdateQueue;
            type["GetBankBagSlotCount"] = &GetBankBagSlotCount;
            type["GetAmmoDPS"] = &GetAmmoDPS;

            // Setters
            type["SetSummonPoint"] = &SetSummonPoint;
            type["SetSummonAsSpectator"] = &SetSummonAsSpectator;
            type["SetPlayerFlag"] = &SetPlayerFlag;
            type["SetInWater"] = &SetInWater;
            type["SetBankBagSlotCount"] = &SetBankBagSlotCount;
            type["SetCommentator"] = &SetCommentator;
            type["SetDeveloper"] = &SetDeveloper;
            type["SetAcceptWhispers"] = &SetAcceptWhispers;
            type["SetGameMaster"] = &SetGameMaster;
            type["SetGMChat"] = &SetGMChat;
            type["SetGMSpectator"] = &SetGMSpectator;
            type["SetTaxiCheater"] = &SetTaxiCheater;
            type["SetGMVisible"] = &SetGMVisible;
            type["SetHas310Flyer"] = &SetHas310Flyer;
            type["SetPvPDeath"] = &SetPvPDeath;
            type["SetCommandStatusOn"] = &SetCommandStatusOn;
            type["SetCommandStatusOff"] = &SetCommandStatusOff;
            type["SetDeathState"] = &SetDeathState;
            type["SetRestState"] = &SetRestState;
            type["SetRestBonus"] = &SetRestBonus;
            type["SetRestFlag"] = &SetRestFlag;
            type["SetVirtualItemSlot"] = &SetVirtualItemSlot;
            type["SetAmmo"] = &SetAmmo;
            type["SetVisibleItemSlot"] = &SetVisibleItemSlot;

            // Booleans
            type["IsSummonAsSpectator"] = &IsSummonAsSpectator;
            type["HasPlayerFlag"] = &HasPlayerFlag;
            type["IsClass"] = &IsClass;
            type["IsInWater"] = &IsInWater;
            type["IsFalling"] = &IsFalling;
            type["IsInAreaTriggerRadius"] = &IsInAreaTriggerRadius;
            type["CanInteractWithQuestGiver"] = &CanInteractWithQuestGiver;
            type["IsAFK"] = &IsAFK;
            type["IsDND"] = &IsDND;
            type["IsCommentator"] = &IsCommentator;
            type["IsDeveloper"] = &IsDeveloper;
            type["IsAcceptWhispers"] = &IsAcceptWhispers;
            type["IsGameMaster"] = &IsGameMaster;
            type["IsGMChat"] = &IsGMChat;
            type["IsGMSpectator"] = &IsGMSpectator;
            type["IsTaxiCheater"] = &IsTaxiCheater;
            type["IsGMVisible"] = &IsGMVisible;
            type["Has310Flyer"] = &Has310Flyer;
            type["HasActivePowerType"] = &HasActivePowerType;
            type["HasRestFlag"] = &HasRestFlag;
            type["CanPetResurrect"] = &CanPetResurrect;
            type["IsExistPet"] = &IsExistPet;
            type["HasWeapon"] = &HasWeapon;
            type["HasWeaponForAttack"] = &HasWeaponForAttack;
            type["IsInventoryPos"] = sol::overload(&IsInventoryPosBySlot, &IsInventoryPosByBagSlot);
            type["IsEquipmentPos"] = sol::overload(&IsEquipmentPosBySlot, &IsEquipmentPosByBagSlot);
            type["IsBagPos"] = &IsBagPos;
            type["IsBankPos"] = sol::overload(&IsBankPosBySlot, &IsBankPosByBagSlot);
            type["IsValidPos"] = sol::overload(&IsValidPosBySlot, &IsValidPosByBagSlot);
            type["HasItemCount"] = &HasItemCount;
            type["HasItemFitToSpellRequirements"] = &HasItemFitToSpellRequirements;
            type["CanNoReagentCast"] = &CanNoReagentCast;
            type["HasItemOrGemWithIdEquipped"] = &HasItemOrGemWithIdEquipped;
            type["HasItemOrGemWithLimitCategoryEquipped"] = &HasItemOrGemWithLimitCategoryEquipped;
            type["CanTakeMoreSimilarItems"] = sol::overload(&CanTakeMoreSimilarItemsByItem, &CanTakeMoreSimilarItemsByEntry);
            type["CanStoreNewItem"] = &CanStoreNewItem;
            type["CanStoreItem"] = sol::overload(&CanStoreItem, &CanStoreItemWithEntry, &CanStoreItemInSlot, &CanStoreItemAnywhere);
            type["CanStoreItems"] = &CanStoreItems;
            type["CanBankItem"] = &CanBankItem;
            type["CanEquipNewItem"] = &CanEquipNewItem;
            type["CanEquipItem"] = &CanEquipItem;
            type["CanEquipUniqueItem"] = sol::overload(&CanEquipUniqueItem, &CanEquipUniqueItemTemplate);
            type["CanUnequipItems"] = &CanUnequipItems;
            type["CanUnequipItem"] = &CanUnequipItem;
            type["CanUseItem"] = sol::overload(&CanUseItem, &CanUseItemByTemplate);
            type["HasItemTotemCategory"] = &HasItemTotemCategory;
            type["IsTotemCategoryCompatiableWith"] = &IsTotemCategoryCompatiableWith;
            type["CanUseAmmo"] = &CanUseAmmo;
            type["CanRollForItemInLFG"] = &CanRollForItemInLFG;

            // Actions
            type["TeleportTo"] = &TeleportTo;
            type["TeleportToEntryPoint"] = &TeleportToEntryPoint;
            type["SummonIfPossible"] = &SummonIfPossible;
            type["RemovePlayerFlag"] = &RemovePlayerFlag;
            type["ReplaceAllPlayerFlags"] = &ReplaceAllPlayerFlags;
            type["ToggleAFK"] = &ToggleAFK;
            type["ToggleDND"] = &ToggleDND;
            type["InitTaxiNodesForLevel"] = &InitTaxiNodesForLevel;
            type["ActivateTaxiPathTo"] = sol::overload(&ActivateTaxiPathToByVector, &ActivateTaxiPathToByNodes);
            type["CleanupAfterTaxiFlight"] = &CleanupAfterTaxiFlight;
            type["ContinueTaxiFlight"] = &ContinueTaxiFlight;
            type["SendTaxiNodeStatusMultiple"] = &SendTaxiNodeStatusMultiple;
            type["GiveXP"] = &GiveXP;
            type["GiveLevel"] = &GiveLevel;
            type["InitStatsForLevel"] = &InitStatsForLevel;
            type["RemoveRestState"] = &RemoveRestState;
            type["RemoveRestFlag"] = &RemoveRestFlag;
            type["SummonPet"] = &SummonPet;
            type["RemovePet"] = &RemovePet;
            type["CreatePet"] = sol::overload(&CreatePetFromCreature, &CreatePetFromEntry);
            type["StoreNewItem"] = sol::overload(&StoreNewItem, &StoreNewItemWithAllowedLooters);
            type["StoreItem"] = &StoreItem;
            type["EquipNewItem"] = &EquipNewItem;
            type["EquipItem"] = &EquipItem;
            type["AutoUnequipOffhandIfNeed"] = &AutoUnequipOffhandIfNeed;
            type["StoreNewItemInBestSlots"] = &StoreNewItemInBestSlots;
            type["AutoStoreLoot"] = sol::overload(&AutoStoreLoot, &AutoStoreLootWithBagSlot);
            type["StoreLootItem"] = &StoreLootItem;
            type["UpdateLootAchievements"] = &UpdateLootAchievements;
            type["UpdateTitansGrip"] = &UpdateTitansGrip;
            type["AddRefundReference"] = &AddRefundReference;
            type["DeleteRefundReference"] = &DeleteRefundReference;
            type["FindEquipSlot"] = &FindEquipSlot;
            type["ApplyEquipCooldown"] = &ApplyEquipCooldown;
            type["RemoveAmmo"] = &RemoveAmmo;
            type["CheckAmmoCompatibility"] = &CheckAmmoCompatibility;
            type["QuickEquipItem"] = &QuickEquipItem;
            type["VisualizeItem"] = &VisualizeItem;
            type["BankItem"] = &BankItem;
            type["RemoveItem"] = sol::overload(&RemoveItem, &RemoveItemSimple);
            type["MoveItemFromInventory"] = &MoveItemFromInventory;
        }
    }
}

#endif // ECLIPSE_PLAYER_METHODS_HPP