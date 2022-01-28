#include "Resolver.hpp"
#include "ragebot.hpp"
#include "../../helpers/utils.hpp"
#include "../../valve_sdk/interfaces/IVEngineClient.hpp"
#include "../../valve_sdk/misc/Enums.hpp"
#include "../../helpers/math.hpp"
#include "../../menu.hpp"
#include "../../options.hpp"
Resolver g_Resolver;


void HandleHits(C_BasePlayer* pEnt)
{
	auto NetChannel = g_EngineClient->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (Globals::Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			g_Resolver.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_GlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_EngineClient->GetNetChannel()->m_nChokedPackets);
			init[pEnt->EntIndex()] = false;
		}

		if (g_GlobalVars->curtime > predTime[pEnt->EntIndex()] && !Globals::Hit[pEnt->EntIndex()])
		{
			Globals::MissedShots[pEnt->EntIndex()] += 1;
			Globals::Shot[pEnt->EntIndex()] = false;
		}
		else if (g_GlobalVars->curtime <= predTime[pEnt->EntIndex()] && Globals::Hit[pEnt->EntIndex()])
			Globals::Shot[pEnt->EntIndex()] = false;

	}
	else
		init[pEnt->EntIndex()] = true;

	Globals::Hit[pEnt->EntIndex()] = false;
}


void Resolver::BruteForce(C_BasePlayer* Player, CBasePlayerAnimStatenew* Animstate)
{
	if (!Player || !Player->IsAlive() || !Player->IsEnemy() || !Animstate)
		return;

	int MissedShots = Globals::ShotsFired[Player->EntIndex()] - Globals::ShotsHit[Player->EntIndex()];
	static float OldYaw = Player->m_angEyeAngles().yaw;
	float Back = Math::NormalizeYaw(Math::CalcAngle(g_LocalPlayer->m_vecOrigin(), Player->m_vecOrigin()).yaw + 180.f);
	float EyeDelta = fabs(Math::NormalizeYaw(Player->m_angEyeAngles().yaw - OldYaw));
	float AntiSide = 0.f;
	float Brute = 0.f;

	Globals::ForceSafePoint[Player->EntIndex()] = false;
	if (Globals::UseFreestand[Player->EntIndex()] && MissedShots <= 1 && EyeDelta < 45.f)
	{
		Brute = Math::NormalizeYaw(Back + Globals::LastFreestandAngle[Player->EntIndex()]);
	}
	else if (EyeDelta >= 45.f)
	{
		Globals::ForceSafePoint[Player->EntIndex()] = true;
	}
	else
	{
		switch ((MissedShots - 2) % 2)
		{
		case 0:
			if (Math::NormalizeYaw(Player->m_angEyeAngles().yaw - Back) > 0.f)
			{
				AntiSide = 90.f;
			}
			else if (Math::NormalizeYaw(Player->m_angEyeAngles().yaw - Back) < 0.f)
			{
				AntiSide = -90.f;
			}
			break;

		case 1:
			if (Math::NormalizeYaw(Player->m_angEyeAngles().yaw - Back) > 0.f)
			{
				AntiSide = -90.f;
			}
			else if (Math::NormalizeYaw(Player->m_angEyeAngles().yaw - Back) < 0.f)
			{
				AntiSide = 90.f;
			}

			break;
		}
		Brute = Math::NormalizeYaw(Animstate->m_flGoalFeetYaw + AntiSide);
	}
	OldYaw = Player->m_angEyeAngles().yaw;
	Animstate->m_flGoalFeetYaw = Brute;
}

float Resolver::get_standing_yaw(C_BasePlayer* player)
{
	auto get_max_delta = [=]() -> float
	{
		const auto animstate = uintptr_t(player->GetPlayerAnimState());

		const float duck_amount = *(float*)(animstate + 0xA4);
		const float speed_fraction = std::max(0.f, std::min(*reinterpret_cast<float*>(animstate + 0xF8), 1.f));

		const float speed_factor = std::max(0.f, std::min(1.f, *reinterpret_cast<float*> (animstate + 0xFC)));

		const float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speed_fraction;
		float unk2 = unk1 + 1.f;

		if (duck_amount > 0)
			unk2 += ((duck_amount * speed_factor) * (0.5f - unk2));

		const float unk3 = *(float*)(animstate + 0x334) * unk2;
		return unk3;
	};

	auto current_resolver_info = player_res_info[player->EntIndex()];

	float lby = player->m_flLowerBodyYawTarget(); //fake, unless shitty desync
	float eye_angs = player->m_angEyeAngles().yaw; //mostly fake

	const std::vector<float> magic_numbers = { 0, get_max_delta(), -get_max_delta(), 114, -114, 156, -156 };
	auto current_shots = current_resolver_info.shots_missed % magic_numbers.size();
	return (lby + magic_numbers.at(current_shots));
}

float Resolver::get_moving_yaw(C_BasePlayer* player)
{
	return player->m_flLowerBodyYawTarget();
}

void Resolver::OnCreateMove() // cancer v2
{
	if (!g_Options.rage_resolver)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	if (!g_LocalPlayer->m_hActiveWeapon() || g_LocalPlayer->IsKnifeorNade())
		return;


	for (size_t i = 1; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		CBasePlayerAnimStatenew* Animstate = nullptr;
		if (!entity)
			continue;

		if (!entity->IsAlive() || entity->IsDormant())
			continue;
		bool Legit = (TIME_TO_TICKS(entity->m_flSimulationTime() - entity->m_flOldSimulationTime()) <= 1);
		if (g_Options.rage_resolver && !Legit)
			BruteForce(entity, Animstate);

		auto pPlayerEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(i));


		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g_LocalPlayer
			|| pPlayerEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = Math::CalcAngle(g_LocalPlayer->GetOrigin(), pPlayerEntity->GetOrigin()).yaw;
			Vector ViewPoint = g_LocalPlayer->GetOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(Math::GRD_TO_BOG(angToLocal))),(45 * cos(Math::GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(Math::GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(Math::GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(Math::GRD_TO_BOG(angToLocal))),(50 * cos(Math::GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(Math::GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(Math::GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (CAutoWall::Get().CanHit(OriginAutowall, ViewPoint))
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (CAutoWall::Get().CanHit(OriginAutowall222, OriginAutowall2))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}

		entity->m_angEyeAngles().yaw = (entity->m_vecVelocity().Length2D() > 0) ? get_moving_yaw(entity) : get_standing_yaw(entity);
	}

}

void Resolver::FrameStage(ClientFrameStage_t stage)
{
	if (!g_LocalPlayer || !g_EngineClient->IsInGame())
		return;

	CCSGOPlayerAnimState* g_Animstate;

	static bool  wasDormant[65];

	for (size_t i = 1; i < g_EngineClient->GetMaxClients(); i++)
	{
		auto pPlayerEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(i));


		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->IsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity);

		}

		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != g_LocalPlayer)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}