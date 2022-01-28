#include "Misc.h"
#include "../helpers/config.hpp"
#include "../options.hpp"
#include <string>
#include <time.h>
#include <chrono>
#include "../helpers/input.hpp"
#include "../Korumalar/Atakan75.hpp"
#include "../render.hpp"

std::vector<std::string> ChatSpam =
{
	"Xhanger | Premium CS:GO Cheats",
	"Get Good Get Xhanger",
	"Get Xhanger Here xhangeryazilim.com",
	"We dont get any VAC Alert cause we are using our own methods."
};

bool RestartBuyBot = false;
C_BasePlayer* LocalPlayer;

void Misc::SolEl()
{


	static auto left_knife = g_CVar->FindVar("cl_righthand");

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
	{
		left_knife->SetValue(1);
		return;
	}

	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon) return;

	left_knife->SetValue(!weapon->IsKnife());
}

void Misc::MoonWalk(CUserCmd* pCmd) {
	if (g_Options.maykilceksin && (g_LocalPlayer->GetMoveType() != MOVETYPE_LADDER && g_LocalPlayer->GetMoveType() != MOVETYPE_FLY))
	{
		if (pCmd->forwardmove > 0)
		{
			pCmd->buttons |= IN_BACK;
			pCmd->buttons &= ~IN_FORWARD;
		}

		if (pCmd->forwardmove < 0)
		{
			pCmd->buttons |= IN_FORWARD;
			pCmd->buttons &= ~IN_BACK;
		}

		if (pCmd->sidemove < 0)
		{
			pCmd->buttons |= IN_MOVERIGHT;
			pCmd->buttons &= ~IN_MOVELEFT;
		}

		if (pCmd->sidemove > 0)
		{
			pCmd->buttons |= IN_MOVELEFT;
			pCmd->buttons &= ~IN_MOVERIGHT;
		}
	}

}

void Misc::SkyChanger()
{
	/*ConVar* sv_skyname = g_CVar->FindVar("sv_skyname");

	auto sv_skyname_backup = g_CVar->FindVar("sv_skyname")->GetString();

	if (!g_Options.skychanger)
		sv_skyname->SetValue(sv_skyname_backup);

	switch (g_Options.skychanger_mode)
	{
	case 0: //Baggage
		sv_skyname->SetValue("cs_baggage_skybox");
		break;
	case 1: //Tibet
		sv_skyname->SetValue("cs_tibet");
		break;
	case 2: //Embassy
		sv_skyname->SetValue("embassy");
		break;
	case 3: //Italy
		sv_skyname->SetValue("italy");
		break;
	case 4: //Daylight 1
		sv_skyname->SetValue("sky_cs15_daylight01_hdr");
		break;
	case 5: //Daylight 2
		sv_skyname->SetValue("sky_cs15_daylight02_hdr");
		break;
	case 6: //Daylight 3
		sv_skyname->SetValue("sky_cs15_daylight03_hdr");
		break;
	case 7: //Daylight 4
		sv_skyname->SetValue("sky_cs15_daylight04_hdr");
		break;
	case 8: //Cloudy
		sv_skyname->SetValue("sky_csgo_cloudy01");
		break;
	case 9: //Night 1
		sv_skyname->SetValue("sky_csgo_night02");
		break;
	case 10: //Night 2
		sv_skyname->SetValue("sky_csgo_night02b");
		break;
	case 11: //Night Flat
		sv_skyname->SetValue("sky_csgo_night_flat");
		break;
	case 12: //Day HD
		sv_skyname->SetValue("sky_day02_05_hdr");
		break;
	case 13: //Day
		sv_skyname->SetValue("sky_day02_05");
		break;
	case 14: //Rural
		sv_skyname->SetValue("sky_l4d_rural02_ldr");
		break;
	case 15: //Vertigo HD
		sv_skyname->SetValue("vertigo_hdr");
		break;
	case 16: //Vertigo Blue HD
		sv_skyname->SetValue("vertigoblue_hdr");
		break;
	case 17: //Vertigo
		sv_skyname->SetValue("vertigo");
		break;
	case 18: //Vietnam
		sv_skyname->SetValue("vietnam");
		break;
	case 19: //Dusty Sky
		sv_skyname->SetValue("sky_dust");
		break;
	case 20: //Jungle
		sv_skyname->SetValue("jungle");
		break;
	case 21: //Nuke
		sv_skyname->SetValue("nukeblank");
		break;
	case 22: //Office
		sv_skyname->SetValue("office");
		break;
	default:
		break;
	}*/
}

void Misc::SohbetSpam()
{


	static float nextTime = 0.f;
	float flServerTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	if (nextTime > flServerTime)
		return;

	nextTime = flServerTime + 0.5f;
	if (g_Options.misc_chatspam == 2)
		g_EngineClient->ClientCmd(std::string(std::string("say ") + g_Options.spam).c_str());
	else
		g_EngineClient->ClientCmd(std::string(std::string("say ") + ChatSpam[rand() % 4]).c_str());
}

bool Misc::RemoveSleeves(const char* modelName)
{
	return g_Options.misc_nosleeves && std::strstr(modelName, "sleeve");
}

void Misc::AutoDefuse(C_BaseEntity* pLocal, CUserCmd* pCmd)
{
	// Abort if you are not a counter-terrorist //
	if (!(pLocal->m_iTeamNum() == 3))
		return;

	// Loop through entities in the world //
	for (int i = 1; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		// Saving the entity for use //
		C_BaseEntity* pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
		// Continue if the entity exists //
		if (!pEntity)
			continue;
		// Pretty self-explanatory //
		if (pEntity->IsDormant())
			continue;
		// Filter entities to get the planted bomb //
		if (pEntity->GetClientClass()->m_ClassID != ClassId_CPlantedC4)
			continue;
		// Making sure that the bomb is not already defused //
		if (((C_PlantedC4*)pEntity)->m_bBombDefused())
			continue;

		// Retrieve the distance from you to the bomb //
		float fDistance = pLocal->m_vecOrigin().DistTo(pEntity->m_vecOrigin());

		// Continue if you are within a reasonable distance & field of view //
		if (fDistance <= 12.f)
		{
			// Defuse the bomb derp //
			pCmd->buttons |= IN_USE;
		}
	}
}

void Misc::RemoveCrouchCooldown(CUserCmd* cmd)
{
	if (g_Options.misc_fastduck)
		cmd->buttons |= IN_BULLRUSH;
}

void Misc::DisablePanoramaBlur()
{
	ConVar* blur = g_CVar->FindVar("@panorama_disable_blur");
	blur->SetValue(1);

}

void Misc::Kick(int userid)
{
	char command[1024]; sprintf(command, "callvote kick %d", userid);

	g_EngineClient->ExecuteClientCmd(command);
}