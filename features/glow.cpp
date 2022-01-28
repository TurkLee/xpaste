#include "glow.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../Korumalar/Atakan75.hpp"

Glow::Glow()
{
}

Glow::~Glow()
{

}

void Glow::Shutdown()
{

	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
		auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		glowObject.m_flAlpha = 0.0f;
	}
}

void Glow::Run()
{


	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
		auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		auto class_id = entity->GetClientClass()->m_ClassID;
		auto color = Color{};

		switch (class_id) {
		case ClassId_CCSPlayer:
		{
			auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

			if (!entity->IsAlive())
				continue;

			if (!is_enemy && g_Options.glow_enemy_only)
				continue;

			if (g_Options.esp_visible_only && !g_LocalPlayer->CanSeePlayer(entity, HITBOX_CHEST, true))
				continue;
			color = Color(g_Options.color_glow);

			break;
		}

		default:
		{
		}
		}

		glowObject.m_flRed = color.r() / 255.0f;
		glowObject.m_flGreen = color.g() / 255.0f;
		glowObject.m_flBlue = color.b() / 255.0f;
		glowObject.m_flAlpha = color.a() / 255.0f;
		glowObject.m_nGlowStyle = g_Options.glow_pulsing;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_bRenderWhenUnoccluded = false;
	}
}