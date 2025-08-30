#include "EclipseCreatureAI.hpp"
#include "ScriptMgr.h"
#include "MapStateManager.hpp"
#include "EventManager.hpp"

namespace Eclipse
{
    bool HasCreatureEvents(Creature* creature)
    {
        if (!creature)
            return false;
            
        uint32 creatureEntry = creature->GetEntry();
        auto& stateManager = MapStateManager::GetInstance();
        
        auto* globalEngine = stateManager.GetGlobalState();
        if (globalEngine && globalEngine->GetEventManager())
        {
            if (globalEngine->GetEventManager()->HasKeyedEvents<EventType::CREATURE>(creatureEntry))
            {
                return true;
            }
        }
        
        if (Map* map = creature->GetMap())
        {
            auto* mapEngine = stateManager.GetStateForMap(map->GetId());
            if (mapEngine && mapEngine != globalEngine && mapEngine->GetEventManager())
            {
                if (mapEngine->GetEventManager()->HasKeyedEvents<EventType::CREATURE>(creatureEntry))
                {
                    return true;
                }
            }
        }
        
        return false;
    }

    class Eclipse_AllCreatureScript : public AllCreatureScript
    {
    public:
        Eclipse_AllCreatureScript() : AllCreatureScript("Eclipse_AllCreatureScript") { }

        CreatureAI* GetCreatureAI(Creature* creature) const override
        {
            if (!creature)
                return nullptr;

            if (HasCreatureEvents(creature))
            {
                return new EclipseCreatureAI(creature);
            }

            return nullptr;
        }
    };
}

void AddEclipseCreatureAIScripts()
{
    new Eclipse::Eclipse_AllCreatureScript();
}