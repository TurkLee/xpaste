#include "ClientMode.h"
#include "../features/Misc.h"
#include "../features/legitbot/aimbot.hpp"
#include "../features/legitbot/backtrack.hpp"
#include "../features/triggerbot.hpp"
#include "../features/ragebot/ragebot.hpp"
#include "../features/ragebot/resolver.hpp"
#include "../runtime_saver.h"
#include <algorithm>

void AutoBlock(CUserCmd* cmd);
matrix3x4_t fakelag[128];

bool sendpacket = false;

void SetLocalPlayerReady()
{
	static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
	if (SetLocalPlayerReadyFn)
		SetLocalPlayerReadyFn("");
}
void aaccept() {
	typedef void(__cdecl* accept_t)(void);
	static accept_t accept = (accept_t)Utils::PatternScan(GetModuleHandleA("client.dll"),
		"55 8B EC 51 56 8B 35 ? ? ? ? 57 83 BE");

	if (accept && **(unsigned long**)((unsigned long)accept + 0x7)) {
		SetLocalPlayerReady();
	}
}

void autoaccept()
{
	auto match_session = g_match_framework->get_match_session();
	if (match_session) {
		auto session_settings = match_session->get_session_settings();
		if (session_settings) {

			//if (session_settings->GetString("game/mode", "competitive") || session_settings->GetString("game/mode", "wingman")) {

			std::string search_state = session_settings->GetStringxD("game/mmqueue", "");

			if (search_state == "reserved") {
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
				Sleep(500);
				aaccept();
			}
			//}
		}
	}
}
bool IsPlayerBehind(C_BasePlayer* player)
{
	Vector toTarget = (g_LocalPlayer->m_vecOrigin() - player->m_vecOrigin()).Normalized();
	Vector playerViewAngles;
	Math::AngleVectors(player->m_angEyeAngles(), playerViewAngles);
	if (toTarget.Dot(playerViewAngles) > -0.5f)
		return false;
	else
		return true;
}
void knifebot(CUserCmd* pCmd) {
	if (g_Options.kbot)
	{
		C_BaseCombatWeapon* pLocalWeapon = g_LocalPlayer->m_hActiveWeapon();
		if (pLocalWeapon && pLocalWeapon->GetCSWeaponData())
		{
			if (g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE)
			{

				for (int EntIndex = 1; EntIndex < g_EngineClient->GetMaxClients(); EntIndex++)
				{
					C_BasePlayer* Entity = (C_BasePlayer*)g_EntityList->GetClientEntity(EntIndex);

					if (!Entity)
						continue;

					if (!Entity->IsPlayer())
						continue;

					if (!Entity->IsAlive())
						continue;

					if (Entity == g_LocalPlayer)
						continue;

					float Distance = g_LocalPlayer->m_vecOrigin().DistTo(Entity->m_vecOrigin());

					if (Distance > 68)
						continue;

					if (Entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
						continue;

					Vector OrignWorld = Entity->m_vecOrigin();
					Vector OrignScreen;

					if (!Math::WorldToScreen(OrignWorld, OrignScreen))
						continue;

					static int cur_attack = 0;

					if (Distance > 60.f || Entity->m_iHealth() < 33)
						pCmd->buttons |= IN_ATTACK;
					else
					{
						if ((g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick) - pLocalWeapon->m_flNextPrimaryAttack() > 0)
						{
							if (!IsPlayerBehind(Entity)) {
								if (Entity->m_ArmorValue() > 0)
								{
									if (Entity->m_iHealth() > 61)
										pCmd->buttons |= IN_ATTACK;
									else
										pCmd->buttons |= IN_ATTACK2;
								}
								else
								{
									if (Entity->m_iHealth() < 33)
										pCmd->buttons |= IN_ATTACK;
									else
										pCmd->buttons |= IN_ATTACK2;
								}
							}
							else
								pCmd->buttons |= IN_ATTACK2;
						}

					}
				}
			}
		}
	}
}

QAngle vangle = QAngle();
float damage = 0.f;

void Desync(CUserCmd* cmd, bool& send_packet);

float anglereal = 0.f;
float anglefake = 0.f;
void fix_movement(CUserCmd* cmd, QAngle& wishangle)
{
	Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	Math::AngleVectors(wishangle, view_fwd, view_right, view_up);
	Math::AngleVectors(cmd->viewangles, cmd_fwd, cmd_right, cmd_up);

	const auto v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	const auto v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	const auto v12 = sqrtf(view_up.z * view_up.z);

	const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	const auto v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	const auto v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	const auto v18 = sqrtf(cmd_up.z * cmd_up.z);

	const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	const auto v22 = norm_view_fwd.x * cmd->forwardmove;
	const auto v26 = norm_view_fwd.y * cmd->forwardmove;
	const auto v28 = norm_view_fwd.z * cmd->forwardmove;
	const auto v24 = norm_view_right.x * cmd->sidemove;
	const auto v23 = norm_view_right.y * cmd->sidemove;
	const auto v25 = norm_view_right.z * cmd->sidemove;
	const auto v30 = norm_view_up.x * cmd->upmove;
	const auto v27 = norm_view_up.z * cmd->upmove;
	const auto v29 = norm_view_up.y * cmd->upmove;

	cmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
		+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	cmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
		+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	cmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
		+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	const auto ratio = 2.f - fmaxf(fabsf(cmd->sidemove), fabsf(cmd->forwardmove)) / 450.f;
	cmd->forwardmove *= ratio;
	cmd->sidemove *= ratio;

	wishangle = cmd->viewangles;
}

int max_choke_ticks() {

	int maxticks = (*g_GameRules && (*g_GameRules)->IsValveDS()) ? 11 : 15;
	static int max_choke_ticks = 0;
	static int latency_ticks = 0;
	float fl_latency = g_EngineClient->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	int latency = TIME_TO_TICKS(fl_latency);;
	if (g_ClientState->chokedcommands <= 0)
		latency_ticks = latency;
	else latency_ticks = std::max(latency, latency_ticks);

	if (fl_latency >= g_GlobalVars->interval_per_tick)
		max_choke_ticks = 11 - latency_ticks;
	else max_choke_ticks = 11;
	return max_choke_ticks;
}
void updatelby(CCSGOPlayerAnimState* animstate);

float GetFullLatency() {
	return g_EngineClient->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + g_EngineClient->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
}

void Clnatag() {

	static bool restore = false;
	static float lastchangetime = 0.0;
	if (g_Options.misc_clantag && g_LocalPlayer) {

		if (g_GlobalVars->curtime + (GetFullLatency() / 2) - lastchangetime >= 0.35f)
		{

			std::string Name;

			if (g_Options.misc_customclan && g_Options.customclan && g_Options.customclan[0] && g_Options.misc_clantaga)
				Name = g_Options.customclan;
			else
				if (!g_Options.misc_customclan)
					Name = _xor_("xhanger$ ").c_str();
				else
					Name = _xor_(" ").c_str();

			Utils::SetClantag(Name.substr(0, int(g_GlobalVars->curtime + (GetFullLatency() / 2)) % Name.length()).c_str());
			lastchangetime = (g_GlobalVars->curtime + (GetFullLatency() / 2));
			restore = true;
		}
	}
	else if (restore) {
		restore = false;
		Utils::SetClantag("");
	}

}
#define Square(x) ((x)*(x))
#define FastSqrt(x)	(sqrt)(x)

void Fakelag(CUserCmd* cmd, bool& send_packet) {
	if (!g_Options.fakelag_enabled)
		return;

	if (g_Options.fakelag_unducking &&
		g_LocalPlayer->m_flDuckAmount() > 0.05f && g_LocalPlayer->m_flDuckAmount() < 0.95f) {
		send_packet = !(g_ClientState->chokedcommands < max_choke_ticks());
		return;
	}

	if (g_Options.fakelag_factor <= 0)
		return;

	int choke_factor = g_Options.misc_desync ? std::min(max_choke_ticks(), g_Options.fakelag_factor) : g_Options.fakelag_factor;

	auto LegitPeek = [choke_factor](CUserCmd* cmd, bool& send_packet) {
		static bool m_bIsPeeking = false;
		if (m_bIsPeeking) {
			send_packet = !(g_ClientState->chokedcommands < choke_factor);
			if (send_packet)
				m_bIsPeeking = false;
			return;
		}

		auto speed = g_LocalPlayer->m_vecVelocity().Length();
		if (speed <= 70.0f)
			return;

		auto collidable = g_LocalPlayer->GetCollideable();

		Vector min, max;
		min = collidable->OBBMins();
		max = collidable->OBBMaxs();

		min += g_LocalPlayer->m_vecOrigin();
		max += g_LocalPlayer->m_vecOrigin();

		Vector center = (min + max) * 0.5f;

		for (int i = 1; i <= g_GlobalVars->maxClients; ++i) {
			auto player = C_BasePlayer::GetPlayerByIndex(i);
			if (!player || !player->IsAlive() || player->IsDormant())
				continue;
			if (player == g_LocalPlayer || g_LocalPlayer->m_iTeamNum() == player->m_iTeamNum())
				continue;

			auto weapon = player->m_hActiveWeapon().Get();
			if (!weapon || weapon->m_iClip1() <= 0)
				continue;

			auto weapon_data = weapon->GetCSWeaponData();
			if (!weapon_data || weapon_data->iWeaponType <= WEAPONTYPE_KNIFE || weapon_data->iWeaponType >= WEAPONTYPE_C4)
				continue;

			auto eye_pos = player->GetEyePos();

			Vector direction;
			Math::AngleVectors(player->m_angEyeAngles(), direction);
			direction.NormalizeInPlace();

			Vector hit_point;
			bool hit = Math::IntersectionBoundingBox(eye_pos, direction, min, max, &hit_point);
			if (hit && eye_pos.DistTo(hit_point) <= weapon_data->flRange) {
				Ray_t ray;
				trace_t tr;
				CTraceFilterSkipEntity filter((C_BasePlayer*)player);
				ray.Init(eye_pos, hit_point);

				g_EngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);
				if (tr.contents & CONTENTS_WINDOW) { // skip windows
																							// at this moment, we dont care about local player
					filter.pSkip = tr.hit_entity;
					ray.Init(tr.endpos, hit_point);
					g_EngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);
				}

				if (tr.fraction == 1.0f || tr.hit_entity == g_LocalPlayer) {
					m_bIsPeeking = true;
					break;
				}
			}
		}
	};

	auto speed = g_LocalPlayer->m_vecVelocity().Length();
	bool standing = speed <= 1.0f;
	if (!g_Options.fakelag_standing && standing) {
		return;
	}

	if (!g_Options.fakelag_moving && !standing) {
		return;
	}

	enum FakelagMode {
		FakelagStatic = 0,
		FakelagSwitch,
		FakelagAdaptive,
		FakelagRandom,
		FakelagLegitPeek
	};

	float UnitsPerTick = 0.0f;

	int WishTicks = 0;
	int AdaptiveTicks = 2;
	static int LastRandomNumber = 5;
	static int randomSeed = 12345;

	switch (g_Options.fakelag_mode)
	{
	case FakelagSwitch:
		// apply same logic as static fakelag
		if (cmd->command_number % 30 > 15)
			break;
	case FakelagStatic:
		send_packet = !(g_ClientState->chokedcommands < choke_factor);
		break;
	case FakelagAdaptive:
		if (standing) {
			send_packet = !(g_ClientState->chokedcommands < choke_factor);
			break;
		}

		UnitsPerTick = g_LocalPlayer->m_vecVelocity().Length() * g_GlobalVars->interval_per_tick;
		while ((WishTicks * UnitsPerTick) <= 68.0f)
		{
			if (((AdaptiveTicks - 1) * UnitsPerTick) > 68.0f)
			{
				++WishTicks;
				break;
			}
			if ((AdaptiveTicks * UnitsPerTick) > 68.0f)
			{
				WishTicks += 2;
				break;
			}
			if (((AdaptiveTicks + 1) * UnitsPerTick) > 68.0f)
			{
				WishTicks += 3;
				break;
			}
			if (((AdaptiveTicks + 2) * UnitsPerTick) > 68.0f)
			{
				WishTicks += 4;
				break;
			}
			AdaptiveTicks += 5;
			WishTicks += 5;
			if (AdaptiveTicks > 16)
				break;
		}

		send_packet = !(g_ClientState->chokedcommands < WishTicks);
		break;
	case FakelagRandom:
		if (g_ClientState->chokedcommands < LastRandomNumber) {
			send_packet = false;
		}
		else {
			randomSeed = 0x41C64E6D * randomSeed + 12345;
			LastRandomNumber = (randomSeed / 0x10000 & 0x7FFFu) % choke_factor;
			send_packet = true;
		}
		break;
	case FakelagLegitPeek:
		LegitPeek(cmd, send_packet);
		break;
	}

	if (choke_factor < g_ClientState->chokedcommands)
		send_packet = true;

};

#include "../features/prediction.h"

void fakeduck(CUserCmd* pCmd, bool& bSendPacket) {
	int fakelag_limit = 14;
	int choked_goal = fakelag_limit / 2;



	if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
	{
		bSendPacket = g_ClientState->m_NetChannel->m_nChokedPackets >= fakelag_limit;

		if (g_ClientState->chokedcommands <= 7)
			pCmd->buttons &= ~IN_DUCK;
		else
			pCmd->buttons |= IN_DUCK;
	}
}

void nosmoke() {
	static bool set = true;

	static std::vector<const char*> vistasmoke_wireframe =
	{
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!g_Options.nosmoke)
	{
		if (set)
		{
			for (auto material_name : vistasmoke_wireframe)
			{
				IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;
	for (auto material_name : vistasmoke_wireframe)
	{
		IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	}
	//static auto shit = *(DWORD*)(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0") + 0x8);
	//if(shit)
	//	if (g_Options.nosmoke)
	//		*(int*)(shit) = 0;


}

void CorrectMouse(CUserCmd* cmd) {
	static ConVar* m_yaw = m_yaw = g_CVar->FindVar("m_yaw");
	static ConVar* m_pitch = m_pitch = g_CVar->FindVar("m_pitch");
	static ConVar* sensitivity = sensitivity = g_CVar->FindVar("sensitivity");

	static QAngle m_angOldViewangles = g_ClientState->viewangles;

	float delta_x = std::remainderf(cmd->viewangles.pitch - m_angOldViewangles.pitch, 360.0f);
	float delta_y = std::remainderf(cmd->viewangles.yaw - m_angOldViewangles.yaw, 360.0f);

	if (delta_x != 0.0f) {
		float mouse_y = -((delta_x / m_pitch->GetFloat()) / sensitivity->GetFloat());
		short mousedy;
		if (mouse_y <= 32767.0f) {
			if (mouse_y >= -32768.0f) {
				if (mouse_y >= 1.0f || mouse_y < 0.0f) {
					if (mouse_y <= -1.0f || mouse_y > 0.0f)
						mousedy = static_cast<short>(mouse_y);
					else
						mousedy = -1;
				}
				else {
					mousedy = 1;
				}
			}
			else {
				mousedy = 0x8000u;
			}
		}
		else {
			mousedy = 0x7FFF;
		}

		cmd->mousedy = mousedy;
	}

	if (delta_y != 0.0f) {
		float mouse_x = -((delta_y / m_yaw->GetFloat()) / sensitivity->GetFloat());
		short mousedx;
		if (mouse_x <= 32767.0f) {
			if (mouse_x >= -32768.0f) {
				if (mouse_x >= 1.0f || mouse_x < 0.0f) {
					if (mouse_x <= -1.0f || mouse_x > 0.0f)
						mousedx = static_cast<short>(mouse_x);
					else
						mousedx = -1;
				}
				else {
					mousedx = 1;
				}
			}
			else {
				mousedx = 0x8000u;
			}
		}
		else {
			mousedx = 0x7FFF;
		}

		cmd->mousedx = mousedx;
	}
}
#include "../features/animfixed shit.h"

void DoSlowWalk(CUserCmd* cmd, C_BaseCombatWeapon* Weapon)
{
	if (!InputSys::Get().IsKeyDown(g_Options.misc_minwalk_bind))
		return;

	float amount = 0.0034f * g_Options.misc_minwalk_amount; // options.misc.slow_walk_amount has 100 max value

	Vector velocity = g_LocalPlayer->m_vecVelocity();
	QAngle direction;

	Math::VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.yaw = cmd->viewangles.yaw - direction.yaw;

	Vector forward;

	Math::AngleVectors(direction, forward);

	Vector source = forward * -speed;

	if (speed >= (*(float*)((uintptr_t)Weapon->GetCSWeaponData() + 0x0130/*maxspeed*/) * amount))
	{
		cmd->forwardmove = source.x;
		cmd->sidemove = source.y;
	}
}

#include "../features/nightmode.hpp"
#include "../features/ragebot/Anti-Aim.hpp"
bool __stdcall Hooks::hkCreateMove(float input_sample_frametime, CUserCmd* cmd)
{
	static auto original = clientmode_hook.get_original<decltype(&hkCreateMove)>(index::CreateMove);
	bool* send_packet = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	original(input_sample_frametime, cmd);

	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);

	if (!cmd || !cmd->command_number || g_Unload)	return original;
	if (!g_LocalPlayer)								return original;

	Globals::cmd = cmd;

	if (original)
		g_Prediction->SetLocalViewangles(Vector(cmd->viewangles.pitch, cmd->viewangles.yaw, cmd->viewangles.roll));

	if (g_Options.misc_bhop)
		BunnyHop::OnCreateMove(cmd);

	if (g_Options.misc_human_bhop)
		BunnyHop::HumanBhop(cmd);

	if (g_Options.misc_autostrafer)
		BunnyHop::AutoStrafe(cmd, cmd->viewangles);

	bool bSendPacket = *SendPacket;
	C_BaseCombatWeapon* weapon = g_LocalPlayer->m_hActiveWeapon();
	sendpacket = bSendPacket;

	static float SpawnTime = 0.0f;
	if (g_LocalPlayer->m_flSpawnTime() != SpawnTime) {
		g_AnimState.pBaseEntity = g_LocalPlayer;
		g_LocalPlayer->ResetAnimationState(&g_AnimState);
		SpawnTime = g_LocalPlayer->m_flSpawnTime();
	}

	Clnatag();
	auto pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));

	if (g_Options.misc_autodefuse)
		Misc::Get().AutoDefuse(pLocal, cmd);

	if (g_Options.solbicak)
		Misc::Get().SolEl();

	Misc::Get().MoonWalk(cmd);

	if (g_LocalPlayer && g_LocalPlayer->IsAlive() && (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2))
		saver.LastShotEyePos = g_LocalPlayer->GetEyePos();

	if (g_Options.misc_disableblur)
		Misc::Get().DisablePanoramaBlur();

	if (g_Options.misc_chatspam)
		Misc::Get().SohbetSpam();

	if (g_EngineClient->IsConnected() && g_EngineClient->IsInGame())
		NightMode::Get().run();

	Misc::Get().RemoveCrouchCooldown(cmd);

	auto unpred_flags = g_LocalPlayer->m_fFlags();
	auto oldangles = cmd->viewangles;

	g_Options.dista = cmd->random_seed;


	EnginePred::BeginPred(cmd);
	{
		AutoBlock(cmd);


		if (cmd->command_number % 2 == 1 && bSendPacket && !(g_Options.fakeduck && InputSys::Get().IsKeyDown(g_Options.fakeduck_bind)) && g_Options.misc_desync)
			bSendPacket = false;

		if (g_Options.edgejump && GetAsyncKeyState(g_Options.edgejump_bind))
		{
			if ((unpred_flags & FL_ONGROUND) && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
				cmd->buttons |= IN_JUMP;
		}
		QAngle angleold = cmd->viewangles;

		g_Aimbot.OnMove(cmd);

		g_Backtrack.OnMove(cmd);

		if (g_Options.enable_trigger)
			Triggerbot::Get().OnCreateMove(cmd);

		if (g_Options.rage_enabled)
			g_Ragebot.DoRagebot(cmd, weapon);

		static bool sidePress = false;
		if (InputSys::Get().IsKeyDown(g_Options.tankAntiaimKey) && !sidePress)
		{
			aaSide = !aaSide;
			sidePress = true;
		}
		else if (!InputSys::Get().IsKeyDown(g_Options.tankAntiaimKey) && sidePress)
		{
			sidePress = false;
		}

		if (g_LocalPlayer->IsAlive() && g_EngineClient->IsInGame() && g_Options.AntiAim_enable)
			AntiAims::OnCreateMove(cmd);

		if (g_Options.rage_resolver)
			g_Resolver.OnCreateMove();

		if (g_Options.kbot)
			knifebot(cmd);

		if (g_Options.misc_minwalk)
			DoSlowWalk(cmd, weapon);

		if (g_Options.misc_desync && g_ClientState->chokedcommands >= max_choke_ticks()) {
			bSendPacket = true;
		}

		if (g_Options.misc_desync && std::fabsf(g_LocalPlayer->m_flSpawnTime() - g_GlobalVars->curtime) > 1.0f)
			Desync(cmd, bSendPacket);

		CorrectMouse(cmd);

		auto anim_state = g_LocalPlayer->GetPlayerAnimState();
		if (anim_state) {
			CCSGOPlayerAnimState anim_state_backup = *anim_state;
			*anim_state = g_AnimState;
			g_LocalPlayer->GetVAngles() = cmd->viewangles;
			g_LocalPlayer->UpdateClientSideAnimation();

			updatelby(anim_state);

			g_AnimState = *anim_state;
			*anim_state = anim_state_backup;
		}
		if (bSendPacket) {
			anglereal = g_AnimState.m_flGoalFeetYaw;
			if (anim_state)
				anglefake = anim_state->m_flGoalFeetYaw;
			vangle = cmd->viewangles;
		}


		fix_movement(cmd, angleold);
	}
	EnginePred::EndPred();

	g_Ragebot.UpdateFakeAnimations();

	if (g_Options.fakeduck && InputSys::Get().IsKeyDown(g_Options.fakeduck_bind))
		fakeduck(cmd, bSendPacket);

	if (g_Options.fakeduck && InputSys::Get().IsKeyDown(g_Options.fakeduck_bind))
		cmd->buttons |= IN_BULLRUSH;

	if (!(g_Options.fakeduck && InputSys::Get().IsKeyDown(g_Options.fakeduck_bind)))
		Fakelag(cmd, bSendPacket);

	nosmoke();

	if (g_LocalPlayer && InputSys::Get().IsKeyDown(VK_TAB) && g_Options.misc_rankreveal)
		Utils::RankRevealAll();


	Math::FixAngles(cmd->viewangles);
	std::clamp(cmd->sidemove, -450.f, 450.f);
	std::clamp(cmd->forwardmove, -450.f, 450.f);
	std::clamp(cmd->upmove, -320.f, 320.f);
	Globals::bSendPacket = bSendPacket;
	*SendPacket = bSendPacket;

	return false;
}

std::string bec_sky_name = "";

void __fastcall Hooks::hkOverrideView(void* _this, int edx, CViewSetup* vsView)
{
	static auto override_view_o = clientmode_hook.get_original<decltype(&hkOverrideView)>(index::OverrideView);

	if (g_EngineClient->IsInGame() && g_LocalPlayer && vsView) {

		auto r_modelAmbientMin = g_CVar->FindVar("r_modelAmbientMin");
		auto mat_force_tonemap_scale = g_CVar->FindVar("mat_force_tonemap_scale");
		auto mat_ambient_light_r = g_CVar->FindVar("mat_ambient_light_r");
		auto mat_ambient_light_g = g_CVar->FindVar("mat_ambient_light_g");
		auto mat_ambient_light_b = g_CVar->FindVar("mat_ambient_light_b");
		static auto mat_postprocess_enable = g_CVar->FindVar("mat_postprocess_enable");
		g_CVar->FindVar("weapon_debug_spread_show")->SetValue(g_Options.crosshair && !g_LocalPlayer->m_bIsScoped() ? 3 : 0);

		if (!g_Unload) {
			if (g_Options.other_mat_ambient_light_rainbow) {
				static float rainbow; rainbow += 0.005f; if (rainbow > 1.f) rainbow = 0.f;
				auto rainbow_col = Color::FromHSB(rainbow, 1.0f, 1.0f);
				mat_ambient_light_r->SetValue(rainbow_col.r() / 255.0f);
				mat_ambient_light_g->SetValue(rainbow_col.g() / 255.0f);
				mat_ambient_light_b->SetValue(rainbow_col.b() / 255.0f);
			}
			else
			{
				if (mat_ambient_light_r->GetFloat() != 0.f)			mat_ambient_light_r->SetValue(0.f);
				if (mat_ambient_light_g->GetFloat() != 0.f)			mat_ambient_light_g->SetValue(0.f);
				if (mat_ambient_light_b->GetFloat() != 0.f)			mat_ambient_light_b->SetValue(0.f);
			}
		}
		else {
			if (r_modelAmbientMin->GetFloat() != 0.0f)
				r_modelAmbientMin->SetValue(0.0f);

			if (mat_force_tonemap_scale->GetFloat() != 1.0f)
				mat_force_tonemap_scale->SetValue(1.0f);

			if (mat_postprocess_enable->GetInt() != 0)
				mat_postprocess_enable->SetValue(0);

			if (mat_ambient_light_r->GetFloat() != 0.0f)	mat_ambient_light_r->SetValue(0.0f);
			if (mat_ambient_light_g->GetFloat() != 0.0f)	mat_ambient_light_g->SetValue(0.0f);
			if (mat_ambient_light_b->GetFloat() != 0.0f)	mat_ambient_light_b->SetValue(0.0f);
		}

		g_CVar->FindVar("zoom_sensitivity_ratio_mouse")->SetValue(float(!g_Options.nozoom));

		if (!Globals::got_w2s_matrix)
		{
			Globals::got_w2s_matrix = true;
			Globals::w2s_offset = (reinterpret_cast<DWORD>(&g_EngineClient->WorldToScreenMatrix()) + 0x40);
		}

		Visuals::Get().ThirdPerson();
		if (!g_LocalPlayer->m_bIsScoped())
			vsView->fov = g_Options.misc_fov;
		else if (g_LocalPlayer->m_bIsScoped() && g_Options.nozoom && g_LocalPlayer->m_hActiveWeapon() &&
			g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() != WEAPON_SG556 &&
			g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() != WEAPON_AUG)
			vsView->fov = g_Options.misc_fov;

		if (g_Options.fakeduck && InputSys::Get().IsKeyDown(g_Options.fakeduck_bind) && g_LocalPlayer->IsAlive() && g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			vsView->origin.z = g_LocalPlayer->m_vecOrigin().z + 64;

		if (g_Options.esp_grenade_prediction && g_LocalPlayer->m_hActiveWeapon())
			GrenadePrediction::Get().View(vsView);
	}
	override_view_o(g_ClientMode, edx, vsView);
}
#include "../features/glow.hpp"

int __fastcall Hooks::hkDoPostScreenEffects(void* _this, int edx, int a1)
{
	static auto do_pose_effects_o = clientmode_hook.get_original<decltype(&hkDoPostScreenEffects)>(index::DoPostScreenSpaceEffects);

	if (g_LocalPlayer && g_Options.glow_enabled)
		Glow::Get().Run();

	return do_pose_effects_o(g_ClientMode, edx, a1);
}