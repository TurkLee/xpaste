#include "hooks.hpp"
#include <intrin.h>  
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/glow.hpp"
#include "features/visuals.hpp"
#pragma comment(lib,"ws2_32")
#include "features/Misc.h"

#include "Korumalar/xorstr.h"
#include <locale>
#include <chrono>

#include "features/damageindicator.h"
#include "features/ragebot/Resolver.hpp"

#include "hooks/DirectHooks.h"
#include "hooks/c_cs_player_.h"
#include "hooks/ClientMode.h"
#include "notification/Globals.hpp"
#include "Logger/event_listener.hpp"
#include "Logger/event_logger.hpp"
#include "features/skin/skins.h"

#include "helpers/MinHook.h"
#include "Korumalar/AtakanınAnnesi.h"
#include "HitPossitionHelper.h"
#include "runtime_saver.h"

void silent_crash()
{
	__asm
	{
		rdtsc
		XOR edx, eax
		add eax, edx
		mov esp, eax
		XOR ebp, edx
		mov ebx, ebp
		mov ecx, esp
		XOR esi, ebx
		XOR edi, esp
		jmp eax
	}
}
#pragma intrinsic(_ReturnAddress)  

int32_t nSinceUse = 0;
bool bInSendMove = false, bFirstSendMovePack = false;

float side = 1.0f;
float next_lby = 0.0f;
namespace Hooks {

#define hookdet(base, indexh, detour, original) MH_CreateHook((LPVOID)(*(int**)base)[indexh], detour, &original)
#define def_orig(name) void* name = nullptr
	def_orig(o_send_net_msg);
	def_orig(o_fire_event);
	def_orig(o_frame_stage_notify);
	def_orig(o_paint_traverse);
	typedef void(__fastcall* fn_fix)(void*, void*);
	fn_fix fn_f1;
	fn_fix fn_f2;

	bool __fastcall hk_is_hltv()
	{
		const auto org_f = engine_hook.get_original< is_hltv_t >(93);

		if (!g_EngineClient->IsInGame())
			return org_f();

		static auto ptr_accumulate_layers = Utils::PatternScan(GetModuleHandleA("client.dll"), ("84 C0 75 0D F6 87"));
		static auto setupvelocity_call = reinterpret_cast<void*> (Utils::PatternScan(GetModuleHandleA("client.dll"),
			("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0")));

		if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (ptr_accumulate_layers) && c_animation_system::Get().enable_bones)
			return true;

		if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (setupvelocity_call))
			return true;

		return org_f();
	}
	//--------------------------------------------------------------------------------
	MDLHandle_t __fastcall HK_FindMDL(void* ecx, void* edx, char* FilePath)
	{
		auto oFindMdl = findmdl_hook.get_original<iFindMdl>(10);

		if (strstr(FilePath, "v_rif_ak47.mdl"))
		{
			sprintf(FilePath, "materials/weapons/");
		}

		return oFindMdl(ecx, FilePath);
	}
	//--------------------------------------------------------------------------------
	const char* __fastcall HK_GetModelName(void* pEcx, void* pEdx, void* pModel)
	{
		uintptr_t* uipStackPointer;

		static auto pGetModelName = findmdl_hook.get_original<o_MdlName>(18);

		static auto uiGetModelNameReturnAddress = (uintptr_t)Utils::PatternScan(GetModuleHandleA("client.dll"), "EB 23 8B 7D FC 8B CF 53 FF 75 08");

		__asm
		{
			mov uipStackPointer, ebp;
		}

		if (uipStackPointer[1] == uiGetModelNameReturnAddress)
		{
			uipStackPointer[1] += 2;

			return 0;
		}

		return pGetModelName(pEcx, pModel);
	}

	const char* GetChamsMaterial(int type)
	{
		switch (type)
		{
		case 0:
			return "debug/debugambientcube";
		case 1:
			return "models/inventory_items/trophy_majors/gold";
		case 2:
			return "models/inventory_items/cologne_prediction/cologne_prediction_glass";
		case 3:
			return "models/inventory_items/trophy_majors/crystal_clear";
		case 4:
			return "models/gibs/glass/glass";
		case 5:
			return "models/inventory_items/trophy_majors/gloss";
		case 6:
			return "models/inventory_items/trophy_majors/velvet";
		case 7:
			return "models/inventory_items/dogtags/dogtags_outline";
		case 8:
			return "models/inventory_items/hydra_crystal/hydra_crystal_detail";
		}
	}


	void __fastcall renderview::scene_end::hooked(IVRenderView*& view)
	{
		static const auto original = sceneendhook.get_original<fn>(index);

		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
			return;

		IMaterial* local_mat = nullptr;
		constexpr float color_gray[4] = { 166, 167, 169, 255 };

		if (GetChamsMaterial(g_Options.esp_localplayer_chams_type) == "debug/debugambientcube")
		{
			local_mat = g_MatSystem->FindMaterial(GetChamsMaterial(g_Options.esp_localplayer_chams_type), TEXTURE_GROUP_OTHER);
		}
		else
		{
			local_mat = g_MatSystem->FindMaterial(GetChamsMaterial(g_Options.esp_localplayer_chams_type), TEXTURE_GROUP_MODEL);
		}
		IMaterial* localfake_mat = g_MatSystem->FindMaterial(GetChamsMaterial(g_Options.esp_localplayer_fakechams_type), TEXTURE_GROUP_MODEL);

		if (g_LocalPlayer && g_LocalPlayer->IsAlive() && !g_LocalPlayer->IsDormant())
		{
			if (g_Options.esp_localplayer_chams)
			{
				if (g_Options.esp_localplayer_chams_xyz)
				{
					if (g_Options.fake_chams)
					{

						QAngle OrigAng;
						OrigAng = g_LocalPlayer->m_angEyeAngles();

						g_LocalPlayer->SetFakeAngle(QAngle(0, g_LocalPlayer->m_angEyeAngles().yaw, 0));
						g_RenderView->SetColorModulation(g_Options.esp_localplayer_fakechams_color_invisible);
						localfake_mat->IncrementReferenceCount();
						localfake_mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g_MdlRender->ForcedMaterialOverride(localfake_mat);
						g_LocalPlayer->DrawModel(1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);

						g_RenderView->SetColorModulation(g_Options.esp_localplayer_fakechams_color_visible);
						localfake_mat->IncrementReferenceCount();
						localfake_mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						g_MdlRender->ForcedMaterialOverride(localfake_mat);
						g_LocalPlayer->DrawModel(1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);
					}
					else
					{

						g_RenderView->SetColorModulation(g_LocalPlayer->m_bGunGameImmunity() ? color_gray : g_Options.esp_localplayer_chams_color_invisible);
						local_mat->IncrementReferenceCount();
						local_mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g_MdlRender->ForcedMaterialOverride(local_mat);
						g_LocalPlayer->DrawModel(0x1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);


						g_RenderView->SetColorModulation(g_LocalPlayer->m_bGunGameImmunity() ? color_gray : g_Options.esp_localplayer_chams_color_visible);
						local_mat->IncrementReferenceCount();
						local_mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						g_MdlRender->ForcedMaterialOverride(local_mat);
						g_LocalPlayer->DrawModel(0x1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);
					}
				}
				else
				{
					if (g_Options.fake_chams)
					{
						QAngle OrigAng;
						OrigAng = g_LocalPlayer->m_angEyeAngles();
						g_LocalPlayer->SetFakeAngle(QAngle(0, g_LocalPlayer->m_angEyeAngles().yaw, 0));
						g_RenderView->SetColorModulation(g_Options.esp_localplayer_fakechams_color_visible);
						g_MdlRender->ForcedMaterialOverride(localfake_mat);
						g_LocalPlayer->DrawModel(1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);

					}
					else
					{

						g_RenderView->SetColorModulation(g_Options.esp_localplayer_chams_color_visible);
						g_MdlRender->ForcedMaterialOverride(local_mat);
						g_LocalPlayer->DrawModel(0x1, 255);
						g_MdlRender->ForcedMaterialOverride(nullptr);
					}
				}
			}
		}
		original(view);

	}

	bool __stdcall is_playing_demo::hook()
	{
		if (g_Options.misc_reveal_money
			&& *static_cast<uintptr_t*>(_ReturnAddress()) == 0x0975C084  // client.dll : 84 C0 75 09 38 05
			&& **reinterpret_cast<uintptr_t**>(uintptr_t(_AddressOfReturnAddress()) + 4) == 0x0C75C084) { // client.dll : 84 C0 75 0C 5B
			return true;
		}
		auto oisPlayingDemo = engine_vhook.get_original<fn>(index::is_playing_demo);
		return false;
	}
	void __fastcall frame_stage_notify_hook(void* thisptr, int edx, ClientFrameStage_t stage)
	{
		if (stage == FRAME_START)
		{

		}

		if (stage == FRAME_RENDER_START)
		{

		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{

		}
		Visuals::Get().PlayerChanger(stage);

		if (g_Options.rage_resolver)
			g_Resolver.FrameStage(stage);

		reinterpret_cast<frame_stage_notify_t>(o_frame_stage_notify)(thisptr, edx, stage);
	}
	bool __fastcall fire_event_hook(void* thisptr, void* edx, IGameEvent* event)
	{
		Visuals::Get().FireGameEvent(event);
		if (strstr(event->GetName(), "player_hurt"))
		{
			auto userid = event->GetInt("userid"),
				attacker = event->GetInt("attacker");
			if (userid || attacker)
			{
				auto
					userid_id = g_EngineClient->GetPlayerForUserID(userid),
					attacker_id = g_EngineClient->GetPlayerForUserID(attacker);
				player_info_t userid_info, attacker_info;
				if (g_EngineClient->GetPlayerInfo(userid_id, &userid_info) || g_EngineClient->GetPlayerInfo(attacker_id, &attacker_info))
				{
					std::stringstream ss;
					if (attacker_id == g_EngineClient->GetLocalPlayer())
					{

						if (g_Options.hiteffect)
						{
							g_LocalPlayer->m_flHealthShotBoostExpirationTime() = g_GlobalVars->curtime + 1.f;
						}
					}
				}
			}
		}

		if (event)
		{
			if (strstr(event->GetName(), "player_death"))
			{
				if (g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer())
				{
					auto& weapon = g_LocalPlayer->m_hActiveWeapon();
					if (weapon)
					{
						auto& skin_data = g_Options.m_items[weapon->m_Item().m_iItemDefinitionIndex()];
						auto& stat_track = g_Options.statrack_items[weapon->m_Item().m_iItemDefinitionIndex()];
						if (skin_data.enabled && skin_data.stat_trak)
						{

							stat_track.statrack_new.counter++;
							weapon->m_nFallbackStatTrak() = stat_track.statrack_new.counter;
							weapon->GetClientNetworkable()->PostDataUpdate(0);
							weapon->GetClientNetworkable()->OnDataChanged(0);
						}
					}

					const auto icon_override = skins::get_icon_override(event->GetString("weapon"));

					if (icon_override)
					{
						event->GetString("weapon", icon_override);
					}
				}
			}

			Visuals::Get().events(event);

		}

		return reinterpret_cast<fire_event_t>(o_fire_event)(thisptr, edx, event);
	}
	//--------------------------------------------------------------------------------
	int __fastcall hkCanLoadThirdPartyFiles(void* _this)
	{
		/*static auto return_to_client_panorama = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7");
		static auto return_to_engine = Utils::PatternScan(GetModuleHandleW(L"engine.dll"), "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04");

		if (_ReturnAddress() == return_to_client_panorama || _ReturnAddress() == return_to_engine)
			return 0;*/

		return 1;
	}
	//--------------------------------------------------------------------------------
	int __stdcall hkGetUnverifiedFileHashes(void* _this, void* someclass, int nMaxFiles)
	{
		return 0;
	}
	//--------------------------------------------------------------------------------
	void EventListeners() {
		damage_indicators.Listener();
		Visuals::Get().Listener();
	}
	//--------------------------------------------------------------------------------


	void Initialize()
	{
		if (MH_Initialize() != MH_OK)
			throw std::runtime_error(("failed initialize minhook"));

		EventListeners();

		direct_hook.setup(g_D3DDevice9);
		direct_hook.hook_index(index::Reset, hkReset);
		direct_hook.hook_index(index::EndScene, hkEndScene);

		clientstate_hook.setup((void*)((uintptr_t)g_ClientState + 0x8));
		clientstate_hook.hook_index(64, hkReadPacketEntities);

		sceneendhook.setup(g_RenderView);
		sceneendhook.hook_index(renderview::scene_end::index, renderview::scene_end::hooked);

		hookdet(g_CHLClient, index::FrameStageNotify, frame_stage_notify_hook, o_frame_stage_notify);

		engine_vhook.setup(g_EngineClient);
		engine_vhook.hook_index(index::is_playing_demo, is_playing_demo::hook);

		hookdet(g_GameEvents, index::FireEvent, fire_event_hook, o_fire_event);

		partition_hook.setup(g_SpatialPartition);
		partition_hook.hook_index(16, hkSuppressLists);

		g_CustomEventsManager.Init();
		VisualsRender::Get().CreateFonts();
		clientmode_hook.setup(g_ClientMode);
		clientmode_hook.hook_index(index::CreateMove, hkCreateMove);
		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);


		mdlrender_hook.setup(g_MdlRender);
		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);

		/*vguipanel_hook.setup(g_VGuiPanel);
		vguipanel_hook.hook_index(index::PaintTraverse, paint_traverse_hook);*/
		hookdet(g_VGuiPanel, index::PaintTraverse, paint_traverse_hook, o_paint_traverse);

		bsp_query_hook.setup(g_EngineClient->GetBSPTreeQuery());
		bsp_query_hook.hook_index(index::ListLeavesInBox, hkListLeavesInBox);

		filesystem_hook.setup(g_FileSystem);
		filesystem_hook.hook_index(128, hkCanLoadThirdPartyFiles);
		filesystem_hook.hook_index(101, hkGetUnverifiedFileHashes);

		sequence_hook = new recv_prop_hook(C_BaseViewModel::m_nSequence(), hkRecvProxy);
		listener = new CGameEventListener();
		g_GameEvents->AddListener(listener, ("game_newmap"), false);
		g_GameEvents->AddListener(listener, ("switch_team"), false);
		g_GameEvents->AddListener(listener, ("player_hurt"), false);
		g_GameEvents->AddListener(listener, ("bullet_impact"), false);
		g_GameEvents->AddListener(listener, ("item_purchase"), false);
		g_GameEvents->AddListener(listener, ("player_spawned"), false);
		g_GameEvents->AddListener(listener, ("cs_pre_restart"), false);
		g_GameEvents->AddListener(listener, ("round_freeze_end"), false);
		g_GameEvents->AddListener(listener, ("announce_phase_end"), false);
		g_GameEvents->AddListener(listener, ("round_start"), false);
		g_GameEvents->AddListener(listener, ("player_footstep"), false);
		g_GameEvents->AddListener(listener, ("player_death"), false);
		g_GameEvents->AddListener(listener, ("begin_new_match"), false);
		g_GameEvents->AddListener(listener, ("client_disconnect"), false);
		g_GameEvents->AddListener(listener, ("weapon_fire"), false);
		g_GameEvents->AddListener(listener, ("inferno_startburn"), false);
		g_GameEvents->AddListener(listener, ("inferno_expire"), false);
		g_GameEvents->AddListener(listener, ("bomb_beginplant"), false);
		g_GameEvents->AddListener(listener, ("bomb_planted"), false);
		g_GameEvents->AddListener(listener, ("bomb_begindefuse"), false);
		MH_EnableHook(MH_ALL_HOOKS);

	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		Glow::Get().Shutdown();
		g_CustomEventsManager.Unload();
		sequence_hook->~recv_prop_hook();
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
	//--------------------------------------------------------------------------------


#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

	int __fastcall hkListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax) {
		typedef int(__thiscall* ListLeavesInBox)(void*, const Vector&, const Vector&, unsigned short*, int);
		static auto ofunc = bsp_query_hook.get_original< ListLeavesInBox >(index::ListLeavesInBox);

		if (*(uint32_t*)_ReturnAddress() != 0x14244489)
			return ofunc(bsp, mins, maxs, pList, listMax);

		auto info = *(RenderableInfo_t**)((uintptr_t)_AddressOfReturnAddress() + 0x14);
		if (!info || !info->m_pRenderable)
			return ofunc(bsp, mins, maxs, pList, listMax);

		auto base_entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
		if (!base_entity || !base_entity->IsPlayer())
			return ofunc(bsp, mins, maxs, pList, listMax);

		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
		auto count = ofunc(bsp, mins, maxs, pList, listMax);
		return count;

	}
	void DrawBeamPaw(Vector src, Vector end, Color color)
	{
		BeamInfo_t beamInfo;
		beamInfo.m_nType = 0; //TE_BEAMPOINTS
		beamInfo.m_vecStart = src;
		beamInfo.m_vecEnd = end;
		beamInfo.m_pszModelName = "sprites/glow01.vmt";
		beamInfo.m_pszHaloName = "sprites/glow01.vmt";
		beamInfo.m_flHaloScale = 3.0;
		beamInfo.m_flWidth = 4.5f;
		beamInfo.m_flEndWidth = 4.5f;
		beamInfo.m_flFadeLength = 0.5f;
		beamInfo.m_flAmplitude = 0;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.0f;
		beamInfo.m_nStartFrame = 0.0;
		beamInfo.m_flFrameRate = 0.0;
		beamInfo.m_flRed = color.r();
		beamInfo.m_flGreen = color.g();
		beamInfo.m_flBlue = color.b();
		beamInfo.m_nSegments = 1;
		beamInfo.m_bRenderable = true;
		beamInfo.m_flLife = 1;
		beamInfo.m_nFlags = 256 | 512 | 32768; //FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM
		Beam_t* myBeam = g_Beams->CreateBeamPoints(beamInfo);
		if (myBeam)
			g_Beams->DrawBeam(myBeam);
	}
	void AddTrails()
	{
		if (g_LocalPlayer && g_LocalPlayer->IsAlive() && g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
		{
			static float rainbow;
			static Vector last_pos = Vector(0, 0, 0);

			rainbow += 0.0005;
			if (rainbow > 1.f)
				rainbow = 0.f;

			auto rainbow_col = Color::FromHSB(rainbow, 1, 1);
			auto local_pos = g_LocalPlayer->m_vecOrigin();
			DrawBeamPaw(last_pos, Vector(local_pos.x + g_Options.foottrails_xshift,
				local_pos.y + g_Options.foottrails_yshift, local_pos.z + g_Options.foottrails_zshift), rainbow_col);
			last_pos = local_pos;
		}
	}
	void __fastcall paint_traverse_hook(PVOID p_panel, int edx, unsigned int vgui_panel, bool force_repaint, bool allow_force) {
		static unsigned int panel_zoom_id = 0;


		if (!strcmp("HudZoom", g_VGuiPanel->GetName(vgui_panel)) && g_Options.nozoom)
			return;

		if (!panel_zoom_id) {
			const auto panelName = g_VGuiPanel->GetName(vgui_panel);
			if (!strcmp(panelName, "MatSystemTopPanel")) {
				panel_zoom_id = vgui_panel;
			}
		}
		else if (panel_zoom_id == vgui_panel)
		{

			if (g_Options.foottrils)
				AddTrails();

			Visuals::Get().QuakeKillCounter();

			if (g_Options.aspect_ratio_enable)
			{
				ConVar* cheat = g_CVar->FindVar("r_aspectratio");
				cheat->SetValue(g_Options.aspect_ratio);
			}
			else
			{
				ConVar* cheat = g_CVar->FindVar("r_aspectratio");
				cheat->SetValue(0);
			}

			static bool bSkip{ false };
			if (!bSkip) {
				g_EngineClient->GetScreenSize(CheatGVars::SWidth, CheatGVars::SHeight);
				CheatGVars::SWidthHalf = CheatGVars::SWidth / 2;
				CheatGVars::SHeightHalf = CheatGVars::SHeight / 2;
				//g_Config->ApplyRainbow();
				Render::Get().BeginScene();
			}
		}

		reinterpret_cast<paint_traverse_t>(o_paint_traverse)(p_panel, vgui_panel, force_repaint, allow_force);
	}

	//--------------------------------------------------------------------------------
	void __fastcall hkReadPacketEntities(void* pClientState, void* pEDX, void* pEntInfo) {
		static auto original = clientstate_hook.get_original<ReadPacketEntities>(64);

		original(pClientState, pEntInfo);

		skins::on_frame_stage_notify(false);

	}
	//--------------------------------------------------------------------------------
	bool AddCCSPlayerListener(IGameEventListener2* listener, const char* name, const bool serverside)
	{
		static const auto return_to_c_csplayer_ctor = reinterpret_cast<void*>(
			reinterpret_cast<uint32_t>(Utils::PatternScan(GetModuleHandle("client.dll"), "FF 50 0C C7 87")) + 3);

		static auto _add_listener = gameevents_hook.get_original<add_listener_t>(3);

		if (_ReturnAddress() == return_to_c_csplayer_ctor)
		{
			const auto player = reinterpret_cast<C_BasePlayer*>(
				reinterpret_cast<uint32_t>(listener) - 0x3870);
		}

		return _add_listener(g_GameEvents, listener, name, serverside);
	}
	//--------------------------------------------------------------------------------
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		static auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");
		static auto ofunc = sv_cheats.get_original<bool(__thiscall*)(PVOID)>(13);
		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;

		return ofunc(pConVar);
	}
	//--------------------------------------------------------------------------------
	bool __fastcall hkgrenadepreviewGetBool(PVOID pConVar, void* edx)
	{
		static auto ofunc = sv_cheats.get_original<bool(__thiscall*)(PVOID)>(13);
		if (!ofunc)
			return false;

		if (g_Options.esp_grenade_prediction)
			return true;
		return ofunc(pConVar);
	}
	//--------------------------------------------------------------------------------
	void CGameEventListener::FireGameEvent(IGameEvent* event)
	{
		if (!strstr(event->GetName(), "bullet_impact"))
		{
			HitPossitionHelper hp;

			C_BasePlayer* shooter = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(event->GetInt("userid"))));

			if (!shooter || shooter != g_LocalPlayer)
				return;

			Vector p = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
			hp.ShotTracer(saver.LastShotEyePos, p);
		}

		if (!strstr(event->GetName(), "player_hurt"))
			HitPossitionHelper::Get().OnFireEvent(event);
	}
}