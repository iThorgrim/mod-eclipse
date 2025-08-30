#ifndef ECLIPSE_UNIT_METHODS_HPP
#define ECLIPSE_UNIT_METHODS_HPP

#include "EclipseIncludes.hpp"

namespace Eclipse
{
    namespace UnitMethods
    {       
        uint32 GetHealth(Unit* unit)
        {
            return unit ? unit->GetHealth() : 0;
        }
        
        uint32 GetMaxHealth(Unit* unit)
        {
            return unit ? unit->GetMaxHealth() : 0;
        }
        
        uint32 GetHealthPct(Unit* unit)
        {
            return unit ? unit->GetHealthPct() : 0;
        }
        
        void SetHealth(Unit* unit, uint32 health)
        {
            if (unit) unit->SetHealth(health);
        }
        
        uint32 GetPower(Unit* unit, uint32 power = POWER_MANA)
        {
            Powers pwr = static_cast<Powers>(power);
            return unit ? unit->GetPower(pwr) : 0;
        }
        
        uint32 GetMaxPower(Unit* unit, uint32 power = POWER_MANA)
        {
            Powers pwr = static_cast<Powers>(power);
            return unit ? unit->GetMaxPower(pwr) : 0;
        }
        
        void SetPower(Unit* unit, uint32 power, uint32 type = POWER_MANA)
        {
            Powers powerType = static_cast<Powers>(type);
            if (unit) unit->SetPower(powerType, power);
        }
               
        uint8 GetLevel(Unit* unit)
        {
            return unit ? unit->GetLevel() : 0;
        }
        
        void SetLevel(Unit* unit, uint8 level)
        {
            if (unit) unit->SetLevel(level);
        }
        
        uint32 GetStat(Unit* unit, Stats stat)
        {
            return unit ? unit->GetStat(stat) : 0;
        }
                
        bool IsInCombat(Unit* unit)
        {
            return unit ? unit->IsInCombat() : false;
        }
        
        bool IsAlive(Unit* unit)
        {
            return unit ? unit->IsAlive() : false;
        }
        
        bool IsDead(Unit* unit)
        {
            return unit ? unit->isDead() : true;
        }
        
        Unit* GetTarget(Unit* unit)
        {
            return unit ? unit->GetVictim() : nullptr;
        }
        
        void Attack(Unit* unit, Unit* target)
        {
            if (unit && target)
                unit->Attack(target, true);
        }
        
        void StopAttack(Unit* unit)
        {
            if (unit) unit->AttackStop();
        }
                
        void MoveTo(Unit* unit, float x, float y, float z)
        {
            if (unit) unit->GetMotionMaster()->MovePoint(0, x, y, z);
        }
        
        void MoveToTarget(Unit* unit, Unit* target)
        {
            if (unit && target)
                unit->GetMotionMaster()->MoveChase(target);
        }
        
        void StopMovement(Unit* unit)
        {
            if (unit) unit->StopMoving();
        }
                
        void CastSpell(Unit* unit, Unit* target, uint32 spellId)
        {
            if (unit) unit->CastSpell(target, spellId);
        }
        
        bool HasAura(Unit* unit, uint32 spellId)
        {
            return unit ? unit->HasAura(spellId) : false;
        }
        
        void AddAura(Unit* unit, uint32 spellId)
        {
            if (unit) unit->AddAura(spellId, unit);
        }
        
        void RemoveAura(Unit* unit, uint32 spellId)
        {
            if (unit) unit->RemoveAurasDueToSpell(spellId);
        }
                
        void Say(Unit* unit, const std::string& text)
        {
            if (unit) unit->Say(text, LANG_UNIVERSAL);
        }
        
        void Yell(Unit* unit, const std::string& text)
        {
            if (unit) unit->Yell(text, LANG_UNIVERSAL);
        }
        
        void Emote(Unit* unit, uint32 emote)
        {
            if (unit) unit->HandleEmoteCommand(emote);
        }
        
        // ========== LUA REGISTRATION ==========
        template<typename T>
        void RegisterUnitMethods(sol::usertype<T>& type)
        {            
            type["GetHealth"] = &GetHealth;
            type["GetMaxHealth"] = &GetMaxHealth;
            type["GetHealthPct"] = &GetHealthPct;
            type["SetHealth"] = &SetHealth;
            
            type["GetPower"] = &GetPower;
            type["GetMaxPower"] = &GetMaxPower;
            type["SetPower"] = &SetPower;
            
            type["GetLevel"] = &GetLevel;
            type["SetLevel"] = &SetLevel;
            type["GetStat"] = &GetStat;
            
            type["IsInCombat"] = &IsInCombat;
            type["IsAlive"] = &IsAlive;
            type["IsDead"] = &IsDead;
            type["GetTarget"] = &GetTarget;
            type["Attack"] = &Attack;
            type["StopAttack"] = &StopAttack;
            
            type["MoveTo"] = &MoveTo;
            type["MoveToTarget"] = &MoveToTarget;
            type["StopMovement"] = &StopMovement;
            
            type["CastSpell"] = &CastSpell;
            type["HasAura"] = &HasAura;
            type["AddAura"] = &AddAura;
            type["RemoveAura"] = &RemoveAura;
            
            type["Say"] = &Say;
            type["Yell"] = &Yell;
            type["Emote"] = &Emote;
        }
    }
}

#endif // ECLIPSE_UNIT_METHODS_HPP