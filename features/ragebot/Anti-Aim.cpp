#include "Anti-Aim.hpp"
#include <time.h>

#define TICKS_TO_TIME(t) (g_GlobalVars->interval_per_tick * (t) )

#define M_PHI 1.61803398874989484820f // golden ratio

#define RandomInt(min, max) (rand() % (max - min + 1) + min)
bool aaSide = false;
bool sendPacket = true;
bool aimstepInProgress = false;

float get_estimate_server_time(CUserCmd* cmd) {
	const auto net_chan = static_cast<INetChannelInfo*> (g_EngineClient->GetNetChannelInfo());

	const auto local_latency = net_chan->GetAvgLatency(INetChannelInfo::LOCALPLAYER);
	const auto generic_latency = net_chan->GetAvgLatency(INetChannelInfo::GENERIC);

	return local_latency + generic_latency + TICKS_TO_TIME(1) + TICKS_TO_TIME(cmd->tick_count);
}

namespace AntiAims
{
	bool set_curtime_after_prediction = false;
	bool in_lby_break = false;
	bool ShouldSwitch{ false };
	float LastPreYaw{ 0.f };
	float NextLBYUpdateTime{ 0.f };

	float Distance(Vector a, Vector b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	}

	bool GetBestHeadAngle(QAngle& angle)
	{
		Vector position = g_LocalPlayer->m_vecOrigin() + g_LocalPlayer->m_vecViewOffset();

		float closest_distance = 100.0f;

		float radius = g_Options.antiaim_edge_dist + 0.1f;
		float step = M_PI * 2.0 / 8;

		for (float a = 0; a < (M_PI * 2.0); a += step)
		{
			Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

			Ray_t ray;
			trace_t tr;
			ray.Init(position, location);

			CTraceFilterWorldOnly traceFilter;

			//CTraceFilter traceFilter;
			//traceFilter.pSkip = g_LocalPlayer;

			g_EngineTrace->TraceRay(ray, 0x4600400B, &traceFilter, &tr);

			float distance = Distance(position, tr.endpos);

			if (distance < closest_distance)
			{
				closest_distance = distance;
				angle.yaw = RAD2DEG(a);
			}
		}

		return closest_distance < g_Options.antiaim_edge_dist;
	}

	void DoAntiAimY(QAngle& angle, int command_number, bool bFlip, bool& clamp) // pitch
	{
		int aa_type = g_Options.antiaim_pitch;

		static float pDance = 0.0f;

		switch (aa_type)
		{
			// lol
		case ANTIAIM_PITCH_NONE:
			break;

		case ANTIAIM_PITCH_DOWN:
			angle.pitch = 89.0f;
			break;
		case ANTIAIM_PITCH_UP:
			angle.pitch = -89.0f;
			break;
		case ANTIAIM_PITCH_RANDOM:
			angle.pitch = RandomInt(-89, 89);
			break;
		case ANTIAIM_PITCH_DANCE:
			pDance += 45.0f;
			if (pDance > 100)
				pDance = 0.0f;
			else if (pDance > 75.f)
				angle.pitch = -89.f;
			else if (pDance < 75.f)
				angle.pitch = 89.f;
			break;
		case ANTIAIM_PITCH_FAKEUP:
			angle.pitch = bFlip ? 89.0f : -89.0f;
			break;
		case ANTIAIM_PITCH_FAKEDOWN:
			angle.pitch = bFlip ? -89.0f : 89.0f;
			break;
		}
	}

	float MaxBodyYawyaw(C_BasePlayer* pl) {

		if (!pl)
			return 0.f;

		auto anim_state = pl->GetPlayerAnimState();
		if (!anim_state)
			return 0.f;

		float duck_amount = anim_state->m_fDuckAmount();
		float speed_fraction = std::max< float >(0, std::min< float >(anim_state->m_flFeetSpeedForwardsOrSideWays(), 1));
		float speed_factor = std::max< float >(0, std::min< float >(1, anim_state->m_flFeetSpeedUnknownForwardOrSideways()));

		float yaw_modifier = (((anim_state->m_flStopToFullRunningFraction() * -0.3f) - 0.2f) * speed_fraction) + 1.0f;

		if (duck_amount > 0.f) {
			yaw_modifier += ((duck_amount * speed_factor) * (0.5f - yaw_modifier));
		}

		return anim_state->m_vVelocityY() * yaw_modifier;
	}


	void DoAntiAimX(QAngle& angle, bool bFlip, bool& clamp) // yaw
	{
		int aa_type = bFlip ? g_Options.antiaim_yaw_fake : g_Options.antiaim_yaw;

		static bool yFlip;
		float temp;
		int random;
		int maxJitter;
		//	LbyUpdate();
		switch (aa_type)
		{
			// xD
		case ANTIAIM_YAW_NONE:
			break;

		case ANTIAIM_YAW_SIDE:
			yFlip ? angle.yaw += 90.f : angle.yaw -= 90.0f;
			yFlip = !yFlip;
			break;
		case ANTIAIM_YAW_BACKWARDS:
			angle.yaw += 180.0f + MaxBodyYawyaw(g_LocalPlayer);
			break;
		case ANTIAIM_YAW_RIGHT:
			angle.yaw -= 90.0f;
			break;
		case ANTIAIM_YAW_LEFT:
			angle.yaw += 90.0f;
			break;
		case ANTIAIM_YAW_STATIC_FORWARD:
			angle.yaw = 0.0f;
			break;
		case ANTIAIM_YAW_STATIC_BACKWARDS:
			angle.yaw = 180.0f + MaxBodyYawyaw(g_LocalPlayer);
			break;
		case ANTIAIM_YAW_STATIC_RIGHT:
			angle.yaw = -90.0f;
			break;
		case ANTIAIM_YAW_STATIC_LEFT:
			angle.yaw = 90.0f;
			break;
		case ANTIAIM_YAW_FAKE_LBY1:
			bFlip ? angle.yaw += g_LocalPlayer->m_flLowerBodyYawTarget() - 90.0f : angle.yaw -= g_LocalPlayer->m_flLowerBodyYawTarget() + 90.0f;
			break;
		case ANTIAIM_YAW_FAKE_LBY2:
			bFlip ? angle.yaw += g_LocalPlayer->m_flLowerBodyYawTarget() - RandomInt(180, 360) : angle.yaw -= g_LocalPlayer->m_flLowerBodyYawTarget() + RandomInt(30, 61);
			break;
		case ANTIAIM_YAW_SPIN:
			angle.yaw = fmodf(g_GlobalVars->curtime * ((360.0f / M_PHI) * g_Options.antiaim_spin_speed), 360.0f);
			break;
		case ANTIAIM_YAW_RANDOM:
			angle.yaw = RandomInt(-180, 180);
			break;
		case ANTIAIM_YAW_JITTER:
			yFlip ? angle.yaw -= 90.0f : angle.yaw -= 270.0f;
			yFlip = !yFlip;
			break;
		case ANTIAIM_YAW_BACKJITTER:
			angle.yaw -= 180;
			random = rand() % 100;
			maxJitter = rand() % (85 - 70 + 1) + 70;
			temp = maxJitter - (rand() % maxJitter);
			if (random < 35 + (rand() % 15))
				angle.yaw -= temp;
			else if (random < 85 + (rand() % 15))
				angle.yaw += temp;
			break;
		case ANTIAIM_YAW_FAKE_SIDE_LBY:
			bFlip ? angle.yaw += g_LocalPlayer->m_flLowerBodyYawTarget() - (fmodf(g_GlobalVars->curtime * ((360.0f / M_PHI) * g_Options.antiaim_spin_speed), 360.0f)) : angle.yaw -= g_LocalPlayer->m_flLowerBodyYawTarget() + 90.0f;
			break;
		}
	}

	void OnCreateMove(CUserCmd* cmd)
	{
		if (!aimstepInProgress)
		{
			QAngle oldAngle = cmd->viewangles;
			float oldForward = cmd->forwardmove;
			float oldSideMove = cmd->sidemove;

			QAngle angle = cmd->viewangles;

			auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();
			if (!weapon)
				return;

			auto weapon_data = weapon->GetCSWeaponData();

			if (weapon_data->iWeaponType == WEAPONTYPE_GRENADE) {
				if (!weapon->m_bPinPulled()) {
					float throwTime = weapon->m_fThrowTime();
					if (throwTime > 0.f)
						return;
				}

				if ((cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_ATTACK2)) {
					if (weapon->m_fThrowTime() > 0.f)
						return;
				}
			}


			auto localplayer = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
			if (!localplayer)
				return;

			auto activeWeapon = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hActiveWeapon());
			if (!activeWeapon)
				return;


			if (!g_Options.antiaim_knife && (activeWeapon->GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE))
				sendPacket = true;

			if ((cmd->buttons & IN_USE) || (cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_GRENADE1) || (cmd->buttons & IN_GRENADE2) || (cmd->buttons & IN_ATTACK2 && activeWeapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER))
				sendPacket = true;

			else if (!(cmd->buttons & IN_USE) && !(cmd->buttons & IN_ATTACK) && !(cmd->buttons & IN_GRENADE1) && !(cmd->buttons & IN_GRENADE2) && !(cmd->buttons & IN_ATTACK2 && activeWeapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER))
			{
				auto weapon = g_LocalPlayer->m_hActiveWeapon();
				if (weapon) {
					if (g_Options.antiaim_knife && !(activeWeapon->GetCSWeaponData()->iWeaponType == WEAPONTYPE_GRENADE))
					{
						QAngle edge_angle = angle;
						bool edging_head = g_Options.antiaim_edge_dist > 0.0f && GetBestHeadAngle(edge_angle);

						static bool bFlip;
						bFlip = !bFlip;

						// Untrusted Anti-Aim Check // SET TO PREVIOUS OPTION IF ANTIAIM IS UNTRUSTED
						if (g_Options.antiuntrusted)
						{
							// UT yaws
							// real yaws
							if (g_Options.antiaim_yaw == ANTIAIM_YAW_JITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							if (g_Options.antiaim_yaw == ANTIAIM_YAW_BACKJITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							// fake yaws
							if (g_Options.antiaim_yaw_fake == ANTIAIM_YAW_JITTER)
								g_Options.antiaim_yaw_fake = ANTIAIM_YAW_FAKE_LBY2;

							if (g_Options.antiaim_yaw == ANTIAIM_YAW_BACKJITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							// UT pitches
							if (g_Options.antiaim_pitch == ANTIAIM_PITCH_FAKEUP)
								g_Options.antiaim_pitch = ANTIAIM_PITCH_DANCE;
						}

						if (g_Options.tankAntiaimKey == 0)
						{
							// pitch antiaim
							DoAntiAimY(angle, cmd->command_number, bFlip, g_Options.antiuntrusted);

							if (g_Options.fakelag_factor == 0.0f) sendPacket = bFlip;
							if (g_Options.antiaim_edge_dist > 0.0f && edging_head) angle.yaw = edge_angle.yaw;

							// yaw antiaim
							DoAntiAimX(angle, bFlip, g_Options.antiuntrusted);

							Math::NormalizeAngles(angle);
						}
						else if (g_Options.tankAntiaimKey != 0)
						{
							// aaSide - false == left, true == right

							// pitch antiaim
							DoAntiAimY(angle, cmd->command_number, bFlip, g_Options.antiuntrusted);

							if (g_Options.fakelag_factor == 0.0f) sendPacket = bFlip;
							if (g_Options.antiaim_edge_dist > 0.0f && edging_head) angle.yaw = edge_angle.yaw;

							// right
							if (aaSide)
							{
								// fake left
								if (sendPacket) angle.yaw += 90;

								// real right
								if (!sendPacket) angle.yaw -= 90;
							}

							// left
							if (!aaSide)
							{
								// real left
								if (!sendPacket) angle.yaw += 90;

								// fake right
								if (sendPacket) angle.yaw -= 90;
							}

							Math::NormalizeAngles(angle);
						}

						if (g_Options.antiuntrusted)
						{
							Math::NormalizeAngles(angle);
							Math::ClampAngles(angle);
						}

						cmd->viewangles = angle;

						if (g_Options.antiaim_antiresolver)
						{
							static bool antiResolverFlip = false;
							if (cmd->viewangles.yaw == g_LocalPlayer->m_flLowerBodyYawTarget())
							{
								if (antiResolverFlip) cmd->viewangles.yaw += 60.f;
								else cmd->viewangles.yaw -= 60.f;

								antiResolverFlip = !antiResolverFlip;

								if (g_Options.antiuntrusted)
								{
									Math::NormalizeAngles(cmd->viewangles);
									Math::ClampAngles(cmd->viewangles);
								}
							}
						}
					}
					else if (!(activeWeapon->GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE) || !(activeWeapon->GetCSWeaponData()->iWeaponType == WEAPONTYPE_GRENADE))
					{
						QAngle edge_angle = angle;
						bool edging_head = g_Options.antiaim_edge_dist > 0.0f && GetBestHeadAngle(edge_angle);

						static bool bFlip;
						bFlip = !bFlip;

						// Untrusted Anti-Aim Check // SET TO PREVIOUS OPTION IF ANTIAIM IS UNTRUSTED
						if (g_Options.antiuntrusted)
						{
							// UT yaws
							// real yaws
							if (g_Options.antiaim_yaw == ANTIAIM_YAW_JITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							if (g_Options.antiaim_yaw == ANTIAIM_YAW_BACKJITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							// fake yaws
							if (g_Options.antiaim_yaw_fake == ANTIAIM_YAW_JITTER)
								g_Options.antiaim_yaw_fake = ANTIAIM_YAW_FAKE_LBY2;

							if (g_Options.antiaim_yaw == ANTIAIM_YAW_BACKJITTER)
								g_Options.antiaim_yaw = ANTIAIM_YAW_FAKE_LBY2;

							// UT pitches
							if (g_Options.antiaim_pitch == ANTIAIM_PITCH_FAKEUP)
								g_Options.antiaim_pitch = ANTIAIM_PITCH_DANCE;
						}

						if (g_Options.tankAntiaimKey == 0)
						{
							// pitch antiaim
							DoAntiAimY(angle, cmd->command_number, bFlip, g_Options.antiuntrusted);

							if (g_Options.fakelag_factor == 0.0f) sendPacket = bFlip;
							if (g_Options.antiaim_edge_dist > 0.0f && edging_head) angle.yaw = edge_angle.yaw;

							// yaw antiaim
							DoAntiAimX(angle, bFlip, g_Options.antiuntrusted);

							Math::NormalizeAngles(angle);
						}
						else if (g_Options.tankAntiaimKey != 0)
						{
							// aaSide - false == left, true == right

							// pitch antiaim
							DoAntiAimY(angle, cmd->command_number, bFlip, g_Options.antiuntrusted);

							if (g_Options.fakelag_factor == 0.0f) sendPacket = bFlip;
							if (g_Options.antiaim_edge_dist > 0.0f && edging_head) angle.yaw = edge_angle.yaw;

							// right
							if (aaSide)
							{
								// fake right
								if (sendPacket) angle.yaw += 90;

								// real right
								if (!sendPacket) angle.yaw -= 90;
							}

							// left
							if (!aaSide)
							{
								// real left
								if (!sendPacket) angle.yaw += 90;

								// fake right
								if (sendPacket) angle.yaw -= 90;
							}

							Math::NormalizeAngles(angle);
						}

						// clamping angles
						if (g_Options.antiuntrusted)
						{
							Math::NormalizeAngles(angle);
							Math::ClampAngles(angle);
						}

						cmd->viewangles = angle;

						if (g_Options.antiaim_antiresolver)
						{
							static bool antiResolverFlip = false;
							if (cmd->viewangles.yaw == g_LocalPlayer->m_flLowerBodyYawTarget())
							{
								if (antiResolverFlip) cmd->viewangles.yaw += 60.f;
								else cmd->viewangles.yaw -= 60.f;

								antiResolverFlip = !antiResolverFlip;

								if (g_Options.antiuntrusted)
								{
									Math::NormalizeAngles(cmd->viewangles);
									Math::ClampAngles(cmd->viewangles);
								}
							}
						}

						/*
						if (g_Options.antiaim_thirdperson_angle == ANTIAIM_THIRDPERSON_BOTH)
							LastTickViewAngles = cmd->viewangles;
						*/

						//Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
					}
				}
			}
		}
	}
}