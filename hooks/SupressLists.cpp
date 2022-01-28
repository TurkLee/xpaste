#include "../hooks.hpp"
#include "../options.hpp"
#include "../features/animfixed shit.h"
void resolve(C_BasePlayer* player);
extern bool sendpacket;
bool* disablePostProcessing = *(bool**)(Utils::PatternScan(GetModuleHandle("client.dll"), "83 EC 4C 80 3D") + 5);

extern QAngle vangle;
extern matrix3x4_t m_real_matrix[128];
#include "ClientMode.h"
int entityId = -1;

const char* PadStringRight(std::string text, size_t value)
{
	text.insert(text.length(), value - text.length(), ' ');

	return text.c_str();
}

bool CanSee[65];
#include "../features/skin/skins.h"
void fixanims(C_BasePlayer* player) {
	if (player->m_flOldSimulationTime() != player->m_flSimulationTime() && player->GetPlayerAnimState())
	{
		const float curtime = g_GlobalVars->curtime;
		const float frametime = g_GlobalVars->frametime;

		static auto host_timescale = g_CVar->FindVar(("host_timescale"));

		g_GlobalVars->frametime = g_GlobalVars->interval_per_tick * host_timescale->GetFloat();
		g_GlobalVars->curtime = player->m_flOldSimulationTime() + g_GlobalVars->interval_per_tick;

		Vector backup_origin = player->m_vecOrigin();
		Vector backup_absorigin = player->GetAbsOrigin();
		Vector backup_velocity = player->m_vecVelocity();
		int backup_flags = player->m_fFlags();
		int backup_eflags = player->get_eflags();

		AnimationLayer backup_layers[15];
		std::memcpy(backup_layers, player->GetAnimOverlays(), (sizeof(AnimationLayer) * player->GetNumAnimOverlays()));

		if (player->GetPlayerAnimState()->m_bOnGround)
		{
			player->m_fFlags() |= FL_ONGROUND;
		}
		else
		{
			player->m_fFlags() &= ~FL_ONGROUND;
		}
		player->get_eflags() &= ~0x1000;

		player->get_abs_velocity() = player->m_vecVelocity();

		if (player->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount == g_GlobalVars->framecount)
			player->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount = g_GlobalVars->framecount - 1;

		player->m_bClientSideAnimation() = true;

		player->UpdateClientSideAnimation();

		player->m_bClientSideAnimation() = false;

		std::memcpy(player->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * player->GetNumAnimOverlays()));
		player->m_vecOrigin() = backup_origin;
		player->SetAbsOrigin(backup_absorigin);
		player->m_vecVelocity() = backup_velocity;
		player->m_fFlags() = backup_flags;
		player->get_eflags() = backup_eflags;

		g_GlobalVars->curtime = curtime;
		g_GlobalVars->frametime = frametime;
	}
}
void ResolverLegit(C_BasePlayer* pl) {
	if (!pl->GetPlayerAnimState() || pl->m_vecVelocity().Length() > 0 || !g_Options.resolver)
		return;

	if (fabs(std::remainderf(pl->m_flLowerBodyYawTarget(), 360.f) - std::remainderf(pl->m_angEyeAngles().yaw, 360.f)) < 60)
		return;

	auto lbyfix = Math::ClampYaw(std::remainderf(pl->m_flLowerBodyYawTarget() + 180, 360.f));
	pl->GetPlayerAnimState()->m_flGoalFeetYaw = lbyfix;
}

void __stdcall Hooks::hkSuppressLists(int a2, bool a3) {
	static auto ofunc = partition_hook.get_original< SuppressLists >(16);
	static float SpawnTime = 0.0f;
	static auto OnRenderStart_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "FF 50 40 8B 1D ? ? ? ?") + 0x3;
	static auto FrameNetUpdateEnd_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "5F 5E 5D C2 04 00 83 3D ? ? ? ? ?");

	if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
		if (_ReturnAddress() == OnRenderStart_Return) {


			if (g_LocalPlayer->GetPlayerAnimState())
				g_LocalPlayer->SetAbsAngles(QAngle(0.0f, g_LocalPlayer->GetPlayerAnimState()->m_flGoalFeetYaw, 0.0f));
			auto state = g_LocalPlayer->GetPlayerAnimState();
			if (state)
				if (g_LocalPlayer->m_vecVelocity().Length2D() < 3.f)
					*reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (state) + 292) = 0.f;

			if (vangle.yaw && g_Input->m_fCameraInThirdPerson)
				g_LocalPlayer->GetVAngles() = vangle;

			g_LocalPlayer->m_flFlashMaxAlpha() = g_Options.flash;
			auto csm = g_CVar->FindVar("cl_csm_enabled");
			if (g_Options.esp_optimise) {

				csm->m_fnChangeCallbacks.m_Size = 0;
				csm->SetValue(false);
				if (disablePostProcessing)
					*disablePostProcessing = true;

			}
			else {
				csm->m_fnChangeCallbacks.m_Size = 0;
				g_CVar->FindVar("cl_csm_enabled")->SetValue(true);
				if (disablePostProcessing)
					*disablePostProcessing = false;

			}
			for (int i = 1; i < 65; i++)
			{
				auto ent = (C_BasePlayer*)C_BasePlayer::GetEntityByIndex(i);
				if (!ent || !ent->IsAlive())
					continue;
				CanSee[ent->EntIndex()] = g_LocalPlayer->CanSeePlayer(ent, HITBOX_CHEST, true);
			}

		}
		else if (_ReturnAddress() == FrameNetUpdateEnd_Return) {
			skins::on_frame_stage_notify(true);

			for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
			{
				auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
				if (ent && ent->IsAlive() && !ent->IsDormant() && ent != g_LocalPlayer) {
					ResolverLegit(ent);
					auto VarMap = reinterpret_cast<uintptr_t>(ent) + 36;
					auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

					for (auto index = 0; index < VarMapSize; index++)
						*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
				}
			}
		}
	}
	ofunc(g_SpatialPartition, a2, a3);
}