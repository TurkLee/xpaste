#include "triggerbot.hpp"
#include "../helpers/math.hpp"
#include "../options.hpp"

bool is_delayed = false;
float shot_delay_time;
bool shot_delay = false;
bool Triggerbot::IsLineGoesThroughSmoke(Vector vStartPos, Vector vEndPos) {
	static auto LineGoesThroughSmokeFn = (bool(*)(Vector vStartPos, Vector vEndPos))Utils::PatternScan(GetModuleHandleA(("client.dll")), ("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));
	return LineGoesThroughSmokeFn(vStartPos, vEndPos);
}

bool Triggerbot::HitChance(QAngle angles, C_BasePlayer* ent, float chance)
{
	auto RandomFloat = [](float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	};
	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();

	if (!weapon)
		return false;

	Vector forward, right, up;
	Vector src = g_LocalPlayer->GetEyePos();
	Math::AngleVectors(angles, forward, right, up);

	int cHits = 0;
	int cNeededHits = static_cast<int> (150.f * (chance / 100.f));

	weapon->UpdateAccuracyPenalty();
	float weap_spread = weapon->GetSpread();
	float weap_inaccuracy = weapon->GetInaccuracy();

	for (int i = 0; i < 150; i++)
	{
		float a = RandomFloat(0.f, 1.f);
		float b = RandomFloat(0.f, 2.f * PI_F);
		float c = RandomFloat(0.f, 1.f);
		float d = RandomFloat(0.f, 2.f * PI_F);

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if (weapon->m_Item().m_iItemDefinitionIndex() == 64)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalized();

		QAngle viewAnglesSpread;
		Math::VectorAngles(direction, viewAnglesSpread);
		viewAnglesSpread.Normalize();

		Vector viewForward;
		Math::AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = src + (viewForward * weapon->GetCSWeaponData()->flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, viewForward);
		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, ent, &tr);

		if (tr.hit_entity == ent)
			++cHits;

		if (static_cast<int> ((static_cast<float> (cHits) / 150.f) * 100.f) >= chance)
			return true;

		if ((150 - i + cHits) < cNeededHits)
			return false;
	}

	return false;
}

void Triggerbot::OnCreateMove(CUserCmd* pCmd) {

	if (g_LocalPlayer->IsAlive() && g_LocalPlayer->IsAlive() && !(g_LocalPlayer->m_lifeState() & LIFE_DYING))
	{
		auto pWeapon = g_LocalPlayer->m_hActiveWeapon();

		if (pWeapon)
		{
			static bool enable = false;
			static int	key = 0;
			static bool head = false;
			static bool arms = false;
			static bool chest = false;
			static bool stomach = false;
			static bool legs = false;

			if (pWeapon->IsAllWeapons()) enable = g_Options.enable_trigger;

			if (pWeapon->IsAllWeapons()) key = g_Options.trigger_key;

			if (pWeapon->IsAllWeapons()) head = g_Options.trigger_head;

			if (pWeapon->IsAllWeapons()) arms = g_Options.trigger_arms;

			if (pWeapon->IsAllWeapons()) chest = g_Options.trigger_chest;

			if (pWeapon->IsAllWeapons()) stomach = g_Options.trigger_stomatch;

			if (pWeapon->IsAllWeapons()) legs = g_Options.trigger_legs;

			if (pWeapon->IsKnife())
				return;

			Vector src, dst, forward;
			trace_t tr;
			Ray_t ray;
			CTraceFilter filter;

			QAngle viewangle = pCmd->viewangles;

			viewangle += g_LocalPlayer->m_aimPunchAngle() * 2.f;

			Math::AngleVectors2(viewangle, forward);

			forward *= g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->flRange;
			filter.pSkip = g_LocalPlayer;
			src = g_LocalPlayer->GetEyePos();
			dst = src + forward;
			ray.Init(src, dst);

			g_EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

			if (!tr.hit_entity)
				return;

			static int weapon_index = 7;

			int hitgroup = tr.hitgroup;
			bool didHit = false;
			if ((head && tr.hitgroup == 1)
				|| (chest && tr.hitgroup == 2)
				|| (stomach && tr.hitgroup == 3)
				|| (arms && (tr.hitgroup == 4 || tr.hitgroup == 5))
				|| (legs && (tr.hitgroup == 6 || tr.hitgroup == 7)))
			{
				didHit = true;
			}

			if (IsLineGoesThroughSmoke(g_LocalPlayer->GetEyePos(), tr.endpos) && g_Options.trigger_smokecheck)
				return;

			if (g_LocalPlayer->IsFlashed() && g_Options.trigger_flashcheck)
				return;

			bool OnGround = (g_LocalPlayer->m_fFlags() & FL_ONGROUND);
			if (g_Options.trigger_jumpcheck && !OnGround)
				return;

			auto player = reinterpret_cast<C_BasePlayer*>(tr.hit_entity);
			if (!player || player->IsDormant() || !player->IsAlive() || !player->IsPlayer())
				return;

			if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				return;

			QAngle viewangles = pCmd->viewangles;
			Math::AngleVectors(viewangles, forward);


			if (didHit && HitChance(viewangles, player, g_Options.trigger_hitchance))
			{
				if (key > 0 && GetAsyncKeyState(key) & 0x8000 && enable)
				{
					if (!is_delayed && !shot_delay && g_Options.trigger_delay > 0) {
						is_delayed = true;
						shot_delay = true;
						shot_delay_time = GetTickCount() + g_Options.trigger_delay;
					}

					if (shot_delay && shot_delay_time <= GetTickCount()) {
						shot_delay = false;
					}

					if (!shot_delay)
					{
						is_delayed = false;
						shot_delay = false;
						pCmd->buttons |= IN_ATTACK;
					}
					if (g_Options.autostop2) {
						pCmd->forwardmove = pCmd->sidemove = 0;
					}

				}
			}
			else
			{
				shot_delay_time = 0.f;
				is_delayed = false;
				shot_delay = false;
			}
			if (g_Options.enable_trigger_auto)
			{
				if (didHit && HitChance(viewangles, player, g_Options.trigger_hitchance))
				{
					//if (g_Options.autostop2 | pWeapon->IsAllWeapons()) {
					//	pCmd->forwardmove = pCmd->sidemove = 0;
					//}

					if (!is_delayed && !shot_delay && g_Options.trigger_delay > 0) {
						is_delayed = true;
						shot_delay = true;
						shot_delay_time = GetTickCount() + g_Options.trigger_delay;
					}

					if (shot_delay && shot_delay_time <= GetTickCount()) {
						shot_delay = false;
					}

					if (!shot_delay)
					{
						is_delayed = false;
						shot_delay = false;
						pCmd->buttons |= IN_ATTACK;
					}
					if (g_Options.autostop2) {
						pCmd->forwardmove = pCmd->sidemove = 0;
					}
				}
			}
		}
	}
}