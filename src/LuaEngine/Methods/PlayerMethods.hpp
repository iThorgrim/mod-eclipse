#ifndef ECLIPSE_PLAYER_METHODS_HPP
#define ECLIPSE_PLAYER_METHODS_HPP

#include "EclipseIncludes.hpp"
#include "Pet.h"
#include "SocialMgr.h"

namespace Eclipse
{
    namespace PlayerMethods
    {

        // ========== WRAPPER FUNCTIONS ==========

        // Static method wrappers - consistent player:Method() API

        /**
         *
         */
        bool TeleportTo(Player* player, uint32 mapId, float x, float y, float z, float orientation, uint32 options, Unit* target, bool forced)
        {
            return player->TeleportTo(mapId, x, y, z, orientation, options, target, forced);
        }

        /**
         *
         */
        bool TeleportToEntryPoint(Player* player)
        {
            return player->TeleportToEntryPoint();
        }

        /**
         *
         */
        void SetSummonPoint(Player* player, uint32 mapId, float x, float y, float z, uint32 zoneId, bool asSpectator)
        {
            player->SetSummonPoint(mapId, x, y, z, zoneId, asSpectator);
        }

        /**
         *
         */
        bool IsSummonAsSpectator(Player* player)
        {
            return player->IsSummonAsSpectator();
        }

        /**
         *
         */
        void SetSummonAsSpectator(Player* player, bool spectator)
        {
            player->SetSummonAsSpectator(spectator);
        }

        /**
         *
         */
        void SummonIfPossible(Player* player, bool asSpectator, ObjectGuid summoner)
        {
            player->SummonIfPossible(asSpectator, summoner);
        }

        /**
         *
         */
        time_t GetSummonExpireTimer(Player* player)
        {
            return player->GetSummonExpireTimer();
        }

        /**
         *
         */
        PlayerFlags GetPlayerFlags(Player* player)
        {
            return player->GetPlayerFlags();
        }

        /**
         *
         */
        bool HasPlayerFlag(Player* player, PlayerFlags flag)
        {
            return player->HasPlayerFlag(flag);
        }

        /**
         *
         */
        void SetPlayerFlag(Player* player, PlayerFlags flag)
        {
            player->SetPlayerFlag(flag);
        }

        /**
         *
         */
        void RemovePlayerFlag(Player* player, PlayerFlags flag)
        {
            player->RemovePlayerFlag(flag);
        }

        /**
         *
         */
        void ReplaceAllPlayerFlags(Player* player, PlayerFlags flags)
        {
            player->ReplaceAllPlayerFlags(flags);
        }

        /**
         *
         */
        bool IsClass(Player* player, Classes clss, ClassContext context)
        {
            return player->IsClass(clss, context);
        }

        /**
         *
         */
        void SetInWater(Player* player, bool apply)
        {
            player->SetInWater(apply);
        }

        /**
         *
         */
        bool IsInWater(Player* player)
        {
            return player->IsInWater();
        }

        /**
         *
         */
        bool IsFalling(Player* player)
        {
            return player->IsFalling();
        }

        /**
         *
         */
        bool IsInAreaTriggerRadius(Player* player, const AreaTrigger* at, float delta)
        {
            return player->IsInAreaTriggerRadius(at, delta);
        }

        /**
         *
         */
        bool CanInteractWithQuestGiver(Player* player, Object* questGiver)
        {
            return player->CanInteractWithQuestGiver(questGiver);
        }

        /**
         *
         */
        Creature* GetNPCIfCanInteractWith(Player* player, ObjectGuid guid, uint32 npcflagmask)
        {
            return player->GetNPCIfCanInteractWith(guid, npcflagmask);
        }

        /**
         *
         */
        GameObject* GetGameObjectIfCanInteractWith(Player* player, ObjectGuid guid, GameobjectTypes type)
        {
            return player->GetGameObjectIfCanInteractWith(guid, type);
        }

        /**
         *
         */
        void ToggleAFK(Player* player)
        {
            player->ToggleAFK();
        }

        /**
         *
         */
        void ToggleDND(Player* player)
        {
            player->ToggleDND();
        }

        /**
         *
         */
        bool IsAFK(Player* player)
        {
            return player->isAFK();
        }

        /**
         *
         */
        bool IsDND(Player* player)
        {
            return player->isDND();
        }

        /**
         *
         */
        uint8 GetChatTag(Player* player)
        {
            return player->GetChatTag();
        }

        /**
         *
         */
        uint32 GetBarberShopCost(Player* player, uint8 newHairStyle, uint8 newHairColor, uint8 newFacialHair, const BarberShopStyleEntry* newSkin)
        {
            return player->GetBarberShopCost(newHairStyle, newHairColor, newFacialHair, newSkin);
        }

        /**
         *
         */
        PlayerSocial* GetSocial(Player* player)
        {
            return player->GetSocial();
        }

        /**
         *
         */
        void InitTaxiNodesForLevel(Player* player)
        {
            player->InitTaxiNodesForLevel();
        }

        /**
         *
         */
        bool ActivateTaxiPathToByVector(Player* player, const std::vector<uint32>& nodes, Creature* npc, uint32 spellid)
        {
            return player->ActivateTaxiPathTo(nodes, npc, spellid);
        }

        /**
         *
         */
        bool ActivateTaxiPathToByNodes(Player* player, uint32 taxi_path_id, uint32 spellid)
        {
            return player->ActivateTaxiPathTo(taxi_path_id, spellid);
        }

        /**
         *
         */
        void CleanupAfterTaxiFlight(Player* player)
        {
            player->CleanupAfterTaxiFlight();
        }

        /**
         *
         */
        void ContinueTaxiFlight(Player* player)
        {
            player->ContinueTaxiFlight();
        }

        /**
         *
         */
        void SendTaxiNodeStatusMultiple(Player* player)
        {
            player->SendTaxiNodeStatusMultiple();
        }

        /**
         *
         */
        bool IsCommentator(Player* player)
        {
            return player->IsCommentator();
        }

        /**
         *
         */
        void SetCommentator(Player* player, bool on)
        {
            player->SetCommentator(on);
        }

        /**
         *
         */
        bool IsDeveloper(Player* player)
        {
            return player->IsDeveloper();
        }

        /**
         *
         */
        void SetDeveloper(Player* player, bool on)
        {
            player->SetDeveloper(on);
        }

        /**
         *
         */
        bool IsAcceptWhispers(Player* player)
        {
            return player->isAcceptWhispers();
        }

        /**
         *
         */
        void SetAcceptWhispers(Player* player, bool on)
        {
            player->SetAcceptWhispers(on);
        }

        /**
         *
         */
        bool IsGameMaster(Player* player)
        {
            return player->IsGameMaster();
        }

        /**
         *
         */
        void SetGameMaster(Player* player, bool on)
        {
            player->SetGameMaster(on);
        }

        /**
         *
         */
        bool IsGMChat(Player* player)
        {
            return player->isGMChat();
        }

        /**
         *
         */
        void SetGMChat(Player* player, bool on)
        {
            player->SetGMChat(on);
        }

        /**
         *
         */
        bool IsGMSpectator(Player* player)
        {
            return player->IsGMSpectator();
        }

        /**
         *
         */
        void SetGMSpectator(Player* player, bool on)
        {
            player->SetGMSpectator(on);
        }

        /**
         *
         */
        bool IsTaxiCheater(Player* player)
        {
            return player->isTaxiCheater();
        }

        /**
         *
         */
        void SetTaxiCheater(Player* player, bool on)
        {
            player->SetTaxiCheater(on);
        }

        /**
         *
         */
        bool IsGMVisible(Player* player)
        {
            return player->isGMVisible();
        }

        /**
         *
         */
        void SetGMVisible(Player* player, bool on)
        {
            player->SetGMVisible(on);
        }

        /**
         *
         */
        bool Has310Flyer(Player* player, bool checkAllSpells, uint32 excludeSpellId)
        {
            return player->Has310Flyer(checkAllSpells, excludeSpellId);
        }

        /**
         *
         */
        void SetHas310Flyer(Player* player, bool on)
        {
            player->SetHas310Flyer(on);
        }

        /**
         *
         */
        void SetPvPDeath(Player* player, bool on)
        {
            player->SetPvPDeath(on);
        }

        /**
         *
         */
        void GiveXP(Player* player, uint32 xp, Unit* victim, float group_rate, bool use_group_rate)
        {
            player->GiveXP(xp, victim, group_rate, use_group_rate);
        }

        /**
         *
         */
        void GiveLevel(Player* player, uint8 level)
        {
            player->GiveLevel(level);
        }

        /**
         *
         */
        void InitStatsForLevel(Player* player, bool reapplyMods)
        {
            player->InitStatsForLevel(reapplyMods);
        }

        /**
         *
         */
        bool HasActivePowerType(Player* player, Powers power)
        {
            return player->HasActivePowerType(power);
        }

        /**
         *
         */
        bool GetCommandStatus(Player* player, uint32 command)
        {
            return player->GetCommandStatus(command);
        }

        /**
         *
         */
        void SetCommandStatusOn(Player* player, uint32 command)
        {
            player->SetCommandStatusOn(command);
        }

        /**
         *
         */
        void SetCommandStatusOff(Player* player, uint32 command)
        {
            player->SetCommandStatusOff(command);
        }

        /**
         *
         */
        uint32 GetTotalPlayedTime(Player* player)
        {
            return player->GetTotalPlayedTime();
        }

        /**
         *
         */
        uint32 GetLevelPlayedTime(Player* player)
        {
            return player->GetLevelPlayedTime();
        }

        /**
         *
         */
        void SetDeathState(Player* player, DeathState s, bool despawn)
        {
            player->setDeathState(s, despawn);
        }

        /**
         *
         */
        void SetRestState(Player* player, uint32 triggerId)
        {
            player->SetRestState(triggerId);
        }

        /**
         *
         */
        void RemoveRestState(Player* player)
        {
            player->RemoveRestState();
        }

        /**
         *
         */
        uint32 GetXPRestBonus(Player* player, uint32 xp)
        {
            return player->GetXPRestBonus(xp);
        }

        /**
         *
         */
        float GetRestBonus(Player* player)
        {
            return player->GetRestBonus();
        }

        /**
         *
         */
        void SetRestBonus(Player* player, float rest_bonus_new)
        {
            player->SetRestBonus(rest_bonus_new);
        }

        /**
         *
         */
        bool HasRestFlag(Player* player, RestFlag flag)
        {
            return player->HasRestFlag(flag);
        }

        /**
         *
         */
        void SetRestFlag(Player* player, RestFlag flag, uint32 triggerId)
        {
            player->SetRestFlag(flag, triggerId);
        }

        /**
         *
         */
        void RemoveRestFlag(Player* player, RestFlag flag)
        {
            player->RemoveRestFlag(flag);
        }

        /**
         *
         */
        uint32 GetInnTriggerId(Player* player)
        {
            return player->GetInnTriggerId();
        }

        /**
         *
         */
        PetStable* GetPetStable(Player* player)
        {
            return player->GetPetStable();
        }

        /**
         *
         */
        const PetStable* GetPetStableConst(Player* player)
        {
            return player->GetPetStable();
        }

        /**
         *
         */
        PetStable& GetOrInitPetStable(Player* player)
        {
            return player->GetOrInitPetStable();
        }

        /**
         *
         */
        Pet* GetPet(Player* player)
        {
            return player->GetPet();
        }

        /**
         *
         */
        Pet* SummonPet(Player* player, uint32 entry, float x, float y, float z, float ang, PetType petType, Milliseconds duration, uint32 spellId)
        {
            return player->SummonPet(entry, x, y, z, ang, petType, duration, spellId);
        }

        /**
         *
         */
        void RemovePet(Player* player, Pet* pet, PetSaveMode mode, bool returnreagent)
        {
            player->RemovePet(pet, mode, returnreagent);
        }

        /**
         *
         */
        bool CanPetResurrect(Player* player)
        {
            return player->CanPetResurrect();
        }

        /**
         *
         */
        bool IsExistPet(Player* player)
        {
            return player->IsExistPet();
        }

        /**
         *
         */
        Pet* CreatePetFromCreature(Player* player, Creature* creature, uint32 spell_id)
        {
            return player->CreatePet(creature, spell_id);
        }

        /**
         *
         */
        Pet* CreatePetFromEntry(Player* player, uint32 entry, uint32 spell_id)
        {
            return player->CreatePet(entry, spell_id);
        }

        /**
         *
         */
        uint32 GetPhaseMaskForSpawn(Player* player)
        {
            return player->GetPhaseMaskForSpawn();
        }

        /**
         *
         */
        void SetVirtualItemSlot(Player* player, uint8 i, Item* item)
        {
            player->SetVirtualItemSlot(i, item);
        }

        /**
         *
         */
        uint8 FindEquipSlot(Player* player, const ItemTemplate* proto, uint32 slot, bool swap)
        {
            return player->FindEquipSlot(proto, slot, swap);
        }

        /**
         *
         */
        uint32 GetItemCount(Player* player, uint32 item, bool inBankAlso, Item* skipItem)
        {
            return player->GetItemCount(item, inBankAlso, skipItem);
        }

        /**
         *
         */
        uint32 GetItemCountWithLimitCategory(Player* player, uint32 limitCategory, Item* skipItem)
        {
            return player->GetItemCountWithLimitCategory(limitCategory, skipItem);
        }

        /**
         *
         */
        Item* GetItemByGuid(Player* player, ObjectGuid itemGuid)
        {
            return player->GetItemByGuid(itemGuid);
        }

        /**
         *
         */
        Item* GetItemByEntry(Player* player, uint32 item)
        {
            return player->GetItemByEntry(item);
        }

        /**
         *
         */
        Item* GetItemByPosSlot(Player* player, uint16 pos)
        {
            return player->GetItemByPos(pos);
        }

        /**
         *
         */
        Item* GetItemByPosBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            return player->GetItemByPos(bag, slot);
        }

        /**
         *
         */
        Bag* GetBagByPos(Player* player, uint8 slot)
        {
            return player->GetBagByPos(slot);
        }

        /**
         *
         */
        uint32 GetFreeInventorySpace(Player* player)
        {
            return player->GetFreeInventorySpace();
        }

        /**
         *
         */
        Item* GetUseableItemByPos(Player* player, uint8 bag, uint8 slot)
        {
            return player->GetUseableItemByPos(bag, slot);
        }

        /**
         *
         */
        Item* GetWeaponForAttack(Player* player, WeaponAttackType attackType, bool useable)
        {
            return player->GetWeaponForAttack(attackType, useable);
        }

        /**
         *
         */
        bool HasWeapon(Player* player, WeaponAttackType attackType)
        {
            return player->HasWeapon(attackType);
        }

        /**
         *
         */
        bool HasWeaponForAttack(Player* player, WeaponAttackType attackType)
        {
            return player->HasWeaponForAttack(attackType);
        }

        /**
         *
         */
        Item* GetShield(Player* player, bool useable)
        {
            return player->GetShield(useable);
        }

        /**
         *
         */
        uint8 GetAttackBySlot(Player* player, uint8 slot)
        {
            (void)player; // unused parameter
            return Player::GetAttackBySlot(slot);
        }

        /**
         *
         */
        std::vector<Item*>& GetItemUpdateQueue(Player* player)
        {
            return player->GetItemUpdateQueue();
        }

        /**
         *
         */
        bool IsInventoryPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsInventoryPos(pos);
        }

        /**
         *
         */
        bool IsInventoryPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsInventoryPos(bag, slot);
        }

        /**
         *
         */
        bool IsEquipmentPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsEquipmentPos(pos);
        }

        /**
         *
         */
        bool IsEquipmentPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsEquipmentPos(bag, slot);
        }

        /**
         *
         */
        bool IsBagPos(Player* player, uint8 pos)
        {
            return player->IsBagPos(pos);
        }

        /**
         *
         */
        bool IsBankPosBySlot(Player* player, uint16 pos)
        {
            (void)player; // Unused parameter
            return Player::IsBankPos(pos);
        }

        /**
         *
         */
        bool IsBankPosByBagSlot(Player* player, uint8 bag, uint8 slot)
        {
            (void)player; // Unused parameter
            return Player::IsBankPos(bag, slot);
        }

        /**
         *
         */
        bool IsValidPosBySlot(Player* player, uint16 pos, bool explicit_pos)
        {
            return player->IsValidPos(pos, explicit_pos);
        }

        /**
         *
         */
        bool IsValidPosByBagSlot(Player* player, uint8 bag, uint8 slot, bool explicit_pos)
        {
            return player->IsValidPos(bag, slot, explicit_pos);
        }

        /**
         *
         */
        uint8 GetBankBagSlotCount(Player* player)
        {
            return player->GetBankBagSlotCount();
        }

        /**
         *
         */
        void SetBankBagSlotCount(Player* player, uint8 count)
        {
            player->SetBankBagSlotCount(count);
        }

        /**
         *
         */
        bool HasItemCount(Player* player, uint32 item, uint32 count, bool inBankAlso)
        {
            return player->HasItemCount(item, count, inBankAlso);
        }

        /**
         *
         */
        bool HasItemFitToSpellRequirements(Player* player, const SpellInfo* spellInfo, const Item* ignoreItem)
        {
            return player->HasItemFitToSpellRequirements(spellInfo, ignoreItem);
        }

        /**
         *
         */
        bool CanNoReagentCast(Player* player, const SpellInfo* spellInfo)
        {
            return player->CanNoReagentCast(spellInfo);
        }

        /**
         *
         */
        bool HasItemOrGemWithIdEquipped(Player* player, uint32 item, uint32 count, uint8 except_slot)
        {
            return player->HasItemOrGemWithIdEquipped(item, count, except_slot);
        }

        /**
         *
         */
        bool HasItemOrGemWithLimitCategoryEquipped(Player* player, uint32 limitCategory, uint32 count, uint8 except_slot)
        {
            return player->HasItemOrGemWithLimitCategoryEquipped(limitCategory, count, except_slot);
        }

        /**
         *
         */
        InventoryResult CanTakeMoreSimilarItemsByItem(Player* player, Item* pItem)
        {
            return player->CanTakeMoreSimilarItems(pItem);
        }

        /**
         *
         */
        InventoryResult CanTakeMoreSimilarItemsByEntry(Player* player, uint32 entry, uint32 count)
        {
            return player->CanTakeMoreSimilarItems(entry, count);
        }

        /**
         *
         */
        std::tuple<InventoryResult, uint32, sol::table> CanStoreNewItem(Player* player, uint8 bag, uint8 slot, uint32 entry, uint32 count, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            uint32 no_space_count = 0;
            InventoryResult result = player->CanStoreNewItem(bag, slot, dest, entry, count, &no_space_count);

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
        std::tuple<InventoryResult, sol::table> CanStoreItem(Player* player, uint8 bag, uint8 slot, Item* pItem, bool swap, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            InventoryResult result = player->CanStoreItem(bag, slot, dest, pItem, swap);

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
        std::tuple<InventoryResult, sol::table> CanStoreItemWithEntry(Player* player, uint8 bag, uint8 slot, uint32 pItem, bool swap, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            InventoryResult result = player->CanStoreItem(bag, slot, dest, pItem, swap);

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
        InventoryResult CanStoreItems(Player* player, const std::vector<Item*>& items, int count)
        {
            std::vector<Item*> item_ptrs = items;
            return player->CanStoreItems(item_ptrs.data(), count);
        }

        /**
         *
         */
        std::tuple<InventoryResult, uint16> CanEquipNewItem(Player* player, uint8 slot, uint32 item, bool swap)
        {
            uint16 dest = 0;
            InventoryResult result = player->CanEquipNewItem(slot, dest, item, swap);
            return std::make_tuple(result, dest);
        }

        /**
         *
         */
        std::tuple<InventoryResult, uint16> CanEquipItem(Player* player, uint8 slot, Item* pItem, bool swap, bool not_loading)
        {
            uint16 dest = 0;
            InventoryResult result = player->CanEquipItem(slot, dest, pItem, swap, not_loading);
            return std::make_tuple(result, dest);
        }

        /**
         *
         */
        InventoryResult CanEquipUniqueItem(Player* player, Item* pItem, uint8 except_slot, uint32 limit_count)
        {
            return player->CanEquipUniqueItem(pItem, except_slot, limit_count);
        }

        /**
         *
         */
        InventoryResult CanEquipUniqueItemTemplate(Player* player, ItemTemplate const* itemProto, uint8 except_slot, uint32 limit_count)
        {
            return player->CanEquipUniqueItem(itemProto, except_slot, limit_count);
        }

        /**
         *
         */
        InventoryResult CanUnequipItems(Player* player, uint32 item, uint32 count)
        {
            return player->CanUnequipItems(item, count);
        }

        /**
         *
         */
        InventoryResult CanUnequipItem(Player* player, uint16 src, bool swap)
        {
            return player->CanUnequipItem(src, swap);
        }

        /**
         *
         */
        std::tuple<InventoryResult, sol::table> CanBankItem(Player* player, uint8 bag, uint8 slot, Item* pItem, bool swap, bool not_loading, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            InventoryResult result = player->CanBankItem(bag, slot, dest, pItem, swap, not_loading);

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
        InventoryResult CanUseItem(Player* player, Item* pItem, bool not_loading)
        {
            return player->CanUseItem(pItem, not_loading);
        }

        /**
         *
         */
        InventoryResult CanUseItemByTemplate(Player* player, ItemTemplate const* pItem)
        {
            return player->CanUseItem(pItem);
        }

        /**
         *
         */
        bool HasItemTotemCategory(Player* player, uint32 totemCategory)
        {
            return player->HasItemTotemCategory(totemCategory);
        }

        /**
         *
         */
        bool IsTotemCategoryCompatiableWith(Player* player, ItemTemplate const* pProto, uint32 requiredTotemCategoryId)
        {
            return player->IsTotemCategoryCompatiableWith(pProto, requiredTotemCategoryId);
        }

        /**
         *
         */
        InventoryResult CanUseAmmo(Player* player, uint32 item)
        {
            return player->CanUseAmmo(item);
        }

        /**
         *
         */
        InventoryResult CanRollForItemInLFG(Player* player, ItemTemplate const* item, WorldObject const* lootedObject)
        {
            return player->CanRollForItemInLFG(item, lootedObject);
        }

        /**
         *
         */
        Item* StoreNewItem(Player* player, sol::table pos_table, uint32 item, bool update, int32 randomPropertyId, sol::this_state s)
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
        Item* StoreNewItemWithAllowedLooters(Player* player, sol::table pos_table, uint32 item, bool update, int32 randomPropertyId, sol::table allowedLooters_table, sol::this_state s)
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
        Item* StoreItem(Player* player, sol::table pos_table, Item* pItem, bool update, sol::this_state s)
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
        Item* EquipNewItem(Player* player, uint16 pos, uint32 item, bool update)
        {
            return player->EquipNewItem(pos, item, update);
        }

        /**
         *
         */
        Item* EquipItem(Player* player, uint16 pos, Item* pItem, bool update)
        {
            return player->EquipItem(pos, pItem, update);
        }

        /**
         *
         */
        void AutoUnequipOffhandIfNeed(Player* player, bool force)
        {
            player->AutoUnequipOffhandIfNeed(force);
        }

        /**
         *
         */
        bool StoreNewItemInBestSlots(Player* player, uint32 item_id, uint32 item_count)
        {
            return player->StoreNewItemInBestSlots(item_id, item_count);
        }

        /**
         *
         */
        void AutoStoreLootWithBagSlot(Player* player, uint8 bag, uint8 slot, uint32 loot_id, LootStore const& store, bool broadcast)
        {
            player->AutoStoreLoot(bag, slot, loot_id, store, broadcast);
        }

        /**
         *
         */
        void AutoStoreLoot(Player* player, uint32 loot_id, LootStore const& store, bool broadcast)
        {
            player->AutoStoreLoot(loot_id, store, broadcast);
        }

        /**
         *
         */
        std::tuple<LootItem*, InventoryResult> StoreLootItem(Player* player, uint8 lootSlot, Loot* loot)
        {
            InventoryResult msg = EQUIP_ERR_OK;
            LootItem* result = player->StoreLootItem(lootSlot, loot, msg);
            return std::make_tuple(result, msg);
        }

        /**
         *
         */
        void UpdateLootAchievements(Player* player, LootItem* item, Loot* loot)
        {
            player->UpdateLootAchievements(item, loot);
        }

        /**
         *
         */
        void UpdateTitansGrip(Player* player)
        {
            player->UpdateTitansGrip();
        }

        /**
         * Check if player can store item in specific bag and slot
         */
        uint8 CanStoreItemInSlot(Player* player, uint8 bag, uint8 slot, uint32 entry, uint32 count, Item* pItem, bool swap)
        {
            ItemPosCountVec dest;
            return static_cast<uint8>(player->CanStoreItem(bag, slot, dest, entry, count, pItem, swap, nullptr));
        }

        /**
         * Check if player can store item anywhere
         */
        uint8 CanStoreItemAnywhere(Player* player, uint32 entry, uint32 count, Item* pItem, bool swap)
        {
            ItemPosCountVec dest;
            return static_cast<uint8>(player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, entry, count, pItem, swap, nullptr));
        }

        /**
         * Add refund reference for item
         */
        void AddRefundReference(Player* player, ObjectGuid itemGUID)
        {
            player->AddRefundReference(itemGUID);
        }

        /**
         * Delete refund reference for item
         */
        void DeleteRefundReference(Player* player, ObjectGuid itemGUID)
        {
            player->DeleteRefundReference(itemGUID);
        }

        /**
         * Apply equip cooldown for item
         */
        void ApplyEquipCooldown(Player* player, Item* pItem)
        {
            player->ApplyEquipCooldown(pItem);
        }

        /**
         * Set ammo
         */
        void SetAmmo(Player* player, uint32 item)
        {
            player->SetAmmo(item);
        }

        /**
         * Remove ammo
         */
        void RemoveAmmo(Player* player)
        {
            player->RemoveAmmo();
        }

        /**
         * Get ammo DPS
         */
        float GetAmmoDPS(Player* player)
        {
            return player->GetAmmoDPS();
        }

        /**
         * Check ammo compatibility
         */
        bool CheckAmmoCompatibility(Player* player, ItemTemplate const* ammo_proto)
        {
            return player->CheckAmmoCompatibility(ammo_proto);
        }

        /**
         * Quick equip item
         */
        void QuickEquipItem(Player* player, uint16 pos, Item* pItem)
        {
            player->QuickEquipItem(pos, pItem);
        }

        /**
         * Visualize item
         */
        void VisualizeItem(Player* player, uint8 slot, Item* pItem)
        {
            player->VisualizeItem(slot, pItem);
        }

        /**
         * Set visible item slot
         */
        void SetVisibleItemSlot(Player* player, uint8 slot, Item* pItem)
        {
            player->SetVisibleItemSlot(slot, pItem);
        }

        /**
         * Bank item at position
         */
        Item* BankItem(Player* player, Item* pItem, uint32 count, bool update, sol::this_state s)
        {
            sol::state_view lua(s);
            ItemPosCountVec dest;
            return player->BankItem(dest, pItem, update);
        }

        /**
         * Remove item from bag/slot
         */
        void RemoveItem(Player* player, uint8 bag, uint8 slot, bool update, bool swap)
        {
            player->RemoveItem(bag, slot, update, swap);
        }

        /**
         * Remove item from bag/slot without swap
         */
        void RemoveItemSimple(Player* player, uint8 bag, uint8 slot, bool update)
        {
            player->RemoveItem(bag, slot, update, false);
        }

        /**
         * Move item from inventory
         */
        void MoveItemFromInventory(Player* player, uint8 bag, uint8 slot, bool update)
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