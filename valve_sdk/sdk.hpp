#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Misc/Enums.hpp"
#include "Misc/vfunc.hpp"
#include "interfaces/memalloc.h"
#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"
#include "Interfaces/ILocalize.hpp"
#include "Interfaces/IAppSystem.hpp"
#include "interfaces/IGameRules.h"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/CInput.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"
#include "Interfaces/IInputSystem.hpp"
#include "Interfaces/IRefCounted.hpp"
#include "Interfaces/IFileSystem.hpp"

#include "../IViewRenderBeams.h"
#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/glow_outline_effect.hpp"
#include "Misc/datamap.hpp"
#include "../Match.h"
#include "netvars.hpp"

class WeapInfo_t
{
public:
    __int32 weapon_type()
    {
        return *reinterpret_cast<__int32*>(reinterpret_cast<uintptr_t>(this) + 0xC8);
    }

    __int32 m_iDamage()
    {
        return *reinterpret_cast<__int32*>(reinterpret_cast<uintptr_t>(this) + 0xF0);
    }

    float m_fArmorRatio()
    {
        return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xF4);
    }

    float m_fPenetration()
    {
        return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xFC);
    }

    float m_fRange()
    {
        return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x108);
    }

    float m_fRangeModifier()
    {
        return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x10C);
    }

    bool m_bHasSilencer()
    {
        return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x120);
    }
};

struct RenderableInfo_t {
    IClientRenderable* m_pRenderable;
    void* m_pAlphaProperty;
    int m_EnumCount;
    int m_nRenderFrame;
    unsigned short m_FirstShadow;
    unsigned short m_LeafList;
    short m_Area;
    uint16_t m_Flags;   // 0x0016
    uint16_t m_Flags2; // 0x0018
    Vector m_vecBloatedAbsMins;
    Vector m_vecBloatedAbsMaxs;
    Vector m_vecAbsMins;
    Vector m_vecAbsMaxs;
    int pad;
};


#include "interfaces/NetworkString.hpp"

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

namespace Global
{
    inline C_BaseEntity* pLocalEntity;
	inline int iHwidIndex = 0;
    inline CUserCmd* cmd;
    inline C_BaseEntity* localplayer;
}
namespace Globals {
    inline bool fakefpscheck = false;
    inline int iHwidIndex = 0;
    inline CUserCmd* cmd;
    inline C_BaseEntity* localplayer;
    inline int ShotsFired[65];
    inline int ShotsHit[65];
    inline float HitchanceValue;
    inline float MinDmgValue;
    inline bool UseFreestand[65];
    inline float FreestandAngle[65];
    inline float LastFreestandAngle[65];
    inline bool ForceSafePoint[65];
    inline bool SendPacket;
    inline matrix3x4_t FakeLagMatrix[128];
    inline matrix3x4_t FakeAngleMatrix[128];
}

class C_CSPlayerResource;
inline IVEngineClient*       g_EngineClient;
inline match_framework*		 g_match_framework;
inline IBaseClientDLL*       g_CHLClient;
inline IClientEntityList*    g_EntityList;
inline CGlobalVarsBase*      g_GlobalVars;
inline IEngineTrace*         g_EngineTrace;
inline ICvar*                g_CVar;
inline IPanel*               g_VGuiPanel;
inline IClientMode*          g_ClientMode;
inline IVDebugOverlay*       g_DebugOverlay;
inline ISurface*             g_VGuiSurface;
inline CInput*               g_Input;
inline IVModelInfoClient*    g_MdlInfo;
inline IVModelRender*        g_MdlRender;
inline uintptr_t*			 g_SpatialPartition;
inline IVRenderView*         g_RenderView;
inline IMaterialSystem*      g_MatSystem;
inline IViewRenderBeams*	 g_RenderBeams;
inline IGameEventManager2*   g_GameEvents;
inline IMoveHelper*          g_MoveHelper;
inline IMDLCache*            g_MdlCache;
inline IPrediction*          g_Prediction;
inline CGameMovement*        g_GameMovement;
inline IEngineSound*         g_EngineSound;
inline CGlowObjectManager*   g_GlowObjManager;
inline IViewRender*          g_ViewRender;
inline IDirect3DDevice9*     g_D3DDevice9;
inline CClientState*         g_ClientState;
inline IPhysicsSurfaceProps* g_PhysSurface;
inline ILocalize*			 g_Localize;
inline IWeaponSystem*		 g_WeaponSystem;
inline IInputSystem*         g_InputSystem;
inline C_CSPlayerResource**  g_PlayerResource;
inline IGameRules**			 g_GameRules;
inline IMemAlloc*			 g_pMemAlloc;
inline CNetworkStringTableContainer* g_ClientStringTableContainer = nullptr;
inline i_basefilesystem* g_BaseFileSys;
inline IFileSystem* g_FileSystem = nullptr;

extern void(__cdecl* RandomSeed)(uint32_t seed);
extern float(__cdecl* RandomFloat)(float min, float max);

template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"



class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};
struct FireBulletData
{
    FireBulletData(const Vector& eye_pos) : src(eye_pos)
    {
    }

    Vector src;
    trace_t enter_trace;
    Vector direction;
    CTraceFilter filter;
    float trace_length;
    float trace_length_remaining;
    float current_damage;
    int penetrate_count;
};

inline IViewRenderBeams* g_Beams;

inline C_LocalPlayer g_LocalPlayer;

