#include "nightmode.hpp"
#include "../options.hpp"

static ConVar* old_sky_name;
bool executed = false;

void LoadNamedSky(const char* sky_name) {
	using Fn = void(__fastcall*)(const char*);
	static auto load_named_sky_fn = reinterpret_cast<Fn>(Utils::PatternScan(GetModuleHandleA(("engine.dll")), ("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
	load_named_sky_fn(sky_name);
}

void NightMode::run() noexcept {
	g_Options.nightmode ? apply() : remove();

	static auto r_drawspecificstaticprop = g_CVar->FindVar(("r_DrawSpecificStaticProp"));
	r_drawspecificstaticprop->SetValue(g_Options.nightmode ? 0 : 1);

	static auto r_3dsky = g_CVar->FindVar(("r_3dsky"));
	r_3dsky->SetValue(g_Options.nightmode ? 0 : 1);
}

void NightMode::apply() noexcept {
	if (executed) {
		return;
	}

	auto local_player = reinterpret_cast<player_info_t*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (!local_player)
		return;

	old_sky_name = g_CVar->FindVar(("sv_skyname"));
	float brightness = g_Options.nightmode_brightness / 100.f;

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i)) {
		auto material = g_MatSystem->GetMaterial(i);

		if (!material)
			continue;

		if (strstr(material->GetTextureGroupName(), ("World"))) {
			material->ColorModulate(brightness, brightness, brightness);
		}
		else if (strstr(material->GetTextureGroupName(), ("StaticProp"))) {
			material->ColorModulate(brightness + 0.25f, brightness + 0.25f, brightness + 0.25f);
		}
		if (strstr(material->GetTextureGroupName(), (("SkyBox")))) {
			material->ColorModulate(g_Options.clr_sky[0], g_Options.clr_sky[1], g_Options.clr_sky[2]);
		}
	}

	switch (g_Options.misc_sky_boxes)
	{
	case 0:
		LoadNamedSky("sky_csgo_night02");

		break;
	case 1:


		LoadNamedSky("space_1");
		break;
	case 2:


		LoadNamedSky("space_3");
		break;
	case 3:


		LoadNamedSky("space_4");
		break;
	case 4:


		LoadNamedSky("space_5");
		break;
	case 5:


		LoadNamedSky("space_6");
		break;
	case 6:


		LoadNamedSky("space_7");
		break;
	case 7:


		LoadNamedSky("space_8");
		break;
	case 8:


		LoadNamedSky("space_9");
		break;
	case 9:


		LoadNamedSky("space_10");
		break;
	case 10:


		LoadNamedSky("sky_descent");
		break;
	}

	executed = true;
}

void NightMode::remove() noexcept {
	if (!executed) {
		return;
	}

	auto local_player = reinterpret_cast<player_info_t*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (!local_player)
		return;

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i)) {
		auto material = g_MatSystem->GetMaterial(i);

		if (!material)
			continue;

		if (strstr(material->GetTextureGroupName(), ("World"))) {
			material->ColorModulate(1.f, 1.f, 1.f);
		}
		else if (strstr(material->GetTextureGroupName(), ("StaticProp"))) {
			material->ColorModulate(1.f, 1.f, 1.f);
		}
		if (strstr(material->GetTextureGroupName(), (("SkyBox")))) {
			material->ColorModulate(1.f, 1.f, 1.f);
		}
	}

	if (old_sky_name)
		LoadNamedSky(old_sky_name->GetString());

	executed = false;
}