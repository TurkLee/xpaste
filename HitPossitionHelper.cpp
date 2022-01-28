#include "HitPossitionHelper.h"
#include "runtime_saver.h"
#include "options.hpp"

runtime_saver saver;

void HitPossitionHelper::OnFireEvent(IGameEvent* event)
{
	if (!event || !g_LocalPlayer)
	{
		return;
	}
	if (!strcmp(event->GetName(), "player_hurt"))
	{
		PlayerHurt(event);
	}
	if (!strcmp(event->GetName(), "bullet_impact"))
	{
		BulletImpact(event);
	}
}

void HitPossitionHelper::PlayerHurt(IGameEvent* event)
{
	//2nd

	C_BasePlayer* attacker = GetPlayer(event->GetInt("attacker"));
	C_BasePlayer* hurt = GetPlayer(event->GetInt("userid"));

	if (!attacker || !hurt || attacker != g_LocalPlayer)
	{
		return;
	}

	Vector EPosition = hurt->m_vecOrigin();
	ImpactInfoStruct BestImpact;
	float BestImpactDistance = -1.f;
	float Time = g_GlobalVars->realtime;
	for (auto info = saver.hitpositions.begin(); info != saver.hitpositions.end();)
	{
		if (Time > info->Time + (g_GlobalVars->interval_per_tick * 2))
		{
			info = saver.hitpositions.erase(info);
			continue;
		}

		Vector position = info->Pos;
		float Distance = EPosition.DistTo(position);

		BulletTracerInfoStruct binfo;
		binfo.DidHit = false;
		binfo.HitPos = info->Pos;
		binfo.ShotPos = info->ShotPos;
		binfo.Time = info->Time;
		//

		if (Distance < BestImpactDistance || BestImpactDistance == -1)
		{
			BestImpactDistance = Distance;
			BestImpact = *info;
			binfo.DidHit = true;
		}
		saver.BulletTracers.push_back(binfo);

		info++;
	}

	if (BestImpactDistance == -1)
	{
		return;
	}
}

void HitPossitionHelper::BulletImpact(IGameEvent* event)
{
	//1st
	C_BasePlayer* shooter = GetPlayer(event->GetInt("userid"));

	if (!shooter || shooter != g_LocalPlayer)
	{
		return;
	}

	ImpactInfoStruct info;
	info.Pos = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
	info.ShotPos = g_LocalPlayer->m_vecOrigin() + g_LocalPlayer->m_vecViewOffset();
	info.Time = g_GlobalVars->realtime;
	info.Tick = g_GlobalVars->tickcount;
	saver.hitpositions.push_back(info);
}

C_BasePlayer* HitPossitionHelper::GetPlayer(int i)
{
	return static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(i)));
}

void HitPossitionHelper::ShotTracer(Vector shot_pos, Vector hit_pos)
{
	if (!g_LocalPlayer || !g_Options.visuals_bullet_tracer)
	{
		return;
	}

	Color clr = Color(g_Options.color_bullet_tracer);

	auto start_pos = shot_pos;

	if (!start_pos.IsValid() || !hit_pos.IsValid() || start_pos.DistTo(hit_pos) < 0.1f)
		return;

	start_pos.z -= 4.f;

	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = "sprites/physbeam.vmt";
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 1.f;
	beamInfo.m_flWidth = 2.0f;
	beamInfo.m_flEndWidth = 2.0f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = 255.f;
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = clr.r();
	beamInfo.m_flGreen = clr.g();
	beamInfo.m_flBlue = clr.b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = FBEAM_FADEIN | FBEAM_FADEOUT;
	beamInfo.m_vecStart = start_pos;
	beamInfo.m_vecEnd = hit_pos;

	auto beam = g_RenderBeams->CreateBeamPoints(beamInfo);
	if (beam)
	{
		g_RenderBeams->DrawBeam(beam);
	}
}