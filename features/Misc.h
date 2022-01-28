#pragma once

#include "../helpers/Singleton.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../hooks.hpp"

class Misc : public Singleton<Misc>
{
public:
	void SolEl();
	void MoonWalk(CUserCmd* pCmd);
	void SkyChanger();
	void SohbetSpam();
	bool RemoveSleeves(const char* modelName);
	void AutoDefuse(C_BaseEntity* pLocal, CUserCmd* pCmd);
	void RemoveCrouchCooldown(CUserCmd* cmd);
	void DisablePanoramaBlur();
	void Kick(int userid);
private:
	QAngle m_aimPunchAngle[128];
};

