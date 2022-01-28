#pragma once
#include "Ragebot.hpp"
#include <minmax.h>
#include <algorithm>

Ragebot g_Ragebot;
int32_t nTickBaseShift = 0;
float DotProduct(const float* a, const float* b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


// lots of math stuff, i need to fix the math.cpp file because you cant access like 50% of stuff from it. sorry for ugly code. -drifttwo/voozez
// fixed this

// this is so fucking ugly, but ya know what, who cares.


float Ragebot::GetInterp()
{
	int updaterate = g_CVar->FindVar("cl_updaterate")->GetInt();
	ConVar* minupdate = g_CVar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = g_CVar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = g_CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_CVar->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = g_CVar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = g_CVar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = std::clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, (ratio / updaterate));
}

bool ShouldBaim(C_BasePlayer* pEnt, matrix3x4_t* boneMatrix) // probably dosnt make sense
{

	auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

	std::vector<int> baim_hitboxes;

	baim_hitboxes.push_back((int)HITBOX_STOMACH);
	baim_hitboxes.push_back((int)HITBOX_PELVIS);
	baim_hitboxes.push_back((int)HITBOX_CHEST);

	float Damage = 0;
	for (auto HitBoxID : baim_hitboxes)
	{
		Damage = CAutoWall::Get().CanHit(pEnt->get_hitbox_position(HitBoxID, boneMatrix).value());
	}

	if (pEnt->m_iHealth() + 5 <= Damage)
		return true;

	return false;
}

std::vector<int> Ragebot::GetHitboxes(C_BasePlayer* pl, matrix3x4_t* mat)
{
	std::vector<int> hitboxes;
	bool baim = ShouldBaim(pl, mat);
	if (!baim) {
		if (g_Options.rage_hitscan[0])
			hitboxes.push_back((int)Hitboxes::HITBOX_HEAD);
		if (g_Options.rage_hitscan[1])
			hitboxes.push_back((int)Hitboxes::HITBOX_NECK);
		if (g_Options.rage_hitscan[2])
		{
			hitboxes.push_back((int)Hitboxes::HITBOX_UPPER_CHEST);
			hitboxes.push_back((int)Hitboxes::HITBOX_CHEST);
			hitboxes.push_back((int)Hitboxes::HITBOX_LOWER_CHEST);
		}
		if (g_Options.rage_hitscan[3])
			hitboxes.push_back((int)Hitboxes::HITBOX_STOMACH);
		if (g_Options.rage_hitscan[4])
			hitboxes.push_back((int)Hitboxes::HITBOX_PELVIS);
		if (g_Options.rage_hitscan[5])
		{
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_FOREARM);
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_FOREARM);
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_UPPER_ARM);
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_UPPER_ARM);
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_HAND);
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_HAND);
		}
		if (g_Options.rage_hitscan[6])
		{
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_CALF);
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_CALF);
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_THIGH);
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_THIGH);
			hitboxes.push_back((int)Hitboxes::HITBOX_LEFT_FOOT);
			hitboxes.push_back((int)Hitboxes::HITBOX_RIGHT_FOOT);
		}
	}
	else {

		hitboxes.push_back((int)Hitboxes::HITBOX_UPPER_CHEST);
		hitboxes.push_back((int)Hitboxes::HITBOX_CHEST);
		hitboxes.push_back((int)Hitboxes::HITBOX_LOWER_CHEST);
		hitboxes.push_back((int)Hitboxes::HITBOX_STOMACH);
		hitboxes.push_back((int)Hitboxes::HITBOX_PELVIS);
	}

	return hitboxes;
}

PointInfo BestHitPoint(C_BasePlayer* player, int prioritized, matrix3x4_t matrix[])
{
	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(player->GetModel());

	if (!pStudioModel)
		return 0.f;

	mstudiohitboxset_t* hitset = pStudioModel->GetHitboxSet(0);

	if (!hitset)
		return 0.f;

	mstudiobbox_t* hitbox = hitset->GetHitbox(prioritized);
	if (!hitbox)
		return 0.f;


	std::vector<Vector> vecArray;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector max;
	Vector min;

	Math::VectorTransform(hitbox->bbmax + mod, matrix[hitbox->bone], max);
	Math::VectorTransform(hitbox->bbmin - mod, matrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	QAngle curAngles = Math::CalcAngle(center, g_LocalPlayer->GetEyePos());

	Vector forward;
	Math::AngleVectors(curAngles, forward);

	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);

	Vector top = Vector(0, 0, 1);
	Vector bot = Vector(0, 0, -1);

	switch (prioritized) {
	case 0://head
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += top * (hitbox->m_flRadius * g_Options.HeadScale * 0.01);
		vecArray[2] += right * (hitbox->m_flRadius * g_Options.HeadScale * 0.01);
		vecArray[3] += left * (hitbox->m_flRadius * g_Options.HeadScale * 0.01);
		break;
	default://rest

		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += right * (hitbox->m_flRadius * g_Options.BodyScale * 0.01);
		vecArray[2] += left * (hitbox->m_flRadius * g_Options.BodyScale * 0.01);
		break;
	}


	PointInfo pointinfo;

	for (Vector cur : vecArray)
	{
		float flCurDamage = CAutoWall::Get().CanHit(cur);

		if (!flCurDamage)
			continue;

		auto mindamage = min(player->m_iHealth(), Globals::MinDmgValue);

		if ((flCurDamage > pointinfo.damage) && (flCurDamage > mindamage))
		{
			pointinfo.damage = flCurDamage;
			pointinfo.point = cur;


			if (pointinfo.damage > player->m_iHealth())
				return pointinfo;
		}

	}

	return pointinfo;
}

void update_Fake_state(CBasePlayerAnimStatenew* state, QAngle ang) {
	using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto ret = reinterpret_cast<fn>(Utils::PatternScan(GetModuleHandleW(L"client.dll"), ("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24")));

	if (!ret)
		return;

	ret(state, NULL, NULL, ang.yaw, ang.pitch, NULL);
}

void CreateAnimationState(CBasePlayerAnimStatenew* state)
{
	using CreateAnimState_t = void(__thiscall*)(CBasePlayerAnimStatenew*, C_BaseEntity*);
	static auto CreateAnimState = (CreateAnimState_t)Utils::PatternScan(GetModuleHandleW(L"client.dll"), ("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	if (!CreateAnimState)
		return;

	CreateAnimState(state, g_LocalPlayer);
}

void Ragebot::UpdateFakeAnimations()
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;


	if (m_fake_spawntime != g_LocalPlayer->m_flSpawnTime() || m_should_update_fake)
	{
		init_fake_anim = false;
		m_fake_spawntime = g_LocalPlayer->m_flSpawnTime();
		m_should_update_fake = false;
	}

	if (!init_fake_anim)
	{
		m_fake_state = static_cast<CBasePlayerAnimStatenew*> (g_pMemAlloc->Alloc(sizeof(CBasePlayerAnimStatenew)));

		if (m_fake_state != nullptr)
			CreateAnimationState(m_fake_state);

		init_fake_anim = true;
	}

	if (Globals::SendPacket)
	{
		std::array<AnimationLayer, 13> networked_layers;
		std::memcpy(&networked_layers, g_LocalPlayer->GetAnimOverlays(), sizeof(AnimationLayer) * 13);

		auto backup_abs_angles = g_LocalPlayer->GetAbsAngles();
		auto backup_poses = g_LocalPlayer->m_flPoseParameter();
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			g_LocalPlayer->m_fFlags() |= FL_ONGROUND;
		else
		{
			if (g_LocalPlayer->GetAnimOverlays()[4].m_flWeight != 1.f && g_LocalPlayer->GetAnimOverlays()[5].m_flWeight != 0.f)
				g_LocalPlayer->m_fFlags() |= FL_ONGROUND;

			if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
				g_LocalPlayer->m_fFlags() &= ~FL_ONGROUND;
		}

		update_Fake_state(m_fake_state, Globals::cmd->viewangles);
		m_got_fake_matrix = g_LocalPlayer->SetupBones(Globals::FakeAngleMatrix, 128, 524032 - 66666/*g_Options.nightmodeval*/, false);
		const auto org_tmp = g_LocalPlayer->GetRenderOrigin();
		if (m_got_fake_matrix)
		{
			for (auto& i : Globals::FakeAngleMatrix)
			{
				i[0][3] -= org_tmp.x;
				i[1][3] -= org_tmp.y;
				i[2][3] -= org_tmp.z;
			}
		}
		std::memcpy(g_LocalPlayer->GetAnimOverlays(), &networked_layers, sizeof(AnimationLayer) * 13);

		g_LocalPlayer->m_flPoseParameter() = backup_poses;
		g_LocalPlayer->GetAbsAngles() = backup_abs_angles;
	}
}

PointInfo Ragebot::HitScan(C_BasePlayer* pBaseEntity)
{


	matrix3x4_t matrix[128];
	int MissedShots = Globals::ShotsFired[pBaseEntity->EntIndex()] - Globals::ShotsHit[pBaseEntity->EntIndex()];


	*(Vector*)((uintptr_t)pBaseEntity + 0xA0) = pBaseEntity->m_vecOrigin();
	*(int*)((uintptr_t)pBaseEntity + 0xA68) = 0;
	*(int*)((uintptr_t)pBaseEntity + 0xA30) = 0;
	pBaseEntity->InvalidateBoneCache();
	if (!pBaseEntity->SetupBones(matrix, 128, BONE_USED_BY_HITBOX, g_GlobalVars->curtime))
		return PointInfo();

	auto hitboxes = GetHitboxes(pBaseEntity, matrix);
	std::vector<int> HitboxesToScan;

	PointInfo bestPoint;

	for (auto hitbox : hitboxes)
	{
		auto point = BestHitPoint(pBaseEntity, hitbox, matrix);

		if (point.damage > bestPoint.damage)
		{
			bestPoint = point;
		}
	}


	if ((g_Options.RageAimbotBaimAfter && MissedShots >= g_Options.RageAimbotBaimAfter) || ForceSafePoint[pBaseEntity->EntIndex()])
	{
		HitboxesToScan.erase(HitboxesToScan.begin(), HitboxesToScan.begin() + HitboxesToScan.size());
		for (int i = HITBOX_PELVIS; i <= HITBOX_UPPER_CHEST; i++)
		{
			HitboxesToScan.push_back(i);
		}
	}

	return bestPoint;
}

Target_t Ragebot::FindTarget(CUserCmd* cmd)
{
	float bestDistance = 8192.f;
	Target_t bestTarget;
	PointInfo bestInfo;

	for (int it = 1; it < g_EngineClient->GetMaxClients(); ++it)
	{
		C_BasePlayer* pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(it);

		if (!pEntity
			|| pEntity == g_LocalPlayer
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive()
			|| pEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		{
			continue;
		}

		float distance = pEntity->GetEyePos().DistTo(g_LocalPlayer->GetEyePos());

		if (distance < bestDistance)
		{
			auto pointinfo = HitScan(pEntity);

			if (pointinfo.damage > bestInfo.damage)
			{
				bestDistance = distance;
				bestInfo = pointinfo;
				bestTarget = Target_t(pEntity, pointinfo.point);
			}
		}
	}

	return bestTarget;
}

void Autostop(CUserCmd* cmd)
{
	if (!g_Options.rage_autostop)
		return;

	cmd->forwardmove = 0;
	cmd->sidemove = 0;
	//	g::autostoping = true;

		// P1000000 SELF CODE DUDE!!!!!!!11
}

bool Ragebot::HitChance(C_BasePlayer* pEnt, C_BaseCombatWeapon* pWeapon, QAngle Angle, float chance)
{
	float Seeds = (chance <= 1.f) ? 356.f : 256.f;

	Vector forward, right, up;

	Math::AngleVectors(Angle, forward, right, up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	pWeapon->UpdateAccuracyPenalty();

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{


		float a = RandomFloat(0.f, 1.f);
		float b = RandomFloat(0.f, 2.f * M_PI);
		float c = RandomFloat(0.f, 1.f);
		float d = RandomFloat(0.f, 2.f * M_PI);

		if (pWeapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}
		else if (pWeapon->m_iItemDefinitionIndex() == WEAPON_NEGEV && pWeapon->m_flRecoilIndex() < 3.f)
		{
			for (int i = 3; i > pWeapon->m_flRecoilIndex(); --i)
			{
				a *= a;
				c *= c;
			}

			a = 1.f - a;
			c = 1.f - c;
		}
		float Inaccuracy = a * weapInaccuracy;
		float Spread = c * weapSpread;

		Vector spreadView((cos(b) * Inaccuracy) + (cos(d) * Spread), (sin(b) * Inaccuracy) + (sin(d) * Spread), 0), direction, viewForward;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalized();

		QAngle viewanglesSpread;

		Math::VectorAngles(direction, up, viewanglesSpread);
		Math::FixAngles(viewanglesSpread);

		Math::AngleVectors(viewanglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g_LocalPlayer->GetEyePos() + (direction * pWeapon->GetCSWeaponData()->flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init(g_LocalPlayer->GetEyePos(), viewForward);
		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, pEnt, &tr);

		if (tr.hit_entity == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
	/*
	float inaccuracy = pWeapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy < chance;*/
}

bool CanFire()
{

	if (!g_LocalPlayer->m_hActiveWeapon())
		return false;

	if (g_LocalPlayer->m_hActiveWeapon()->IsReloading() || g_LocalPlayer->m_hActiveWeapon()->m_iClip1() <= 0)
		return false;

	auto flServerTime = g_GlobalVars->curtime;

	return g_LocalPlayer->m_hActiveWeapon()->m_flNextPrimaryAttack() < flServerTime;
}

void Autoscope(CUserCmd* cmd) {
	if (g_Options.autoscope) {
		if (g_LocalPlayer->m_hActiveWeapon()->IsSniper()) {
			if (!g_LocalPlayer->m_bIsScoped()) {
				cmd->buttons |= IN_ATTACK2;
			}
		}
	} //autoscope

}

float LagFix()
{
	//pAyy fakelag fix bro

	float updaterate = g_CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar* minupdate = g_CVar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = g_CVar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetFloat();

	float ratio = g_CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_CVar->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = g_CVar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = g_CVar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = std::clamp(ratio, cmin->GetFloat(), cmax->GetFloat());


	return max(lerp, ratio / updaterate);

}

bool Ragebot::CockRevolver(CUserCmd* cmd, C_BaseCombatWeapon* weapon)
{
	bool ret = false;
	if (weapon->get_item_definition_index() == ItemDefinitionIndex::WEAPON_REVOLVER)
	{
		static int delay = 0;
		delay++;
		if (delay <= 15)
		{
			cmd->buttons |= IN_ATTACK;
			ret = true;
		}

		else {
			delay = 0;
			ret = false;
		}
	}
	return ret;
}

void Ragebot::Fire(CUserCmd* cmd)
{
	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	C_BasePlayer* pBaseEntity;

	if (!CanFire())
		return;
	auto tTarget = this->FindTarget(cmd); // gets target for aimbot
	if (!(cmd->buttons & IN_ATTACK2) && tTarget.pEnt && tTarget.vPos.IsValid())
	{
		QAngle result; // shot angle



		result = Math::CalcAngle(g_LocalPlayer->GetEyePos(), tTarget.vPos); //calculates shot angle

		auto aimpunchangle = g_LocalPlayer->m_aimPunchAngle(); //gets aim punch for no recoil

		result -= (aimpunchangle * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()); // more recoilkstuff

		bool hitchance = HitChance(tTarget.pEnt, weapon, result, Globals::HitchanceValue); // hitchance calculation

		Autoscope(cmd);

		Autostop(cmd);

		float damage = CAutoWall::Get().CanHit(tTarget.vPos);

		if (hitchance) // if hitchance is true(aka can hit)
		{
			auto mindamage = min(tTarget.pEnt->m_iHealth(), Globals::MinDmgValue);

			if (damage > mindamage) { // second min damage calculation, for accuracy.
				cmd->viewangles = result; // sets to result aka where it aims
				cmd->buttons |= IN_ATTACK; //shoots
				cmd->tick_count = TIME_TO_TICKS(tTarget.pEnt->m_flSimulationTime() + GetInterp()); // lag comp stuff(or helps to hit moving enemies maybe? dont know tbh.
			}

			//G::RealAngles = cmd->viewangles;
		}
	}

}

// i really hate this as a ragebot, it confuses me but whatever what can i do

void Ragebot::extrapolation() {
	for (int i = 1; i < g_GlobalVars->maxClients; i++) {
		C_BasePlayer* e = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (!e
			|| !e->IsAlive()
			|| e->IsDormant()) {
			continue;
		}

		float simtime_delta = e->m_flSimulationTime() - e->m_flOldSimulationTime();
		int choked_ticks = simtime_delta;
		Vector lastOrig;

		if (lastOrig.Length() != e->m_vecOrigin().Length())
			lastOrig = e->m_vecOrigin();

		float delta_distance = (e->m_vecOrigin() - lastOrig).LengthSqr();
		if (delta_distance > 4096.f) {
			Vector velocity_per_tick = e->m_vecVelocity() * g_GlobalVars->interval_per_tick;
			auto new_origin = e->m_vecOrigin() + (velocity_per_tick * choked_ticks);
			e->SetOrigin(new_origin);
		}
	}
} //vro extrapolation

void Ragebot::WeaponSettings(C_BaseCombatWeapon* Weapon)
{
	if (Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_SCAR20 ||
		Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_G3SG1)
	{
		Globals::HitchanceValue = g_Options.RageAimbotHitchanceAuto;
		Globals::MinDmgValue = g_Options.RageAimbotMinDmgAuto;
	}
	else if (Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_SSG08)
	{
		Globals::HitchanceValue = g_Options.RageAimbotHitchanceScout;
		Globals::MinDmgValue = g_Options.RageAimbotMinDmgScout;
	}
	else if (Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_AWP)
	{
		Globals::HitchanceValue = g_Options.RageAimbotHitchanceAWP;
		Globals::MinDmgValue = g_Options.RageAimbotMinDmgAWP;
	}
	else if (Weapon->IsPistol())
	{
		if (Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_DEAGLE ||
			Weapon->m_Item().m_iItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			Globals::HitchanceValue = g_Options.RageAimbotHitchanceDeagle;
			Globals::MinDmgValue = g_Options.RageAimbotMinDmgDeagle;
		}
		else
		{
			Globals::HitchanceValue = g_Options.RageAimbotHitchancePistols;
			Globals::MinDmgValue = g_Options.RageAimbotMinDmgPistols;
		}
	}
	else
	{
		Globals::HitchanceValue = g_Options.RageAimbotHitchanceOther;
		Globals::MinDmgValue = g_Options.RageAimbotMinDmgOther;
	}
}

void Ragebot::DoRagebot(CUserCmd* cmd, C_BaseCombatWeapon* Weapon) { // runs ragebot stuff
	if (!g_Options.rage_enabled)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;
	// checks if localentity exists, if is ingame, and is alive
	WeaponSettings(Weapon);

	if (g_Options.rage_autor8)
		CockRevolver(cmd, Weapon);

	this->Fire(cmd); // shoots at it
}