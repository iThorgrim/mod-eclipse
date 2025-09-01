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
        auto CanStoreNewItemWrapper = [](Player* player, uint8 bag, uint8 slot, ItemPosCountVec& dest, uint32 entry, uint32 count) -> std::tuple<InventoryResult, uint32>
        {
            uint32 no_space_count = 0;
            InventoryResult result = player->CanStoreNewItem(bag, slot, dest, entry, count, &no_space_count);
            return std::make_tuple(result, no_space_count);
        };

        auto CanStoreItemsWrapper = [](Player* player, const std::vector<Item*>& items, int count) -> InventoryResult
        {
            std::vector<Item*> item_ptrs = items;
            return player->CanStoreItems(item_ptrs.data(), count);
        };

        // ========== LUA REGISTRATION ==========
        template<typename T>
        void RegisterPlayerMethods(sol::usertype<T>& type)
        {
            /**
             *
             */
            type["TeleportTo"] = static_cast<bool(Player::*)(uint32, float, float, float, float, uint32, Unit*, bool)>(&Player::TeleportTo);

            /**
             *
             */
            type["TeleportToEntryPoint"] = static_cast<bool(Player::*)()>(&Player::TeleportToEntryPoint);

            /**
             *
             */
            type["SetSummonPoint"] = static_cast<void(Player::*)(uint32, float, float, float, uint32, bool)>(&Player::SetSummonPoint);

            /**
             *
             */
            type["IsSummonAsSpectator"] = static_cast<bool(Player::*)() const>(&Player::IsSummonAsSpectator);

            /**
             *
             */
            type["SetSummonAsSpectator"] = static_cast<void(Player::*)(bool)>(&Player::SetSummonAsSpectator);

            /**
             *
             */
            type["SummonIfPossible"] = static_cast<void(Player::*)(bool, ObjectGuid)>(&Player::SummonIfPossible);

            /**
             *
             */
            type["GetSummonExpireTimer"] = static_cast<time_t(Player::*)() const>(&Player::GetSummonExpireTimer);

            /**
             *
             */
            type["GetPlayerFlags"] = static_cast<PlayerFlags(Player::*)() const>(&Player::GetPlayerFlags);

            /**
             *
             */
            type["HasPlayerFlag"] = static_cast<bool(Player::*)(PlayerFlags) const>(&Player::HasPlayerFlag);

            /**
             *
             */
            type["SetPlayerFlag"] = static_cast<void(Player::*)(PlayerFlags)>(&Player::SetPlayerFlag);

            /**
             *
             */
            type["RemovePlayerFlag"] = static_cast<void(Player::*)(PlayerFlags)>(&Player::RemovePlayerFlag);

            /**
             *
             */
            type["ReplaceAllPlayerFlags"] = static_cast<void(Player::*)(PlayerFlags)>(&Player::ReplaceAllPlayerFlags);

            /**
             *
             */
            type["IsClass"] = static_cast<bool(Player::*)(Classes, ClassContext) const>(&Player::IsClass);

            /**
             *
             */
            type["SetInWater"] = static_cast<void(Player::*)(bool)>(&Player::SetInWater);

            /**
             *
             */
            type["IsInWater"] = static_cast<bool(Player::*)() const>(&Player::IsInWater);

            /**
             *
             */
            type["IsFalling"] = static_cast<bool(Player::*)() const>(&Player::IsFalling);

            /**
             *
             */
            type["IsInAreaTriggerRadius"] = static_cast<bool(Player::*)(const AreaTrigger *, float) const>(&Player::IsInAreaTriggerRadius);

            /**
             *
             */
            type["CanInteractWithQuestGiver"] = static_cast<bool(Player::*)(Object*)>(&Player::CanInteractWithQuestGiver);

            /**
             *
             */
            type["GetNPCIfCanInteractWith"] = static_cast<Creature*(Player::*)(ObjectGuid, uint32)>(&Player::GetNPCIfCanInteractWith);

            /**
             *
             */
            type["GetGameObjectIfCanInteractWith"] = static_cast<GameObject*(Player::*)(ObjectGuid, GameobjectTypes) const>(&Player::GetGameObjectIfCanInteractWith);

            /**
             *
             */
            type["ToggleAFK"] = static_cast<void(Player::*)()>(&Player::ToggleAFK);

            /**
             *
             */
            type["ToggleDND"] = static_cast<void(Player::*)()>(&Player::ToggleDND);

            /**
             *
             */
            type["IsAFK"] = static_cast<bool(Player::*)() const>(&Player::isAFK);

            /**
             *
             */
            type["IsDND"] = static_cast<bool(Player::*)() const>(&Player::isDND);

            /**
             *
             */
            type["GetChatTag"] = static_cast<uint8(Player::*)() const>(&Player::GetChatTag);

            /**
             *
             */
            type["GetBarberShopCost"] = static_cast<uint32(Player::*)(uint8, uint8, uint8, const BarberShopStyleEntry*)>(&Player::GetBarberShopCost);

            /**
             *
             */
            type["GetSocial"] = static_cast<PlayerSocial*(Player::*)()>(&Player::GetSocial);

            /**
             *
             */
            type["InitTaxiNodesForLevel"] = static_cast<void(Player::*)()>(&Player::InitTaxiNodesForLevel);

            /**
             *
             */
            type["ActivateTaxiPathTo"] = sol::overload(
                static_cast<bool(Player::*)(const std::vector<uint32>&, Creature*, uint32)>(&Player::ActivateTaxiPathTo),

                static_cast<bool(Player::*)(uint32, uint32)>(&Player::ActivateTaxiPathTo)
            );

            /**
             *
             */
            type["CleanupAfterTaxiFlight"] = static_cast<void(Player::*)()>(&Player::CleanupAfterTaxiFlight);

            /**
             *
             */
            type["ContinueTaxiFlight"] = static_cast<void(Player::*)()>(&Player::ContinueTaxiFlight);

            /**
             *
             */
            type["SendTaxiNodeStatusMultiple"] = static_cast<void(Player::*)()>(&Player::SendTaxiNodeStatusMultiple);

            /**
             *
             */
            type["IsCommentator"] = static_cast<bool(Player::*)() const>(&Player::IsCommentator);

            /**
             *
             */
            type["SetCommentator"] = static_cast<void(Player::*)(bool)>(&Player::SetCommentator);

            /**
             *
             */
            type["IsDeveloper"] = static_cast<bool(Player::*)() const>(&Player::IsDeveloper);

            /**
             *
             */
            type["SetDeveloper"] = static_cast<void(Player::*)(bool)>(&Player::SetDeveloper);

            /**
             *
             */
            type["IsAcceptWhispers"] = static_cast<bool(Player::*)() const>(&Player::isAcceptWhispers);

            /**
             *
             */
            type["SetAcceptWhispers"] = static_cast<void(Player::*)(bool)>(&Player::SetAcceptWhispers);

            /**
             *
             */
            type["IsGameMaster"] = static_cast<bool(Player::*)() const>(&Player::IsGameMaster);

            /**
             *
             */
            type["SetGameMaster"] = static_cast<void(Player::*)(bool)>(&Player::SetGameMaster);

            /**
             *
             */
            type["IsGMChat"] = static_cast<bool(Player::*)() const>(&Player::isGMChat);

            /**
             *
             */
            type["SetGMChat"] = static_cast<void(Player::*)(bool)>(&Player::SetGMChat);

            /**
             *
             */
            type["IsGMSpectator"] = static_cast<bool(Player::*)() const>(&Player::IsGMSpectator);

            /**
             *
             */
            type["SetGMSpectator"] = static_cast<void(Player::*)(bool)>(&Player::SetGMSpectator);

            /**
             *
             */
            type["IsTaxiCheater"] = static_cast<bool(Player::*)() const>(&Player::isTaxiCheater);

            /**
             *
             */
            type["SetTaxiCheater"] = static_cast<void(Player::*)(bool)>(&Player::SetTaxiCheater);

            /**
             *
             */
            type["IsGMVisible"] = static_cast<bool(Player::*)() const>(&Player::isGMVisible);

            /**
             *
             */
            type["SetGMVisible"] = static_cast<void(Player::*)(bool)>(&Player::SetGMVisible);

            /**
             *
             */
            type["Has310Flyer"] = static_cast<bool(Player::*)(bool, uint32)>(&Player::Has310Flyer);

            /**
             *
             */
            type["SetHas310Flyer"] = static_cast<void(Player::*)(bool)>(&Player::SetHas310Flyer);

            /**
             *
             */
            type["SetPvPDeath"] = static_cast<void(Player::*)(bool)>(&Player::SetPvPDeath);

            /**
             *
             */
            type["GiveXP"] = static_cast<void(Player::*)(uint32, Unit*, float, bool)>(&Player::GiveXP);

            /**
             *
             */
            type["GiveLevel"] = static_cast<void(Player::*)(uint8)>(&Player::GiveLevel);

            /**
             *
             */
            type["InitStatsForLevel"] = static_cast<void(Player::*)(bool)>(&Player::InitStatsForLevel);

            /**
             *
             */
            type["HasActivePowerType"] = static_cast<bool(Player::*)(Powers)>(&Player::HasActivePowerType);

            /**
             *
             */
            type["GetCommandStatus"] = static_cast<bool(Player::*)(uint32) const>(&Player::GetCommandStatus);

            /**
             *
             */
            type["SetCommandStatusOn"] = static_cast<void(Player::*)(uint32)>(&Player::SetCommandStatusOn);

            /**
             *
             */
            type["SetCommandStatusOff"] = static_cast<void(Player::*)(uint32)>(&Player::SetCommandStatusOff);

            /**
             *
             */
            type["GetTotalPlayedTime"] = static_cast<uint32(Player::*)()>(&Player::GetTotalPlayedTime);

            /**
             *
             */
            type["GetLevelPlayedTime"] = static_cast<uint32(Player::*)()>(&Player::GetLevelPlayedTime);

            /**
             *
             */
            type["SetDeathState"] = static_cast<void(Player::*)(DeathState, bool)>(&Player::setDeathState);

            /**
             *
             */
            type["SetRestState"] = static_cast<void(Player::*)(uint32)>(&Player::SetRestState);

            /**
             *
             */
            type["RemoveRestState"] = static_cast<void(Player::*)()>(&Player::RemoveRestState);

            /**
             *
             */
            type["GetXPRestBonus"] = static_cast<uint32(Player::*)(uint32)>(&Player::GetXPRestBonus);

            /**
             *
             */
            type["GetRestBonus"] = static_cast<float(Player::*)() const>(&Player::GetRestBonus);

            /**
             *
             */
            type["SetRestBonus"] = static_cast<void(Player::*)(float)>(&Player::SetRestBonus);

            /**
             *
             */
            type["HasRestFlag"] = static_cast<bool(Player::*)(RestFlag) const>(&Player::HasRestFlag);

            /**
             *
             */
            type["SetRestFlag"] = static_cast<void(Player::*)(RestFlag, uint32)>(&Player::SetRestFlag);

            /**
             *
             */
            type["RemoveRestFlag"] = static_cast<void(Player::*)(RestFlag)>(&Player::RemoveRestFlag);

            /**
             *
             */
            type["GetInnTriggerId"] = static_cast<uint32(Player::*)() const>(&Player::GetInnTriggerId);

            /**
             *
             */
            type["GetPetStable"] = sol::overload(
                static_cast<PetStable*(Player::*)()>(&Player::GetPetStable),

                static_cast<const PetStable*(Player::*)() const>(&Player::GetPetStable)
            );

            /**
             *
             */
            type["GetOrInitPetStable"] = static_cast<PetStable&(Player::*)()>(&Player::GetOrInitPetStable);

            /**
             *
             */
            type["GetPet"] = static_cast<Pet*(Player::*)() const>(&Player::GetPet);

            /**
             *
             */
            type["SummonPet"] = static_cast<Pet*(Player::*)(uint32, float, float, float, float, PetType, Milliseconds, uint32)>(&Player::SummonPet);

            /**
             *
             */
            type["RemovePet"] = static_cast<void(Player::*)(Pet*, PetSaveMode, bool)>(&Player::RemovePet);

            /**
             *
             */
            type["CanPetResurrect"] = static_cast<bool(Player::*)()>(&Player::CanPetResurrect);

            /**
             *
             */
            type["IsExistPet"] = static_cast<bool(Player::*)()>(&Player::IsExistPet);

            /**
             *
             */
            type["CreatePet"] = sol::overload(
                static_cast<Pet*(Player::*)(Creature*, uint32)>(&Player::CreatePet),

                static_cast<Pet*(Player::*)(uint32, uint32)>(&Player::CreatePet)
            );

            /**
             *
             */
            type["GetPhaseMaskForSpawn"] = static_cast<uint32(Player::*)() const>(&Player::GetPhaseMaskForSpawn);

            /**
             *
             */
            type["SetVirtualItemSlot"] = static_cast<void(Player::*)(uint8, Item*)>(&Player::SetVirtualItemSlot);

            /**
             *
             */
            type["FindEquipSlot"] = static_cast<uint8(Player::*)(const ItemTemplate*, uint32, bool) const>(&Player::FindEquipSlot);

            /**
             *
             */
            type["GetItemCount"] = static_cast<uint32(Player::*)(uint32, bool, Item*) const>(&Player::GetItemCount);

            /**
             *
             */
            type["GetItemCountWithLimitCategory"] = static_cast<uint32(Player::*)(uint32, Item*) const>(&Player::GetItemCountWithLimitCategory);

            /**
             *
             */
            type["GetItemByGuid"] = static_cast<Item*(Player::*)(ObjectGuid) const>(&Player::GetItemByGuid);

            /**
             *
             */
            type["GetItemByEntry"] = static_cast<Item*(Player::*)(uint32) const>(&Player::GetItemByEntry);

            /**
             *
             */
            type["GetItemByPos"] = sol::overload(
                static_cast<Item*(Player::*)(uint16) const>(&Player::GetItemByPos),

                static_cast<Item*(Player::*)(uint8, uint8) const>(&Player::GetItemByPos)
            );

            /**
             *
             */
            type["GetBagByPos"] = static_cast<Bag*(Player::*)(uint8) const>(&Player::GetBagByPos);

            /**
             *
             */
            type["GetFreeInventorySpace"] = static_cast<uint32(Player::*)() const>(&Player::GetFreeInventorySpace);

            /**
             *
             */
            type["GetUseableItemByPos"] = static_cast<Item*(Player::*)(uint8, uint8) const>(&Player::GetUseableItemByPos);

            /**
             *
             */
            type["GetWeaponForAttack"] = static_cast<Item*(Player::*)(WeaponAttackType, bool) const>(&Player::GetWeaponForAttack);

            /**
             *
             */
            type["HasWeapon"] = static_cast<bool(Player::*)(WeaponAttackType) const>(&Player::HasWeapon);

            /**
             *
             */
            type["HasWeaponForAttack"] = static_cast<bool(Player::*)(WeaponAttackType) const>(&Player::HasWeaponForAttack);

            /**
             *
             */
            type["GetShield"] = static_cast<Item*(Player::*)(bool) const>(&Player::GetShield);

            /**
             *
             */
            type["GetAttackBySlot"] = &Player::GetAttackBySlot;

            /**
             *
             */
            type["GetItemUpdateQueue"] = static_cast<std::vector<Item*>&(Player::*)()>(&Player::GetItemUpdateQueue);

            /**
             *
             */
            type["IsInventoryPos"] = sol::overload(
                static_cast<bool(*)(uint16)>(&Player::IsInventoryPos),

                static_cast<bool(*)(uint8, uint8)>(&Player::IsInventoryPos)
            );

            /**
             *
             */
            type["IsEquipmentPos"] = sol::overload(
                static_cast<bool(*)(uint16)>(&Player::IsEquipmentPos),

                static_cast<bool(*)(uint8, uint8)>(&Player::IsEquipmentPos)
            );

            /**
             *
             */
            type["IsBagPos"] = &Player::IsBagPos;

            /**
             *
             */
            type["IsBankPos"] = sol::overload(
                static_cast<bool(*)(uint16)>(&Player::IsBankPos),

                static_cast<bool(*)(uint8, uint8)>(&Player::IsBankPos)
            );

            /**
             *
             */
            type["IsValidPos"] = sol::overload(
                static_cast<bool(Player::*)(uint16, bool)>(&Player::IsValidPos),

                static_cast<bool(Player::*)(uint8, uint8, bool)>(&Player::IsValidPos)
            );

            /**
             *
             */
            type["GetBankBagSlotCount"] = static_cast<uint8(Player::*)() const>(&Player::GetBankBagSlotCount);

            /**
             *
             */
            type["SetBankBagSlotCount"] = static_cast<void(Player::*)(uint8)>(&Player::SetBankBagSlotCount);

            /**
             *
             */
            type["HasItemCount"] = static_cast<bool(Player::*)(uint32, uint32, bool) const>(&Player::HasItemCount);

            /**
             *
             */
            type["HasItemFitToSpellRequirements"] = static_cast<bool(Player::*)(const SpellInfo*, const Item*) const>(&Player::HasItemFitToSpellRequirements);

            /**
             *
             */
            type["CanNoReagentCast"] = static_cast<bool(Player::*)(const SpellInfo*) const>(&Player::CanNoReagentCast);

            /**
             *
             */
            type["HasItemOrGemWithIdEquipped"] = static_cast<bool(Player::*)(uint32, uint32, uint8) const>(&Player::HasItemOrGemWithIdEquipped);

            /**
             *
             */
            type["HasItemOrGemWithLimitCategoryEquipped"] = static_cast<bool(Player::*)(uint32, uint32, uint8) const>(&Player::HasItemOrGemWithLimitCategoryEquipped);

            /**
             *
             */
            type["CanTakeMoreSimilarItems"] = sol::overload(
                static_cast<InventoryResult(Player::*)(Item*) const>(&Player::CanTakeMoreSimilarItems),

                static_cast<InventoryResult(Player::*)(uint32, uint32) const>(&Player::CanTakeMoreSimilarItems)
            );

            /**
             *
             */
            type["CanStoreNewItem"] = CanStoreNewItemWrapper;

            /**
             *
             */
            type["CanStoreItem"] = static_cast<InventoryResult(Player::*)(uint8, uint8, ItemPosCountVec&, Item*, bool) const>(&Player::CanStoreItem);

            /**
             *
             */
            type["CanStoreItems"] = CanStoreItemsWrapper;
        }
    }
}

#endif // ECLIPSE_PLAYER_METHODS_HPP