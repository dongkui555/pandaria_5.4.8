/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "violet_hold.h"

#define SPAWN_TIME          20000

enum PortalCreatures
{
    NPC_AZURE_INVADER_1                = 30661,
    NPC_AZURE_INVADER_2                = 30961,
    NPC_AZURE_SPELLBREAKER_1           = 30662,
    NPC_AZURE_SPELLBREAKER_2           = 30962,
    NPC_AZURE_BINDER_1                 = 30663,
    NPC_AZURE_BINDER_2                 = 30918,
    NPC_AZURE_MAGE_SLAYER_1            = 30664,
    NPC_AZURE_MAGE_SLAYER_2            = 30963,
    NPC_AZURE_CAPTAIN                  = 30666,
    NPC_AZURE_SORCEROR                 = 30667,
    NPC_AZURE_RAIDER                   = 30668,
    NPC_AZURE_STALKER                  = 32191,
};

enum AzureInvaderSpells
{
    SPELL_CLEAVE                      = 15496,
    SPELL_IMPALE                      = 58459,
    SPELL_IMPALE_H                    = 59256,
    SPELL_BRUTAL_STRIKE               = 58460,
    SPELL_SUNDER_ARMOR                = 58461
};

enum AzureSellbreakerSpells
{
    SPELL_ARCANE_BLAST                = 58462,
    SPELL_ARCANE_BLAST_H              = 59257,
    SPELL_SLOW                        = 25603,
    SPELL_CHAINS_OF_ICE               = 58464,
    SPELL_CONE_OF_COLD                = 58463,
    SPELL_CONE_OF_COLD_H              = 59258
};

enum AzureBinderSpells
{
    SPELL_ARCANE_BARRAGE              = 58456,
    SPELL_ARCANE_BARRAGE_H            = 59248,
    SPELL_ARCANE_EXPLOSION            = 58455,
    SPELL_ARCANE_EXPLOSION_H          = 59245,
    SPELL_FROST_NOVA                  = 58458,
    SPELL_FROST_NOVA_H                = 59253,
    SPELL_FROSTBOLT                   = 58457,
    SPELL_FROSTBOLT_H                 = 59251,
};

enum AzureMageSlayerSpells
{
    SPELL_ARCANE_EMPOWERMENT          = 58469,
    SPELL_SPELL_LOCK                  = 30849
};

enum AzureCaptainSpells
{
    SPELL_MORTAL_STRIKE               = 32736,
    SPELL_WHIRLWIND_OF_STEEL          = 41057
};

enum AzureSorcerorSpells
{
    SPELL_ARCANE_STREAM               = 60181,
    SPELL_ARCANE_STREAM_H             = 60204,
    SPELL_MANA_DETONATION             = 60182,
    SPELL_MANA_DETONATION_H           = 60205
};

enum AzureRaiderSpells
{
    SPELL_CONCUSSION_BLOW             = 52719,
    SPELL_MAGIC_REFLECTION            = 60158
};

enum AzureStalkerSpells
{
    SPELL_BACKSTAB                    = 58471,
    SPELL_TACTICAL_BLINK              = 58470
};

enum AzureSaboteurSpells
{
    SABOTEUR_SHIELD_DISRUPTION        = 58291,
    SABOTEUR_SHIELD_EFFECT            = 45775
};

enum TrashDoorSpell
{
    SPELL_DESTROY_DOOR_SEAL           = 58040
};

enum Spells
{
    SPELL_PORTAL_CHANNEL              = 58012,
    SPELL_CRYSTALL_ACTIVATION         = 57804
};

enum eSinclari
{
    // Sinclari
    SAY_SINCLARI_1                    = 0,
    SAY_SINCLARI_2                    = 1,
    SAY_SINCLARI_OUTRO                = 2, // unused

    // Sinclari Trigger
    SAY_SINCLARI_ELITE_SQUAD          = 0,
    SAY_SINCLARI_PORTAL_GUARDIAN      = 1,
    SAY_SINCLARI_PORTAL_KEEPER        = 2
};

enum Gossip
{
    GOSSIP_MENU_SINCLARI_START_1      = 9997,
    GOSSIP_MENU_SINCLARI_START_2      = 9998,
    GOSSIP_MENU_SINCLARI_END          = 10030,
    GOSSIP_MENU_SINCLARI_TELEPORT     = 10275,

    GOSSIP_OPTION_SINCLARI_START_1    = 0,
    GOSSIP_OPTION_SINCLARI_START_2    = 0,
    GOSSIP_OPTION_SINCLARI_TELEPORT   = 0,
};

float FirstPortalWPs[6][3] =
{
    { 1877.670288f, 842.280273f, 43.333591f },
    { 1877.338867f, 834.615356f, 38.762287f },
    { 1872.161011f, 823.854309f, 38.645401f },
    { 1864.860474f, 815.787170f, 38.784843f },
    { 1858.953735f, 810.048950f, 44.008759f },
    { 1843.707153f, 805.807739f, 44.135197f }
    //{ 1825.736084f, 807.305847f, 44.363785f }
};

float SecondPortalFirstWPs[9][3] =
{
    { 1902.561401f, 853.334656f, 47.106117f },
    { 1895.486084f, 855.376404f, 44.334591f },
    { 1882.805176f, 854.993286f, 43.333591f },
    { 1877.670288f, 842.280273f, 43.333591f },
    { 1877.338867f, 834.615356f, 38.762287f },
    { 1872.161011f, 823.854309f, 38.645401f },
    { 1864.860474f, 815.787170f, 38.784843f },
    { 1858.953735f, 810.048950f, 44.008759f },
    { 1843.707153f, 805.807739f, 44.135197f }
    //{ 1825.736084f, 807.305847f, 44.363785f }
};

float SecondPortalSecondWPs[8][3] =
{
    { 1929.392212f, 837.614990f, 47.136166f },
    { 1928.290649f, 824.750427f, 45.474411f },
    { 1915.544922f, 826.919373f, 38.642811f },
    { 1900.933960f, 818.855652f, 38.801647f },
    { 1886.810547f, 813.536621f, 38.490490f },
    { 1869.079712f, 808.701538f, 38.689003f },
    { 1860.843384f, 806.645020f, 44.008789f },
    { 1843.707153f, 805.807739f, 44.135197f }
    //{ 1825.736084f, 807.305847f, 44.363785f }
};

float ThirdPortalWPs[8][3] =
{
    { 1934.049438f, 815.778503f, 52.408699f },
    { 1928.290649f, 824.750427f, 45.474411f },
    { 1915.544922f, 826.919373f, 38.642811f },
    { 1900.933960f, 818.855652f, 38.801647f },
    { 1886.810547f, 813.536621f, 38.490490f },
    { 1869.079712f, 808.701538f, 38.689003f },
    { 1860.843384f, 806.645020f, 44.008789f },
    { 1843.707153f, 805.807739f, 44.135197f }
    //{ 1825.736084f, 807.305847f, 44.363785f }
};

float FourthPortalWPs[9][3] =
{
    { 1921.658447f, 761.657043f, 50.866741f },
    { 1910.559814f, 755.780457f, 47.701447f },
    { 1896.664673f, 752.920898f, 47.667004f },
    { 1887.398804f, 763.633240f, 47.666851f },
    { 1879.020386f, 775.396973f, 38.705990f },
    { 1872.439087f, 782.568604f, 38.808292f },
    { 1863.573364f, 791.173584f, 38.743660f },
    { 1857.811890f, 796.765564f, 43.950329f },
    { 1845.577759f, 800.681152f, 44.104248f }
    //{ 1827.100342f, 801.605957f, 44.363358f }
};

float FifthPortalWPs[6][3] =
{
    { 1887.398804f, 763.633240f, 47.666851f },
    { 1879.020386f, 775.396973f, 38.705990f },
    { 1872.439087f, 782.568604f, 38.808292f },
    { 1863.573364f, 791.173584f, 38.743660f },
    { 1857.811890f, 796.765564f, 43.950329f },
    { 1845.577759f, 800.681152f, 44.104248f }
    //{ 1827.100342f, 801.605957f, 44.363358f }
};

float SixthPoralWPs[4][3] =
{
    { 1888.861084f, 805.074768f, 38.375790f },
    { 1869.793823f, 804.135804f, 38.647018f },
    { 1861.541504f, 804.149780f, 43.968292f },
    { 1843.567017f, 804.288208f, 44.139091f }
    //{ 1826.889648f, 803.929993f, 44.363239f }
};

const float SaboteurFinalPos1[3][3] =
{
    { 1892.502319f, 777.410767f, 38.630402f },
    { 1891.165161f, 762.969421f, 47.666920f },
    { 1893.168091f, 740.919189f, 47.666920f }
};
const float SaboteurFinalPos2[3][3] =
{
    { 1882.242676f, 834.818726f, 38.646786f },
    { 1879.220825f, 842.224854f, 43.333641f },
    { 1873.842896f, 863.892456f, 43.333641f }
};
const float SaboteurFinalPos3[2][3] =
{
    { 1904.298340f, 792.400391f, 38.646782f },
    { 1935.716919f, 758.437073f, 30.627895f }
};
const float SaboteurFinalPos4[3] = { 1855.006104f, 760.641724f, 38.655266f };
const float SaboteurFinalPos5[3] = { 1906.667358f, 841.705566f, 38.637894f };

const float SaboteurFinalPos6[5][3] =
{
    { 1911.437012f, 821.289246f, 38.684128f },
    { 1920.734009f, 822.978027f, 41.525414f },
    { 1928.262939f, 830.836609f, 44.668266f },
    { 1929.338989f, 837.593933f, 47.137596f },
    { 1931.063354f, 848.468445f, 47.190434f }
 };

const std::vector<uint32> invadersType =
{
    NPC_AZURE_INVADER_1,
    NPC_AZURE_INVADER_2,
    NPC_AZURE_SPELLBREAKER_1,
    NPC_AZURE_SPELLBREAKER_2,
    NPC_AZURE_BINDER_1,
    NPC_AZURE_BINDER_2,
    NPC_AZURE_MAGE_SLAYER_1,
    NPC_AZURE_MAGE_SLAYER_2,
};

const Position playerTeleportPosition = { 1830.531006f, 803.939758f, 44.340508f, 6.281611f };

class npc_sinclari_vh : public CreatureScript
{
    public:
        npc_sinclari_vh() : CreatureScript("npc_sinclari_vh") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->PlayerTalkClass->GetGossipMenu().SetMenuId(GOSSIP_MENU_SINCLARI_START_2);
                    player->ADD_GOSSIP_ITEM_DB(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), GOSSIP_OPTION_SINCLARI_START_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                    player->SEND_GOSSIP_MENU(player->GetGossipTextId(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), creature), creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    CAST_AI(npc_sinclari_vh::npc_sinclariAI, (creature->AI()))->uiPhase = 1;
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    player->NearTeleportTo(playerTeleportPosition.GetPositionX(),playerTeleportPosition.GetPositionY(),playerTeleportPosition.GetPositionZ(),playerTeleportPosition.GetOrientation(),true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                case GOSSIP_ACTION_INFO_DEF + 5:
                case GOSSIP_ACTION_INFO_DEF + 6:
                case GOSSIP_ACTION_INFO_DEF + 7:
                case GOSSIP_ACTION_INFO_DEF + 8:
                case GOSSIP_ACTION_INFO_DEF + 9:
                case GOSSIP_ACTION_INFO_DEF + 10:
                    if (InstanceScript* instance = creature->GetInstanceScript())
                    {
                        instance->SetData(DATA_MAIN_EVENT_PHASE, IN_PROGRESS);
                        instance->SetData(DATA_WAVE_COUNT, 6);
                        instance->SetData(DATA_FIRST_BOSS, action - (GOSSIP_ACTION_INFO_DEF+4));
                        instance->SetData(DATA_SECOND_BOSS, 0);
                    }
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                switch (instance->GetData(DATA_MAIN_EVENT_PHASE))
                {
                    case NOT_STARTED:
                    case FAIL: // Allow to start event if not started or wiped
                        player->PlayerTalkClass->GetGossipMenu().SetMenuId(GOSSIP_MENU_SINCLARI_START_1);
                        player->ADD_GOSSIP_ITEM_DB(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), GOSSIP_OPTION_SINCLARI_START_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                        if (player->IsGameMaster())
                        {
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Quick start with random bosses", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 0);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Moragg",                   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 1);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Erekem",                   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 2);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Ichoron",                  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 3);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Lavanthor",                GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 4);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Xevozz",                   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 5);
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[GM] Spawn Zuramat",                  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4 + 6);
                        }
                        player->SEND_GOSSIP_MENU(player->GetGossipTextId(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), creature), creature->GetGUID());
                        break;
                    case IN_PROGRESS: // Allow to teleport inside if event is in progress
                        player->PlayerTalkClass->GetGossipMenu().SetMenuId(GOSSIP_MENU_SINCLARI_TELEPORT);
                        player->ADD_GOSSIP_ITEM_DB(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), GOSSIP_OPTION_SINCLARI_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                        player->SEND_GOSSIP_MENU(player->GetGossipTextId(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), creature), creature->GetGUID());
                        break;
                    default:
                        player->PlayerTalkClass->GetGossipMenu().SetMenuId(GOSSIP_MENU_SINCLARI_END);
                        player->SEND_GOSSIP_MENU(player->GetGossipTextId(player->PlayerTalkClass->GetGossipMenu().GetMenuId(), creature), creature->GetGUID());
                }
            }
            return true;
        }

        struct npc_sinclariAI : public ScriptedAI
        {
            npc_sinclariAI(Creature* creature) : ScriptedAI(creature)
            {
               instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            uint8  uiPhase;
            uint32 uiTimer;

            void Reset() override
            {
                uiPhase = 0;
                uiTimer = 0;

                me->SetReactState(REACT_PASSIVE);

                if (me->IsSummon())
                    return;

                me->m_Events.Schedule(1, [this]()
                {
                    std::list<Creature*> GuardList;
                    me->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 40.0f);
                    if (!GuardList.empty())
                    {
                        for (auto&& pGuard : GuardList)
                        {
                            pGuard->DisappearAndDie();
                            pGuard->Respawn();
                            pGuard->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                            pGuard->SetVisible(true);
                            pGuard->SetReactState(REACT_AGGRESSIVE);
                        }
                    }
                });
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type == POINT_MOTION_TYPE && pointId == 1)
                {
                    me->HandleEmoteCommand(EMOTE_ONESHOT_USE_STANDING);
                    instance->SetData(DATA_ACTIVATE_CRYSTAL, 0);
                    uiTimer = 2000;
                    uiPhase = 3;
                }
                else if (type == POINT_MOTION_TYPE && pointId == 2)
                {
                    me->SetFacingTo(0);
                    uiTimer = 5000;
                    uiPhase = 7;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                ScriptedAI::UpdateAI(diff);

                if (uiPhase)
                {
                    if (uiTimer <= diff)
                    {
                        switch (uiPhase)
                        {
                            case 1:
                                me->GetMotionMaster()->MovePoint(1, 1829.142f, 798.219f, 44.39403f);
                                uiTimer = 0;
                                uiPhase = 2;
                                break;
                            case 2:
                                // Waiting for movement generator
                                break;
                            case 3:
                            {
                                me->SetFacingTo(0.122173f);
                                Talk(SAY_SINCLARI_1);
                                uiTimer = 400;
                                uiPhase = 4;
                                break;
                            }
                            case 4:
                            {
                                std::list<Creature*> GuardList;
                                me->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 40.0f);
                                if (!GuardList.empty())
                                    for (auto&& pGuard : GuardList)
                                    {
                                        if (pGuard)
                                        {
                                            pGuard->SetWalk(false);
                                            pGuard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                                            pGuard->GetMotionMaster()->MovePoint(0, 1802.086f, 803.9344f, 44.36466f);
                                            pGuard->m_Events.Schedule(6000, [pGuard]() { pGuard->SetVisible(false); });
                                        }
                                    }
                                uiTimer = 2000;
                                uiPhase = 5;
                                break;
                            }
                            case 5:
                                me->GetMotionMaster()->MovePoint(2, 1820.177f, 803.8799f, 44.36465f);
                                uiTimer = 0;
                                uiPhase = 6;
                                break;
                            case 6:
                                // Waiting for movement generator
                                break;
                            case 7:
                                Talk(SAY_SINCLARI_2);
                                uiTimer = 3500;
                                uiPhase = 8;
                                break;
                            case 8:
                                me->HandleEmoteCommand(EMOTE_ONESHOT_TALK_NO_SHEATHE);
                                uiTimer = 200;
                                uiPhase = 9;
                            case 9:
                                if (instance)
                                    instance->SetData(DATA_MAIN_EVENT_PHASE,IN_PROGRESS);
                                uiTimer = 6000;
                                uiPhase = 10;
                                break;
                            case 10:
                                me->SummonCreature(me->GetEntry(), 1816.185f, 804.0629f, 44.44799f, 3.176499f);
                                me->DespawnOrUnsummon();
                                uiTimer = 0;
                                uiPhase = 0;
                                break;
                        }
                    }
                    else uiTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_sinclariAI(creature);
        }
};

class npc_azure_saboteur : public CreatureScript
{
    public:
        npc_azure_saboteur() : CreatureScript("npc_azure_saboteur") { }

        struct npc_azure_saboteurAI : public npc_escortAI
        {
            npc_azure_saboteurAI(Creature* creature):npc_escortAI(creature)
            {
                instance = creature->GetInstanceScript();
                bHasGotMovingPoints = false;
                uiBoss = 0;
                Reset();
            }

            InstanceScript* instance;
            bool bHasGotMovingPoints;
            uint32 uiBoss;

            void AttackStart(Unit* /*who*/) override { }
            void MoveInLineOfSight(Unit* /*who*/) override { }

            void Reset() override
            {
                if (instance && !uiBoss)
                    uiBoss = instance->GetData(DATA_WAVE_COUNT) == 6 ? instance->GetData(DATA_FIRST_BOSS) : instance->GetData(DATA_SECOND_BOSS);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void WaypointReached(uint32 waypointId)
            {
                switch (uiBoss)
                {
                    case 1:
                        if (waypointId == 2)
                            FinishPointReached();
                        break;
                    case 2:
                        if (waypointId == 2)
                            FinishPointReached();
                        break;
                    case 3:
                        if (waypointId == 1)
                            FinishPointReached();
                        break;
                    case 4:
                        if (waypointId == 0)
                            FinishPointReached();
                        break;
                    case 5:
                        if (waypointId == 0)
                            FinishPointReached();
                        break;
                    case 6:
                        if (waypointId == 4)
                            FinishPointReached();
                        break;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (instance && instance->GetData(DATA_MAIN_EVENT_PHASE) != IN_PROGRESS)
                    me->CastStop();

                npc_escortAI::UpdateAI(diff);

                if (!bHasGotMovingPoints)
                {
                    bHasGotMovingPoints = true;
                    switch (uiBoss)
                    {
                        case 1:
                            for (int i = 0; i < 3; i++)
                                AddWaypoint(i,SaboteurFinalPos1[i][0],SaboteurFinalPos1[i][1],SaboteurFinalPos1[i][2],0);
                            me->SetHomePosition(SaboteurFinalPos1[2][0],SaboteurFinalPos1[2][1],SaboteurFinalPos1[2][2],4.762346f);
                            break;
                        case 2:
                            for (int i = 0; i < 3; i++)
                                AddWaypoint(i,SaboteurFinalPos2[i][0],SaboteurFinalPos2[i][1],SaboteurFinalPos2[i][2],0);
                            me->SetHomePosition(SaboteurFinalPos2[2][0],SaboteurFinalPos2[2][1],SaboteurFinalPos2[2][2],1.862674f);
                            break;
                        case 3:
                            for (int i = 0; i < 2 ; i++)
                                AddWaypoint(i,SaboteurFinalPos3[i][0],SaboteurFinalPos3[i][1],SaboteurFinalPos3[i][2],0);
                            me->SetHomePosition(SaboteurFinalPos3[1][0],SaboteurFinalPos3[1][1],SaboteurFinalPos3[1][2],5.500638f);
                            break;
                        case 4:
                            AddWaypoint(0,SaboteurFinalPos4[0],SaboteurFinalPos4[1],SaboteurFinalPos4[2],0);
                            me->SetHomePosition(SaboteurFinalPos4[0],SaboteurFinalPos4[1],SaboteurFinalPos4[2],3.991108f);
                            break;
                        case 5:
                            AddWaypoint(0,SaboteurFinalPos5[0],SaboteurFinalPos5[1],SaboteurFinalPos5[2],0);
                            me->SetHomePosition(SaboteurFinalPos5[0],SaboteurFinalPos5[1],SaboteurFinalPos5[2],1.100841f);
                            break;
                        case 6:
                            for (int i = 0; i < 5; i++)
                                AddWaypoint(i,SaboteurFinalPos6[i][0],SaboteurFinalPos6[i][1],SaboteurFinalPos6[i][2],0);
                            me->SetHomePosition(SaboteurFinalPos6[4][0],SaboteurFinalPos6[4][1],SaboteurFinalPos6[4][2],0.983031f);
                            break;
                    }

                    SetDespawnAtEnd(false);
                    Start(true,true);
                }
            }

            void FinishPointReached()
            {
                me->CastSpell(me, SABOTEUR_SHIELD_DISRUPTION, false);
                me->DisappearAndDie();
                Creature* pSaboPort = Unit::GetCreature(*me, instance->GetData64(DATA_SABOTEUR_PORTAL));
                if (pSaboPort)
                    pSaboPort->DisappearAndDie();
                instance->SetData(DATA_START_BOSS_ENCOUNTER, 1);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_saboteurAI(creature);
        }
};

class npc_teleportation_portal_vh : public CreatureScript
{
    public:
        npc_teleportation_portal_vh() : CreatureScript("npc_teleportation_portal_vh") { }

        struct npc_teleportation_portalAI : public ScriptedAI
        {
            npc_teleportation_portalAI(Creature* creature) : ScriptedAI(creature), listOfMobs(me)
            {
                instance = creature->GetInstanceScript();
                uiTypeOfMobsPortal = urand(0, 1); // 0 - elite mobs   1 - portal guardian or portal keeper with regular mobs
                bPortalGuardianOrKeeperOrEliteSpawn = false;
            }

            uint32 uiSpawnTimer;
            bool bPortalGuardianOrKeeperOrEliteSpawn;
            uint8 uiTypeOfMobsPortal;

            SummonList listOfMobs;

            InstanceScript* instance;

            void Reset() override
            {
                uiSpawnTimer = 10000;
                bPortalGuardianOrKeeperOrEliteSpawn = false;
            }

            void EnterCombat(Unit* /*who*/) override { }

            void MoveInLineOfSight(Unit* /*who*/) override { }

            void UpdateAI(uint32 diff) override
            {
                if (!instance) //Massive usage of instance, global check
                    return;

                if (instance->GetData(DATA_REMOVE_NPC) == 1)
                {
                    me->DespawnOrUnsummon();
                    instance->SetData(DATA_REMOVE_NPC, 0);
                }

                uint8 uiWaveCount = instance->GetData(DATA_WAVE_COUNT);
                if ((uiWaveCount == 6) || (uiWaveCount == 12)) //Don't spawn mobs on boss encounters
                    return;

                switch (uiTypeOfMobsPortal)
                {
                    // spawn elite mobs and then set portals visibility to make it look like it dissapeard
                    case 0:
                        if (!bPortalGuardianOrKeeperOrEliteSpawn)
                        {
                            if (uiSpawnTimer <= diff)
                            {
                                bPortalGuardianOrKeeperOrEliteSpawn = true;
                                uint8 k = uiWaveCount < 12 ? 2 : 3;
                                for (uint8 i = 0; i < k; ++i)
                                {
                                    uint32 entry = RAND(NPC_AZURE_CAPTAIN,NPC_AZURE_RAIDER,NPC_AZURE_STALKER,NPC_AZURE_SORCEROR);
                                    DoSummon(entry, me, 2.0f, 20000, TEMPSUMMON_DEAD_DESPAWN);
                                }

                                if (Creature* sinclariTrigger = instance->instance->GetCreature(instance->GetData64(DATA_SINCLARI_TRIGGER)))
                                    sinclariTrigger->AI()->Talk(SAY_SINCLARI_ELITE_SQUAD);

                                me->SetVisible(false);
                            } else uiSpawnTimer -= diff;
                        }
                        else
                        {
                            // if all spawned elites have died kill portal
                            if (listOfMobs.empty())
                            {
                                me->Kill(me, false);
                                me->RemoveCorpse();
                            }
                        }
                        break;
                    // spawn portal guardian or portal keeper with regular mobs
                    case 1:
                        if (uiSpawnTimer <= diff)
                        {
                            if (bPortalGuardianOrKeeperOrEliteSpawn)
                            {
                                uint8 k = instance->GetData(DATA_WAVE_COUNT) < 12 ? 3 : 4;
                                for (uint8 i = 0; i < k; ++i)
                                {
                                    uint32 entry = RAND(NPC_AZURE_INVADER_1, NPC_AZURE_INVADER_2, NPC_AZURE_SPELLBREAKER_1, NPC_AZURE_SPELLBREAKER_2, NPC_AZURE_MAGE_SLAYER_1, NPC_AZURE_MAGE_SLAYER_2, NPC_AZURE_BINDER_1, NPC_AZURE_BINDER_2);
                                    DoSummon(entry, me, 2.0f, 20000, TEMPSUMMON_DEAD_DESPAWN);
                                }
                            }
                            else
                            {
                                bPortalGuardianOrKeeperOrEliteSpawn = true;
                                uint32 entry = RAND(NPC_PORTAL_GUARDIAN, NPC_PORTAL_KEEPER);
                                if (Creature* pPortalKeeper = DoSummon(entry, me, 2.0f, 0, TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    me->CastSpell(pPortalKeeper, SPELL_PORTAL_CHANNEL, false);
                                    me->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                                }

                                if (Creature* sinclariTrigger = instance->instance->GetCreature(instance->GetData64(DATA_SINCLARI_TRIGGER)))
                                    sinclariTrigger->AI()->Talk(entry == NPC_PORTAL_GUARDIAN ? SAY_SINCLARI_PORTAL_GUARDIAN : SAY_SINCLARI_PORTAL_KEEPER);
                            }
                            uiSpawnTimer = SPAWN_TIME;
                        } else uiSpawnTimer -= diff;

                        if (bPortalGuardianOrKeeperOrEliteSpawn && !me->IsNonMeleeSpellCasted(false))
                        {
                            me->Kill(me, false);
                            me->RemoveCorpse();
                        }
                        break;
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (instance)
                    instance->SetData(DATA_WAVE_COUNT,instance->GetData(DATA_WAVE_COUNT)+1);
            }

            void JustSummoned(Creature* summon) override
            {
                listOfMobs.Summon(summon);
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
            {
                listOfMobs.Despawn(summon);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_teleportation_portalAI(creature);
        }
};

struct violet_hold_trashAI : public npc_escortAI
{
    violet_hold_trashAI(Creature* creature):npc_escortAI(creature)
    {
        instance = creature->GetInstanceScript();
        bHasGotMovingPoints = false;
        if (instance)
            portalLocationID = instance->GetData(DATA_PORTAL_LOCATION);
        Reset();
    }

    public:
        InstanceScript* instance;
        bool bHasGotMovingPoints;
        bool isAtDoor = false;
        uint32 portalLocationID;
        uint32 secondPortalRouteID;

    void WaypointReached(uint32 uiPointId)
    {
        switch (portalLocationID)
        {
            case 0:
                if (uiPointId == 5)
                   CreatureStartAttackDoor();
                break;
            case 1:
                if ((uiPointId == 8 && secondPortalRouteID == 0) || (uiPointId == 7 && secondPortalRouteID == 1))
                    CreatureStartAttackDoor();
                break;
            case 2:
                if (uiPointId == 7)
                   CreatureStartAttackDoor();
                break;
            case 3:
                if (uiPointId == 8)
                    CreatureStartAttackDoor();
                break;
            case 4:
                if (uiPointId == 5)
                    CreatureStartAttackDoor();
                break;
            case 5:
                if (uiPointId == 3)
                    CreatureStartAttackDoor();
                break;
        }
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        if (instance && instance->GetData(DATA_MAIN_EVENT_PHASE) != IN_PROGRESS)
            me->CastStop();

        if (!bHasGotMovingPoints)
        {
            bHasGotMovingPoints = true;
            switch (portalLocationID)
            {
                case 0:
                    for (int i = 0; i < 6; i++)
                        AddWaypoint(i,FirstPortalWPs[i][0]+irand(-1, 1),FirstPortalWPs[i][1]+irand(-1, 1),FirstPortalWPs[i][2]+irand(-1, 1),0);
                    me->SetHomePosition(FirstPortalWPs[5][0],FirstPortalWPs[5][1],FirstPortalWPs[5][2],3.149439f);
                    break;
                case 1:
                    secondPortalRouteID = urand(0, 1);
                    switch (secondPortalRouteID)
                    {
                        case 0:
                            for (int i = 0 ; i < 9 ; i++)
                                AddWaypoint(i,SecondPortalFirstWPs[i][0]+irand(-1, 1),SecondPortalFirstWPs[i][1]+irand(-1, 1),SecondPortalFirstWPs[i][2],0);
                            me->SetHomePosition(SecondPortalFirstWPs[8][0]+irand(-1, 1),SecondPortalFirstWPs[8][1]+irand(-1, 1),SecondPortalFirstWPs[8][2]+irand(-1, 1),3.149439f);
                            break;
                        case 1:
                            for (int i = 0; i <8 ; i++)
                                AddWaypoint(i,SecondPortalSecondWPs[i][0]+irand(-1, 1),SecondPortalSecondWPs[i][1]+irand(-1, 1),SecondPortalSecondWPs[i][2],0);
                            me->SetHomePosition(SecondPortalSecondWPs[7][0],SecondPortalSecondWPs[7][1],SecondPortalSecondWPs[7][2],3.149439f);
                            break;
                    }
                    break;
                case 2:
                    for (int i = 0; i < 8; i++)
                        AddWaypoint(i,ThirdPortalWPs[i][0]+irand(-1, 1),ThirdPortalWPs[i][1]+irand(-1, 1),ThirdPortalWPs[i][2],0);
                        me->SetHomePosition(ThirdPortalWPs[7][0],ThirdPortalWPs[7][1],ThirdPortalWPs[7][2],3.149439f);
                    break;
                case 3:
                    for (int i = 0; i < 9; i++)
                        AddWaypoint(i,FourthPortalWPs[i][0]+irand(-1, 1),FourthPortalWPs[i][1]+irand(-1, 1),FourthPortalWPs[i][2],0);
                    me->SetHomePosition(FourthPortalWPs[8][0],FourthPortalWPs[8][1],FourthPortalWPs[8][2],3.149439f);
                    break;
                case 4:
                    for (int i = 0; i < 6; i++)
                        AddWaypoint(i,FifthPortalWPs[i][0]+irand(-1, 1),FifthPortalWPs[i][1]+irand(-1, 1),FifthPortalWPs[i][2],0);
                    me->SetHomePosition(FifthPortalWPs[5][0],FifthPortalWPs[5][1],FifthPortalWPs[5][2],3.149439f);
                    break;
                case 5:
                    for (int i = 0; i < 4; i++)
                        AddWaypoint(i,SixthPoralWPs[i][0]+irand(-1, 1),SixthPoralWPs[i][1]+irand(-1, 1),SixthPoralWPs[i][2],0);
                    me->SetHomePosition(SixthPoralWPs[3][0],SixthPoralWPs[3][1],SixthPoralWPs[3][2],3.149439f);
                    break;
            }
            SetDespawnAtEnd(false);
            Start(true,true);
        }
    }

    void JustDied(Unit* /*unit*/) override
    {
        if (instance && isAtDoor)
            instance->SetData(DATA_NPC_PRESENCE_AT_DOOR_REMOVE, 1);
    }

    void CreatureStartAttackDoor()
    {
        isAtDoor = true;
        //me->SetReactState(REACT_PASSIVE);
        DoCast(SPELL_DESTROY_DOOR_SEAL);
        if (instance)
            instance->SetData(DATA_NPC_PRESENCE_AT_DOOR_ADD,1);
    }
};

class npc_azure_invader : public CreatureScript
{
    public:
        npc_azure_invader() : CreatureScript("npc_azure_invader") { }

        struct npc_azure_invaderAI : public violet_hold_trashAI
        {
            npc_azure_invaderAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiCleaveTimer;
            uint32 uiImpaleTimer;
            uint32 uiBrutalStrikeTimer;
            uint32 uiSunderArmorTimer;

            void Reset() override
            {
                uiCleaveTimer = 5000;
                uiImpaleTimer = 4000;
                uiBrutalStrikeTimer = 5000;
                uiSunderArmorTimer = 4000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (me->GetEntry() == NPC_AZURE_INVADER_1)
                {
                    if (uiCleaveTimer <= diff)
                    {
                        DoCast(me->GetVictim(),SPELL_CLEAVE);
                        uiCleaveTimer = 5000;
                    } else uiCleaveTimer -= diff;

                    if (uiImpaleTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,SPELL_IMPALE);
                        uiImpaleTimer = 4000;
                    } else uiImpaleTimer -= diff;
                }

                if (me->GetEntry() == NPC_AZURE_INVADER_2)
                {
                    if (uiBrutalStrikeTimer <= diff)
                    {
                        DoCast(me->GetVictim(),SPELL_BRUTAL_STRIKE);
                        uiBrutalStrikeTimer = 5000;
                    } else uiBrutalStrikeTimer -= diff;

                    if (uiSunderArmorTimer <= diff)
                    {
                        DoCast(me->GetVictim(),SPELL_SUNDER_ARMOR);
                        uiSunderArmorTimer = urand(8000, 10000);
                    } else uiSunderArmorTimer -= diff;

                    DoMeleeAttackIfReady();
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_invaderAI(creature);
        }
};

class npc_azure_binder : public CreatureScript
{
    public:
        npc_azure_binder() : CreatureScript("npc_azure_binder") { }

        struct npc_azure_binderAI : public violet_hold_trashAI
        {
            npc_azure_binderAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiArcaneExplosionTimer;
            uint32 uiArcainBarrageTimer;
            uint32 uiFrostNovaTimer;
            uint32 uiFrostboltTimer;

            void Reset() override
            {
                uiArcaneExplosionTimer = 5000;
                uiArcainBarrageTimer = 4000;
                uiFrostNovaTimer = 5000;
                uiFrostboltTimer = 4000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (me->GetEntry() == NPC_AZURE_BINDER_1)
                {
                    if (uiArcaneExplosionTimer <= diff)
                    {
                        DoCast(DUNGEON_MODE(SPELL_ARCANE_EXPLOSION,SPELL_ARCANE_EXPLOSION_H));
                        uiArcaneExplosionTimer = 5000;
                    } else uiArcaneExplosionTimer -= diff;

                    if (uiArcainBarrageTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,DUNGEON_MODE(SPELL_ARCANE_BARRAGE,SPELL_ARCANE_BARRAGE_H));
                        uiArcainBarrageTimer = 6000;
                    } else uiArcainBarrageTimer -= diff;
                }

                if (me->GetEntry() == NPC_AZURE_BINDER_2)
                {
                    if (uiFrostNovaTimer <= diff)
                    {
                        DoCast(DUNGEON_MODE(SPELL_FROST_NOVA,SPELL_FROST_NOVA_H));
                        uiFrostNovaTimer = 5000;
                    } else uiFrostNovaTimer -= diff;

                    if (uiFrostboltTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,DUNGEON_MODE(SPELL_FROSTBOLT,SPELL_FROSTBOLT_H));
                        uiFrostboltTimer = 6000;
                    } else uiFrostboltTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_binderAI(creature);
        }
};

class npc_azure_mage_slayer : public CreatureScript
{
    public:
        npc_azure_mage_slayer() : CreatureScript("npc_azure_mage_slayer") { }

        struct npc_azure_mage_slayerAI : public violet_hold_trashAI
        {
            npc_azure_mage_slayerAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiArcaneEmpowermentTimer;
            uint32 uiSpellLockTimer;

            void Reset() override
            {
                uiArcaneEmpowermentTimer = 5000;
                uiSpellLockTimer = 5000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (me->GetEntry() == NPC_AZURE_MAGE_SLAYER_1)
                {
                    if (uiArcaneEmpowermentTimer <= diff)
                    {
                        DoCast(me,SPELL_ARCANE_EMPOWERMENT);
                            uiArcaneEmpowermentTimer = 14000;
                    } else uiArcaneEmpowermentTimer -= diff;
                }

                if (me->GetEntry() == NPC_AZURE_MAGE_SLAYER_2)
                {
                    if (uiSpellLockTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,SPELL_SPELL_LOCK);
                        uiSpellLockTimer = 9000;
                    } else uiSpellLockTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_mage_slayerAI(creature);
        }
};

class npc_azure_raider : public CreatureScript
{
    public:
        npc_azure_raider() : CreatureScript("npc_azure_raider") { }

        struct npc_azure_raiderAI : public violet_hold_trashAI
        {
            npc_azure_raiderAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiConcussionBlowTimer;
            uint32 uiMagicReflectionTimer;

            void Reset() override
            {
                uiConcussionBlowTimer = 5000;
                uiMagicReflectionTimer = 8000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (uiConcussionBlowTimer <= diff)
                {
                    DoCast(me->GetVictim(),SPELL_CONCUSSION_BLOW);
                    uiConcussionBlowTimer = 5000;
                } else uiConcussionBlowTimer -= diff;

                if (uiMagicReflectionTimer <= diff)
                {
                    DoCast(SPELL_MAGIC_REFLECTION);
                    uiMagicReflectionTimer = urand(10000, 15000);
                } else uiMagicReflectionTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_raiderAI(creature);
        }
};

class npc_azure_stalker : public CreatureScript
{
    public:
        npc_azure_stalker() : CreatureScript("npc_azure_stalker") { }

        struct npc_azure_stalkerAI : public violet_hold_trashAI
        {
            npc_azure_stalkerAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiBackstabTimer;
            uint32 uiTacticalBlinkTimer;
            bool TacticalBlinkCasted;

            void Reset() override
            {
                uiBackstabTimer = 1300;
                uiTacticalBlinkTimer = 8000;
                TacticalBlinkCasted =false;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (!TacticalBlinkCasted)
                {
                    if (uiTacticalBlinkTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                            DoCast(target,SPELL_TACTICAL_BLINK);
                        uiTacticalBlinkTimer = 6000;
                        TacticalBlinkCasted = true;
                    } else uiTacticalBlinkTimer -= diff;
                }

                else
                {
                    if (uiBackstabTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 10, true))
                            DoCast(target,SPELL_BACKSTAB);
                        TacticalBlinkCasted = false;
                        uiBackstabTimer =1300;
                    } else uiBackstabTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_stalkerAI(creature);
        }
};

class npc_azure_spellbreaker : public CreatureScript
{
    public:
        npc_azure_spellbreaker() : CreatureScript("npc_azure_spellbreaker") { }

        struct npc_azure_spellbreakerAI : public violet_hold_trashAI
        {
            npc_azure_spellbreakerAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiArcaneBlastTimer;
            uint32 uiSlowTimer;
            uint32 uiChainsOfIceTimer;
            uint32 uiConeOfColdTimer;

            void Reset() override
            {
                uiArcaneBlastTimer = 5000;
                uiSlowTimer = 4000;
                uiChainsOfIceTimer = 5000;
                uiConeOfColdTimer = 4000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (me->GetEntry() == NPC_AZURE_SPELLBREAKER_1)
                {
                    if (uiArcaneBlastTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,DUNGEON_MODE(SPELL_ARCANE_BLAST,SPELL_ARCANE_BLAST_H));
                        uiArcaneBlastTimer = 6000;
                    } else uiArcaneBlastTimer -= diff;

                    if (uiSlowTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,SPELL_SLOW);
                        uiSlowTimer = 5000;
                    } else uiSlowTimer -= diff;
                }

                if (me->GetEntry() == NPC_AZURE_SPELLBREAKER_2)
                {
                    if (uiChainsOfIceTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target,SPELL_CHAINS_OF_ICE);
                        uiChainsOfIceTimer = 7000;
                    } else uiChainsOfIceTimer -= diff;

                    if (uiConeOfColdTimer <= diff)
                    {
                        DoCast(DUNGEON_MODE(SPELL_CONE_OF_COLD,SPELL_CONE_OF_COLD_H));
                        uiConeOfColdTimer = 5000;
                    } else uiConeOfColdTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_spellbreakerAI(creature);
        }
};

class npc_azure_captain : public CreatureScript
{
    public:
        npc_azure_captain() : CreatureScript("npc_azure_captain") { }

        struct  npc_azure_captainAI : public violet_hold_trashAI
        {
            npc_azure_captainAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiMortalStrikeTimer;
            uint32 uiWhirlwindTimer;

            void Reset() override
            {
                uiMortalStrikeTimer = 5000;
                uiWhirlwindTimer = 8000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (uiMortalStrikeTimer <= diff)
                {
                    DoCast(me->GetVictim(),SPELL_MORTAL_STRIKE);
                    uiMortalStrikeTimer = 5000;
                } else uiMortalStrikeTimer -= diff;

                if (uiWhirlwindTimer <= diff)
                {
                    DoCast(me,SPELL_WHIRLWIND_OF_STEEL);
                    uiWhirlwindTimer = 8000;
                } else uiWhirlwindTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_captainAI(creature);
        }
};

class npc_azure_sorceror : public CreatureScript
{
    public:
        npc_azure_sorceror() : CreatureScript("npc_azure_sorceror") { }

        struct  npc_azure_sorcerorAI : public violet_hold_trashAI
        {
            npc_azure_sorcerorAI(Creature* creature) : violet_hold_trashAI(creature) { }

            uint32 uiArcaneStreamTimer;
            uint32 uiArcaneStreamTimerStartingValueHolder;
            uint32 uiManaDetonationTimer;

            void Reset() override
            {
                uiArcaneStreamTimer = 4000;
                uiArcaneStreamTimerStartingValueHolder = uiArcaneStreamTimer;
                uiManaDetonationTimer = 5000;
            }

            void UpdateAI(uint32 diff) override
            {
                violet_hold_trashAI::UpdateAI(diff);
                npc_escortAI::UpdateAI(diff);

                if (!UpdateVictim())
                    return;

                if (uiArcaneStreamTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target,DUNGEON_MODE(SPELL_ARCANE_STREAM,SPELL_ARCANE_STREAM_H));
                    uiArcaneStreamTimer = urand(0, 5000)+5000;
                    uiArcaneStreamTimerStartingValueHolder = uiArcaneStreamTimer;
                } else uiArcaneStreamTimer -= diff;

                if (uiManaDetonationTimer <= diff && uiArcaneStreamTimer >=1500 && uiArcaneStreamTimer <= uiArcaneStreamTimerStartingValueHolder/2)
                {
                    DoCast(DUNGEON_MODE(SPELL_MANA_DETONATION,SPELL_MANA_DETONATION_H));
                    uiManaDetonationTimer = urand(2000, 6000);
                } else uiManaDetonationTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_sorcerorAI(creature);
        }
};

class npc_violet_hold_arcane_sphere : public CreatureScript
{
    public:
        npc_violet_hold_arcane_sphere() : CreatureScript("npc_violet_hold_arcane_sphere") { }

        struct npc_violet_hold_arcane_sphereAI : public ScriptedAI
        {
            npc_violet_hold_arcane_sphereAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() override { }
            void AttackStart(Unit* /*who*/) override { }
            void EnterCombat(Unit* /*who*/) override { }
            void EnterEvadeMode() override { }
            void UpdateAI(uint32 /*diff*/) override { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_violet_hold_arcane_sphereAI(creature);
        }
};

class go_activation_crystal : public GameObjectScript
{
    public:
        go_activation_crystal() : GameObjectScript("go_activation_crystal") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go) override
        {
            if (InstanceScript* instance = go->GetInstanceScript())
                instance->SetData(DATA_ACTIVATE_CRYSTAL, 1);

            return false;
        }
};

// Arcane Lightning 57912, 58152
class spell_violet_hold_arcane_lightning : public SpellScript
{
    PrepareSpellScript(spell_violet_hold_arcane_lightning);

    void CorrectTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target && !IsInvader(target->GetEntry()); });
    }

    private:
        bool IsInvader(uint32 npc_entry)
        {
            for (auto&& itr : invadersType)
                if (itr == npc_entry)
                    return true;

            return false;
        }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_violet_hold_arcane_lightning::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

void AddSC_violet_hold()
{
    new npc_sinclari_vh();
    new npc_teleportation_portal_vh();
    new npc_azure_invader();
    new npc_azure_spellbreaker();
    new npc_azure_binder();
    new npc_azure_mage_slayer();
    new npc_azure_captain();
    new npc_azure_sorceror();
    new npc_azure_raider();
    new npc_azure_stalker();
    new npc_azure_saboteur();
    new npc_violet_hold_arcane_sphere();
    new go_activation_crystal();
    register_spell_script(spell_violet_hold_arcane_lightning);
}
