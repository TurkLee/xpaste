#pragma once
#include "../../valve_sdk/csgostructs.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/interfaces/IClientEntity.hpp"
#include "../../helpers/math.hpp"
#include "../../helpers/utils.hpp"
#include "../../valve_sdk/interfaces/ILocalize.hpp"
#include "../../options.hpp"
#include "../../helpers/singleton.hpp"
#include "../../valve_sdk/interfaces/CInput.hpp"
#include "../../valve_sdk/interfaces/IGameEventmanager.hpp"
#include "../../valve_sdk\interfaces\IMDLCache.hpp"

extern bool sendPacket;
extern bool aimstepInProgress;
extern bool aaSide;

namespace AntiAims
{
	float Distance(Vector a, Vector b);
	bool GetBestHeadAngle(QAngle& angle);
	bool HasViableEnemy();
	void DoAntiAimY(QAngle& angle, int command_number, bool bFlip, bool& clamp);
	void DoAntiAimX(QAngle& angle, bool bFlip, bool& clamp);
	void OnCreateMove(CUserCmd* cmd);

}