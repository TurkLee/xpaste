#pragma once

#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>
#include "hooks/MinHook.h"
inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }


namespace index
{
	constexpr auto EmitSound1 = 5;
	constexpr auto EmitSound2 = 6;
	constexpr auto FireEvent = 9;
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 24;
	constexpr auto PlaySound = 82;
	constexpr auto is_playing_demo = 82;
	constexpr auto DrawModelExecute = 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool = 13;
	constexpr auto OverrideView = 18;
	constexpr auto LockCursor = 67;
	constexpr auto FrameStageNotify = 37;
	constexpr auto ListLeavesInBox = 6;
	constexpr auto WriteUsercmd = 24;

}

extern int32_t nSinceUse;
extern bool bInSendMove, bFirstSendMovePack;
using SeventyHook = MinHook;
namespace Hooks
{
	class CGameEventListener final : public IGameEventListener2
	{
		void FireGameEvent(IGameEvent* p_event) override;
		int GetEventDebugID() override { return EVENT_DEBUG_ID_INIT; }
	};


    void Initialize();
    void Shutdown();
	inline vgui::HFont espfont;
    inline SeventyHook hlclient_hook;
	inline SeventyHook file_crc_svr;
	inline SeventyHook direct_hook;
	inline SeventyHook engine_hook;
	inline SeventyHook findmdl_hook;
	inline SeventyHook post_data_update;
	inline SeventyHook vguipanel_hook;
	inline SeventyHook gameevents_hook;
	inline recv_prop_hook* sequence_hook;
	inline SeventyHook vguisurf_hook;
	inline SeventyHook mdlrender_hook;
	inline SeventyHook clientstate_hook;
	inline SeventyHook partition_hook;
	inline SeventyHook viewrender_hook;
	inline SeventyHook sound_hook;
	inline SeventyHook clientmode_hook;
	inline SeventyHook sv_cheats;
	inline SeventyHook cl_grenadepreview;
	inline SeventyHook ccsplayer_vtablehook;
	inline SeventyHook gameevents_vhook;
	inline SeventyHook sceneendhook;
	inline SeventyHook engine_vhook;
	inline SeventyHook client_vhook;
	inline SeventyHook netchannel_vhook;
	inline SeventyHook filesystem_hook;
	inline SeventyHook bsp_query_hook;




	struct renderview
	{
		struct scene_end
		{
			static const int index = 9;

			using fn = void(__thiscall*)(IVRenderView*);
			static void __fastcall hooked(IVRenderView*&);
		};
	};

	namespace is_playing_demo
	{
		using fn = bool(__stdcall*)();
		bool __stdcall hook();
	}
	using WriteUsercmdDeltaToBufferFn = bool(__thiscall*)(IBaseClientDLL*, /*void* unk,*/ int, bf_write*, int, int, bool);
	typedef bool(__fastcall* send_net_msg_t)(void*, int, i_net_message&, bool, bool);
	using SuppressLists = bool(__thiscall*)(void*, int, bool);
	using iFindMdl = MDLHandle_t(__thiscall*)(void*, char*);
	using o_MdlName = const char*(__thiscall*)(void*, void*);
	using EyeAng = QAngle*(__thiscall*)(void*, void*);
	using ReadPacketEntities = void(__thiscall*)(void*, void*);
	using FireEvent = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
	using PostUpdate = void(__thiscall*)(IClientNetworkable* thisptr, int update_type);
	typedef bool(__thiscall* add_listener_t)(IGameEventManager2*, IGameEventListener2*, const char*, bool);
	typedef void(__fastcall* draw_model_execute_t)(void*, int, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using is_hltv_t = bool(__fastcall*) ();
	bool	__fastcall	send_net_msg_hook(void*, int, i_net_message&, bool, bool);
	//void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __fastcall hkEmitSound1(void* _this, int, IRecipientFilter & filter, int iEntIndex, int iChannel, const char * pSoundEntry, unsigned int nSoundEntryHash, const char * pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector * pOrigin, const Vector * pDirection, void * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	void __fastcall hkDrawModelExecute(void*, int, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	long __stdcall	hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	long __stdcall  hkEndScene(IDirect3DDevice9* pDevice);
	void hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output);
	bool __stdcall hkCreateMove(float input_sample_frametime, CUserCmd* cmd);
	bool	__fastcall	fire_event_hook(void*, void*, IGameEvent*);
	typedef bool(__fastcall* fire_event_t)(void*, void*, IGameEvent*);
	typedef bool(__fastcall* create_move_t) (IClientMode*, void*, float, CUserCmd*);
	typedef bool(__fastcall* override_view_t) (void*, int, CViewSetup*);
	void	__fastcall  paint_traverse_hook(PVOID, int, unsigned int, bool, bool);
	typedef void(__thiscall* paint_traverse_t) (PVOID, unsigned int, bool, bool);
	int __fastcall hkDoPostScreenEffects(void* _this, int edx, int a1);
	typedef int(__fastcall* do_post_screen_effects_t)(void*, int, int);
	void __fastcall hkOverrideView(void* _this, int edx, CViewSetup* vsView);
	void __fastcall hkReadPacketEntities(void* pClientState, void* pEDX, void* pEntInfo);
	bool AddCCSPlayerListener(IGameEventListener2* listener, const char* name, const bool serverside);
	void __stdcall hkSuppressLists(int a2, bool a3);
	void __fastcall hkLockCursor(void* _this);
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx);
	bool __fastcall hkgrenadepreviewGetBool(PVOID pConVar, void* edx);
	int  __fastcall hkListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);
	void	__fastcall	frame_stage_notify_hook(void*, int, ClientFrameStage_t);
	typedef void(__fastcall* frame_stage_notify_t)(void*, int, ClientFrameStage_t);

	inline CGameEventListener* listener;
}
