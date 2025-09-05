#ifndef ECLIPSE_CREATURE_AI_HPP
#define ECLIPSE_CREATURE_AI_HPP

#include "ScriptedCreature.h"
#include "EventDispatcher.hpp"
#include "Events.hpp"

namespace Eclipse
{
    class EclipseCreatureAI : public ScriptedAI
    {
    private:
        bool justSpawned;
        
    public:
        EclipseCreatureAI(Creature* creature) : ScriptedAI(creature), justSpawned(true) {}
        ~EclipseCreatureAI() = default;

        void UpdateAI(uint32 diff) override
        {
            if (justSpawned)
            {
                justSpawned = false;
                // EventDispatcher::GetInstance().TriggerKeyedEvent(CREATURE_EVENT_ON_SPAWN, me);
            }
            
            ScriptedAI::UpdateAI(diff);
        }

        void JustEngagedWith(Unit* target) override
        {
            // EventDispatcher::GetInstance().TriggerKeyedEvent(CREATURE_EVENT_ON_ENTER_COMBAT, me, target);
            ScriptedAI::JustEngagedWith(target);
        }

        void JustDied(Unit* killer) override
        {
            // EventDispatcher::GetInstance().TriggerKeyedEvent(CREATURE_EVENT_ON_JUST_DIED, me, killer);
            ScriptedAI::JustDied(killer);
        }

        void KilledUnit(Unit* victim) override
        {
            // EventDispatcher::GetInstance().TriggerKeyedEvent(CREATURE_EVENT_ON_KILLED_UNIT, me, victim);
            ScriptedAI::KilledUnit(victim);
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            // EventDispatcher::GetInstance().TriggerKeyedEvent(CREATURE_EVENT_ON_LEAVE_COMBAT, me);
            ScriptedAI::EnterEvadeMode(why);
        }

        void AttackStart(Unit* target) override
        {
            ScriptedAI::AttackStart(target);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            ScriptedAI::SpellHit(caster, spell);
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damagetype*/, SpellSchoolMask /*damageSchoolMask*/) override
        {
            ScriptedAI::DamageTaken(attacker, damage, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL);
        }
    };
}

#endif // ECLIPSE_CREATURE_AI_HPP