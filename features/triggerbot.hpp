#pragma once

#include "../valve_sdk/sdk.hpp"
#include "../valve_sdk/csgostructs.hpp"

class C_BasePlayer;
class CUserCmd;
class IGameEvent;

class Triggerbot : public Singleton<Triggerbot>
{
public:
	Triggerbot() {}

	bool IsLineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);

	bool HitChance(QAngle angles, C_BasePlayer* ent, float chance);

	void OnCreateMove(CUserCmd* pCmd);
};