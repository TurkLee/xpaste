#pragma once
#include "../../valve_sdk/csgostructs.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../options.hpp"
#include "rautowall.hpp"
#include "../../helpers/math.hpp"

struct PointInfo
{
public:
	PointInfo(float damage = 0.f, Vector point = Vector(0.f, 0.f, 0.f))
	{
		this->damage = damage;
		this->point = point;
	}

	float damage;
	Vector point;
};

struct Target_t
{
public:
	Target_t(C_BasePlayer* pEnt = nullptr, Vector vPos = Vector(0.f, 0.f, 0.f))
	{
		this->pEnt = pEnt;
		this->vPos = vPos;
	}

	C_BasePlayer* pEnt = nullptr;
	Vector vPos = { 0.f, 0.f, 0.f };
};


class Ragebot
{
public:
	void extrapolation();
	void WeaponSettings(C_BaseCombatWeapon* Weapon);
	void DoRagebot(CUserCmd* cmd, C_BaseCombatWeapon* Weapon);
	void dt(CUserCmd* cmd);
	void UpdateFakeAnimations();

	float GetInterp();
	bool m_should_update_fake = false;
	std::array< AnimationLayer, 13 > m_fake_layers;
	std::array< float, 20 > m_fake_poses;
	CBasePlayerAnimStatenew* m_fake_states = nullptr;
	CBasePlayerAnimStatenew* m_fake_state = nullptr;
	float m_fake_rotation = 0.f;
	bool init_fake_anim = false;
	float m_fake_spawntime = 0.f;
	float m_fake_delta = 0.f;
	matrix3x4_t m_fake_matrix[128];
	matrix3x4_t m_fake_position_matrix[128];
	bool m_got_fake_matrix = false;
	bool ForceSafePoint[65];

private:
	bool HitChance(C_BasePlayer* pEnt, C_BaseCombatWeapon* pWeapon, QAngle Angle, float chance);
	Target_t FindTarget(CUserCmd* cmd);
	bool CockRevolver(CUserCmd* cmd, C_BaseCombatWeapon* weapon);
	void Fire(CUserCmd* cmd);
	std::vector<int> GetHitboxes(C_BasePlayer* pl, matrix3x4_t* mat);
	PointInfo HitScan(C_BasePlayer* pBaseEntity);


};
extern int32_t nTickBaseShift;

extern Ragebot g_Ragebot;

