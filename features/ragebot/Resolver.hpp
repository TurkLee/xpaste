#pragma once
#include "../../helpers/utils.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/interfaces/IClientMode.hpp"
#include <deque>
struct resolver_player_info
{
	float moving_lby;
	int shots_missed = 0;
};

class Resolver
{
public:
	std::array<resolver_player_info, 65> player_res_info;

	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];
	bool ForceSafePoint[65];

	void BruteForce(C_BasePlayer* Player, CBasePlayerAnimStatenew* Animstate);

	float get_standing_yaw(C_BasePlayer* player);

	float get_moving_yaw(C_BasePlayer* player);

	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
private:
};
extern Resolver g_Resolver;