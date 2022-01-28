#include <algorithm>

#include "visuals.hpp"
#include "../hooks.hpp"
#include "../options.hpp"
#include "grenade_prediction.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include <iomanip>
#include "../notification/Globals.hpp"
#include "../Korumalar/Atakan75.hpp"
#include "../helpers/input.hpp"

bomb_planted_event bomb_planted_listener;
round_end_event round_end_listener;
std::string string_format(const std::string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

float GetArmourHealth(float flDamage, int ArmorValue)
{
	float flCurDamage = flDamage;

	if (flCurDamage == 0.0f || ArmorValue == 0)
		return flCurDamage;

	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	float flNew = flCurDamage * flArmorRatio;
	float flArmor = (flCurDamage - flNew) * flArmorBonus;

	if (flArmor > ArmorValue)
	{
		flArmor = ArmorValue * (1.0f / flArmorBonus);
		flNew = flCurDamage - flArmor;
	}

	return flNew;
}

void VisualsRender::CreateFonts()
{
	VisualsFont = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(VisualsFont, "Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE);

	EspFont = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(EspFont, "Visitor TT2 BRK", 10, 400, 0, 0, FONTFLAG_DROPSHADOW);

	watermark_ui_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(watermark_ui_font, "Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	small_pixel_11 = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(small_pixel_11, "Smallest Pixel-7", 11, 100, 0, 0, FONTFLAG_DROPSHADOW);

	verdanaBold_12 = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(verdanaBold_12, "Comic Sans", 24, 700, 0, 0, FONTFLAG_DROPSHADOW);
}

void VisualsRender::Text(int X, int Y, const char* Text, vgui::HFont Font, Color DrawColor, bool Center)
{
	std::wstring WText = std::wstring(std::string_view(Text).begin(), std::string_view(Text).end());

	g_VGuiSurface->DrawSetTextFont(Font);
	g_VGuiSurface->DrawSetTextColor(DrawColor);

	if (Center)
	{
		int TextWidth, TextHeight;
		VisualsRender::Get().TextSize(TextWidth, TextHeight, Text, Font);
		g_VGuiSurface->DrawSetTextPos(X - TextWidth / 2, Y);
	}
	else g_VGuiSurface->DrawSetTextPos(X, Y);
	g_VGuiSurface->DrawPrintText(WText.c_str(), wcslen(WText.c_str()));
}

void VisualsRender::TextSize(int& Width, int& Height, const char* Text, vgui::HFont Font)
{
	std::wstring WText = std::wstring(std::string_view(Text).begin(), std::string_view(Text).end());
	g_VGuiSurface->GetTextSize(Font, WText.c_str(), Width, Height);
}

void VisualsRender::FilledRectange(int X1, int Y1, int X2, int Y2, Color DrawColor)
{
	g_VGuiSurface->DrawSetColor(DrawColor);
	g_VGuiSurface->DrawFilledRect(X1, Y1, X2, Y2);
}

void VisualsRender::OutlinedRectange(int X1, int Y1, int X2, int Y2, Color DrawColor)
{
	g_VGuiSurface->DrawSetColor(DrawColor);
	g_VGuiSurface->DrawOutlinedRect(X1, Y1, X2, Y2);
}

void VisualsRender::Line(int X1, int Y1, int X2, int Y2, Color DrawColor)
{
	g_VGuiSurface->DrawSetColor(DrawColor);
	g_VGuiSurface->DrawLine(X1, Y1, X2, Y2);
}

void VisualsRender::Circle(int x, int y, int r, int seg, Color color)
{
	g_VGuiSurface->DrawSetColor(0, 0, 0, 255);
	g_VGuiSurface->DrawSetColor(color);
	g_VGuiSurface->DrawOutlinedCircle(x, y, r, seg);
}

void Visuals::FireGameEvent(IGameEvent* event) {
	if (!strcmp(event->GetName(), "player_hurt")) {
		int attacker = event->GetInt("attacker");
		if (g_EngineClient->GetPlayerForUserID(attacker) == g_EngineClient->GetLocalPlayer()) {
			flHurtTime = g_GlobalVars->realtime;
		}
	}
	if (strcmp(event->GetName(), "player_death") == 0) {
		if (!event)
			return;

		auto entity = g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")));
		if (!entity)
			return;

		auto attacker = event->GetInt("attacker");
		if (!attacker)
			return;

		if (g_Options.kill_counter)
		{
			killtime = g_GlobalVars->realtime;
			kills++;
		}
		globalAlpha = g_GlobalVars->curtime;
	}

	if (!strcmp(event->GetName(), "round_start") && g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
		kills = 0;
	}
}

void Visuals::RecoilCrosshair()
{
	auto recoilCrosshair = g_CVar->FindVar("cl_crosshair_recoil");
	recoilCrosshair->SetValue(g_Options.recoilCrosshair ? 1 : 0);
}

void Visuals::QuakeKillCounter()
{
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected()) return;
	if (!g_Options.kill_counter)	return;
	if (!g_LocalPlayer)
		return;

	int screen_sizex, screen_sizey, cW, cH;
	g_EngineClient->GetScreenSize(screen_sizex, screen_sizey);

	cW = screen_sizex / 2;
	cH = screen_sizey / 2;

	float animationAlpha = 0.0;
	int alpha = 0;
	float animation_speed_value = 1.2;
	float step = 255 / 1.2 * g_GlobalVars->frametime;

	if (killtime + 0.8 > g_GlobalVars->realtime)
	{
		alpha = 255;
	}
	else
		alpha = alpha - step;

	float endTime = globalAlpha + 1.1f;
	float t = 1.0f - (endTime - g_GlobalVars->curtime) / (endTime - globalAlpha);

	float animationStep = animation_speed_value * g_GlobalVars->frametime;

	animationAlpha = animationAlpha - animationStep;

	screen_sizey = (screen_sizey / 2) - (t * (60.0f));

	if (alpha > 0) {
		switch (kills) {
		case 1:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(255, 0, 0, alpha), VisualsRender::Get().EspFont, true, "FIRST BLOOD");
			break;
		case 2:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 0, 255, alpha), VisualsRender::Get().EspFont, true, "DOUBLE KILL");
			break;
		case 3:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 255, 0, alpha), VisualsRender::Get().EspFont, true, "TRIPLE KILL");
			break;
		case 4:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(155, 200, 0, alpha), VisualsRender::Get().EspFont, true, "MULTI KILL");
			break;
		case 5:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(200, 0, 167, alpha), VisualsRender::Get().EspFont, true, "RAMPAGE");
			break;
		default:
			g_VGuiSurface->DrawT(screen_sizex / 2, screen_sizey, Color(23, 34, 255, alpha), VisualsRender::Get().EspFont, true, "DOMINATING");
			break;
		}
	}
}

void Visuals::AsusWalls()
{


	static std::string old_Skyname = "";
	static bool OldNightmode;
	static int OldSky;
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
	{
		old_Skyname = "";
		OldNightmode = false;
		OldSky = 0;
		return;
	}
	static ConVar* r_DrawSpecificStaticProp;
	if (OldNightmode != g_Options.gorus_saydamduvarbakim)
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->SetValue(0);
		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_MatSystem->GetMaterial(i);
			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetName(), "models/props"))
			{
				if (g_Options.gorus_saydamduvarbakim)
					pMaterial->AlphaModulate(0.5f);
				else
					pMaterial->AlphaModulate(1.f);
			}
		}
		OldNightmode = g_Options.gorus_saydamduvarbakim;
	}
}

void Visuals::draw_granades(C_BaseEntity* entity)
{
	if (!g_LocalPlayer)
		return;

	std::string name;
	Color clr;

	const model_t* model = entity->GetModel();

	if (entity->GetClientClass())
	{
		if (model)
		{
			studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(model);
			if (hdr)
			{
				std::string hdrName = hdr->szName;
				if (hdrName.find("thrown") != std::string::npos || hdrName.find("dropped") != std::string::npos)
				{
					if (hdrName.find("flashbang") != std::string::npos)
					{
						name = "flash";
						clr = Color(g_Options.color_flash);
					}
					else if (hdrName.find("fraggrenade") != std::string::npos)
					{
						name = "frag";
						clr = Color(g_Options.color_frag);
					}
					else if (hdrName.find("molotov") != std::string::npos)
					{
						name = "molotov";
						clr = Color(g_Options.color_molotov);
					}
					else if (hdrName.find("incendiarygrenade") != std::string::npos)
					{
						name = "incendiary";
						clr = Color(g_Options.color_molotov);
					}
					else if (hdrName.find("decoy") != std::string::npos)
					{
						name = "decoy";
						clr = Color(g_Options.color_decoy);
					}
					else if (hdrName.find("smoke") != std::string::npos)
					{
						name = "smoke";
						clr = Color(g_Options.color_smoke);
					}
				}
			}

			if (!name.empty())
			{
				Vector pos;
				if (Math::WorldToScreen(entity->m_vecOrigin(), pos))
				{
					Render::Get().RenderText(name, ImVec2(pos.x, pos.y), 11.f, clr, false, true);
				}
			}
		}
	}
}

void Visuals::draw_grenades()
{
	if (!g_Options.grenades_enable)
		return;

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		auto m_entity = reinterpret_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(i));

		if (m_entity && m_entity != g_LocalPlayer)
		{
			draw_granades(m_entity);
		}
	}

	for (int i = 0; i < smoke_info.size(); i++)
	{
		Vector position;

		if (Math::WorldToScreen(smoke_info[i].position, position))
		{
			char buf[255];
			snprintf(buf, sizeof(buf), "%4.1f", (float)smoke_info[i].time_to_expire - g_GlobalVars->curtime);

			ImVec2 time_size = ImGui::CalcTextSize(buf);


			if (g_Options.smoke_bar)
			{
				float box_w = (float)fabs((position.x - 30) - (position.x + 30));
				auto width = (((box_w * (smoke_info[i].time_to_expire - g_GlobalVars->curtime)) / 18.f));

				Render::Get().RenderBoxFilled(position.x - 30, position.y + 15, position.x + 30, position.y + 18.f, Color(g_Options.color_bar_smoke_back));
				Render::Get().RenderBoxFilled(position.x - 30, position.y + 15, (position.x - 30) + (int)width, position.y + 18.f, Color(g_Options.color_bar_smoke_main));
				if (g_Options.smoke_timer)
				{
					Render::Get().RenderText(
						buf,
						ImVec2(((position.x - 30) + (int)width) - (time_size.x / 2), position.y + 20.f), 10.f,
						Color(g_Options.color_molotov), false, true);
				}
			}
			else if (g_Options.smoke_timer)
			{
				Render::Get().RenderText(
					buf,
					ImVec2(position.x, position.y + 10), 10.f,
					Color(g_Options.color_smoke), false, true);
			}
		}


		if (smoke_info[i].time_to_expire - g_GlobalVars->curtime < 0)
		{
			smoke_info.erase(smoke_info.begin() + i);
		}

	}

	for (int i = 0; i < molotov_info.size(); i++)
	{
		Vector position;
		if (Math::WorldToScreen(molotov_info[i].position, position))
		{
			char buf[255];
			snprintf(buf, sizeof(buf), "%4.1f", (float)molotov_info[i].time_to_expire - g_GlobalVars->curtime);

			ImVec2 time_size = ImGui::CalcTextSize(buf);
			Render::Get().RenderText("molotov", position.x, position.y, 12.f, Color(g_Options.color_molotov), false, true);

			if (g_Options.molotov_bar)
			{
				float box_w = (float)fabs((position.x - 30) - (position.x + 30));
				auto width = (((box_w * (molotov_info[i].time_to_expire - g_GlobalVars->curtime)) / 7.f));

				Render::Get().RenderBoxFilled(position.x - 30, position.y + 15.f, position.x + 30, position.y + 18.f, Color(g_Options.color_bar_molotov_back));
				Render::Get().RenderBoxFilled(position.x - 30, position.y + 15.f, (position.x - 30) + (int)width, position.y + 18.f, Color(g_Options.color_bar_molotov_main));

				if (g_Options.molotov_timer)
				{
					Render::Get().RenderText(
						buf,
						ImVec2(((position.x - 30) + (int)width) - (time_size.x / 2), position.y + 20.f), 10.f,
						Color(g_Options.color_molotov), false, true);
				}
			}
			else if (g_Options.molotov_timer)
			{
				Render::Get().RenderText(
					buf,
					ImVec2(position.x, position.y + 10), 10.f,
					Color(g_Options.color_molotov));
			}

		}

		if (molotov_info[i].time_to_expire - g_GlobalVars->curtime < 0)
		{
			molotov_info.erase(molotov_info.begin() + i);
		}

	}
}

void Visuals::events(IGameEvent* event)
{
	if (strstr(event->GetName(), "inferno_startburn"))
	{
		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		molotov_info_t temp = { position, g_GlobalVars->curtime + 7.f };
		molotov_info.emplace_back(temp);
	}

	if (strstr(event->GetName(), "smokegrenade_detonate"))
	{
		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		smoke_info_t temp = { position, g_GlobalVars->curtime + 18.f };
		smoke_info.emplace_back(temp);
	}

	if (strstr(event->GetName(), "round_end") || strstr(event->GetName(), "round_prestart") || strstr(event->GetName(), "round_freeze_end"))
	{
		for (int i = 0; i < molotov_info.size(); i++)
			molotov_info.erase(molotov_info.begin() + i);

		for (int i = 0; i < smoke_info.size(); i++)
			smoke_info.erase(smoke_info.begin() + i);
	}

	if (!strcmp(event->GetName(), "player_hurt"))
	{
		C_BasePlayer* hurt = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")));
		C_BasePlayer* attacker = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")));
		if (attacker == g_LocalPlayer)
		{
			//flHurtTime = g_global_vars->realtime;
		}

		bool hurt_dead = event->GetInt("health") == 0;

		if (hurt != g_LocalPlayer && attacker == g_LocalPlayer)
		{

		}
	}
}

void Visuals::Player::debug_visuals()
{
	int x_offset = 0;
	if (g_Options.debug_showposes)
	{
		char act[128];
		RECT bbox = ctx.bbox;
		auto poses = ctx.pl->m_flPoseParameter();
		for (int i = 0; i < 24; ++i) {
			sprintf(act, "Pose %d %f", i, poses[i]);
			//Render::Get().RenderText(act, bbox.right + 5, bbox.top + i * 12, 15.f, Color(10 * i, 255, 255, 255));
		}
		//Visuals::DrawString(Visuals::ui_font, bbox.right + 5, bbox.top + -12, Color(255, 0, 55, 255), FONT_LEFT, "LBY %f", (Visuals::ESP_ctx.player->m_flLowerBodyYawTarget() + 180.f) / 360.f);
		x_offset += 50;
	}

	if (g_Options.debug_showactivities)
	{
		float h = fabs(ctx.feet_pos.y - ctx.head_pos.y);
		float w = h / 2.0f;

		int offsety = 0;
		for (int i = 0; i < ctx.pl->GetNumAnimOverlays(); i++)
		{
			auto layer = ctx.pl->GetAnimOverlay(i);
			int number = layer->m_nSequence,
				activity = ctx.pl->GetSequenceActivity(number);
			std::setprecision(3);
			Render::Get().RenderText(std::to_string(number).c_str(), ctx.bbox.right + 5, ctx.bbox.top + i * 12, 15.f, Color(255, 255, 0, 255));
			Render::Get().RenderText(std::to_string(activity).c_str(), ctx.bbox.right + 75, ctx.bbox.top + i * 12, 15.f, Color(255, 255, 0, 255));
			Render::Get().RenderText(std::to_string(layer->m_flCycle).c_str(), ctx.bbox.right + 145, ctx.bbox.top + i * 12, 15.f, Color(255, 255, 0, 255));
			Render::Get().RenderText(std::to_string(layer->m_flWeight).c_str(), ctx.bbox.right + 215, ctx.bbox.top + i * 12, 15.f, Color(255, 255, 0, 255));
			Render::Get().RenderText(std::to_string(layer->m_flPlaybackRate).c_str(), ctx.bbox.right + 285, ctx.bbox.top + i * 12, 15.f, Color(255, 255, 0, 255));

			/*if (activity == 979)
			{
				Visuals::DrawString(Visuals::ui_font, Visuals::ESP_ctx.head_pos.x + w + x_offset + 65, Visuals::ESP_ctx.head_pos.y + offsety, Color(255, 255, 0, 255), FONT_LEFT, std::to_string(layer.m_flWeight).c_str());
				Visuals::DrawString(Visuals::ui_font, Visuals::ESP_ctx.head_pos.x + w + x_offset + 65, Visuals::ESP_ctx.head_pos.y + offsety + 12, Color(255, 255, 0, 255), FONT_LEFT, std::to_string(layer.m_flCycle).c_str());
			}*/

			offsety += 12;
		}
		x_offset += 100;
	}
}

void draw_hitboxes(C_BasePlayer* player, matrix3x4_t* matrix)
{
	const auto model = player->GetModel();

	if (!model)
		return;

	const auto hdr = g_MdlInfo->GetStudiomodel(model);

	if (!hdr)
		return;

	const auto set = hdr->GetHitboxSet(player->m_nHitboxSet());

	if (!set)
		return;

	for (auto i = 0; i < set->numhitboxes; i++) {
		const auto hitbox = set->GetHitbox(i);

		if (!hitbox)
			continue;

		if (hitbox->m_flRadius == -1.0f)
		{

		}
		else
		{
			Vector min, max;
			Math::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], min);
			Math::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], max);

			g_DebugOverlay->DrawPill(min, max, hitbox->m_flRadius, 0, 255, 0, 160, g_GlobalVars->interval_per_tick * 2.f);
		}
	}
}

float flPlayerAlpha[65];

bool bombisready = false;

void Visuals::Listener()
{
	g_GameEvents->AddListener(&bomb_planted_listener, "bomb_planted", false);
	g_GameEvents->AddListener(&round_end_listener, "round_end", false);
}

void round_end_event::FireGameEvent(IGameEvent* p_event)
{
	/* if we are connected */
	if (!g_EngineClient->IsConnected() || !g_EngineClient->IsInGame() || !g_LocalPlayer)
		return;

	/* return if not event */
	if (!p_event)
		return;

	if (strstr(p_event->GetName(), "round_end")) {
		bombisready = false;
	}
}

void bomb_planted_event::FireGameEvent(IGameEvent* p_event)
{
	/* if we are connected */
	if (!g_EngineClient->IsConnected() || !g_EngineClient->IsInGame() || !g_LocalPlayer)
		return;

	/* return if not event */
	if (!p_event)
		return;

	if (strstr(p_event->GetName(), "bomb_planted")) {
		bombisready = true;
	}
}

void Visuals::RenderSounds() {
	UpdateSounds();

	auto Add3DCircle = [](const Vector& position, Color color, float radius) {
		float precision = 160.0f;

		const float step = DirectX::XM_2PI / precision;

		for (float a = 0.f; a < DirectX::XM_2PI; a += step) {
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
			if (!Math::WorldToScreen(start, start2d) || !Math::WorldToScreen(end, end2d))
				return;

			Render::Get().RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
		}
	};


	for (auto& [entIndex, sound] : m_Sounds) {
		if (sound.empty())
			continue;

		for (auto& info : sound) {
			if (info.soundTime + g_Options.esp_sounds_time < g_GlobalVars->realtime)
				info.alpha -= g_GlobalVars->frametime;

			if (info.alpha <= 0.0f)
				continue;

			float deltaTime = g_GlobalVars->realtime - info.soundTime;

			auto factor = deltaTime / g_Options.esp_sounds_time;
			if (factor > 1.0f)
				factor = 1.0f;

			float radius = g_Options.esp_sounds_radius * factor;
			Color color = Color(g_Options.color_esp_sounds[0], g_Options.color_esp_sounds[1], g_Options.color_esp_sounds[2],
				info.alpha);

			Add3DCircle(info.soundPos, color, radius);
		}

		while (!sound.empty()) {
			auto& back = sound.back();
			if (back.alpha <= 0.0f)
				sound.pop_back();
			else
				break;
		}
	}
}

void Visuals::UpdateSounds() {

	sounds.RemoveAll();
	g_EngineSound->GetActiveSounds(sounds);
	if (sounds.Count() < 1)
		return;

	Vector eye_pos = g_LocalPlayer->GetEyePos();
	for (int i = 0; i < sounds.Count(); ++i) {
		SndInfo_t& sound = sounds.Element(i);
		if (sound.m_nSoundSource < 1)
			continue;

		C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(sound.m_nSoundSource);
		if (!player)
			continue;

		if (player->m_hOwnerEntity().IsValid() && player->IsWeapon()) {
			player = (C_BasePlayer*)player->m_hOwnerEntity().Get();
		}

		if (!player->IsPlayer() || !player->IsAlive())
			continue;

		if ((C_BasePlayer*)g_LocalPlayer == player || (g_Options.esp_enemy_only && g_LocalPlayer->m_iTeamNum() == player->m_iTeamNum()))
			continue;

		if (player->m_vecOrigin().DistTo(g_LocalPlayer->m_vecOrigin()) > 900)
			continue;

		auto& player_sound = m_Sounds[player->EntIndex()];
		if (player_sound.size() > 0) {
			bool should_break = false;
			for (const auto& snd : player_sound) {
				if (snd.guid == sound.m_nGuid) {
					should_break = true;
					break;
				}
			}

			if (should_break)
				continue;
		}

		SoundInfo_t& snd = player_sound.emplace_back();
		snd.guid = sound.m_nGuid;
		snd.soundPos = *sound.m_pOrigin;
		snd.soundTime = g_GlobalVars->realtime;
		snd.alpha = 1.0f;
	}
}

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

Visuals::Visuals()
{
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}

extern bool CanSee[65];

//--------------------------------------------------------------------------------
void Visuals::Render() {
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	if (!pl->IsAlive())
		return false;

	if (pl->IsDormant() && flPlayerAlpha[pl->EntIndex()] > 0)
		flPlayerAlpha[pl->EntIndex()] -= 0.33f;
	else if (!pl->IsDormant())
		flPlayerAlpha[pl->EntIndex()] = 255;
	int alpha = (int)flPlayerAlpha[pl->EntIndex()];
	std::clamp(alpha, 0, 255);

	ctx.dormantfade = alpha;
	ctx.pl = pl;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_visible = CanSee[pl->EntIndex()];

	if (!ctx.is_visible && g_Options.esp_visible_only)
		return false;

	if (!ctx.is_enemy && g_Options.esp_enemy_only)
		return false;

	ctx.clr = ctx.is_visible ? Color(g_Options.color_esp_visible) : Color(g_Options.color_esp_occluded);
	if (ctx.dormantfade < 254)
		ctx.clr = Color(80, 80, 80, ctx.dormantfade);

	ctx.bbox = GetBBox(pl);

	if (!ctx.bbox.bottom || !ctx.bbox.right || !ctx.bbox.left || !ctx.bbox.top)
		return false;

	std::swap(ctx.bbox.top, ctx.bbox.bottom);
	return true;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox() {
	float edge_size;

	float
		length_horizontal = (ctx.bbox.right - ctx.bbox.left) * 0.2f,
		length_vertical = (ctx.bbox.bottom - ctx.bbox.top) * 0.2f;

	switch (g_Options.esp_player_box) {
	case 1:
		Render::Get().RenderBox(ctx.bbox, ctx.clr);
		Render::Get().RenderBox(ctx.bbox.left - 1.f, ctx.bbox.top - 1.f, ctx.bbox.right + 1.f, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBox(ctx.bbox.left + 1.f, ctx.bbox.top + 1.f, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, Color(0, 0, 0, ctx.clr.a()));
		break;

	case 2:
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.top - 1.f, ctx.bbox.left + 1.f + length_horizontal, ctx.bbox.top + 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 1.f - length_horizontal, ctx.bbox.top - 1.f, ctx.bbox.right + 1.f, ctx.bbox.top + 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.bottom - 2.f, ctx.bbox.left + 1.f + length_horizontal, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 1.f - length_horizontal, ctx.bbox.bottom - 2.f, ctx.bbox.right + 1.f, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));

		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.top + 2.f, ctx.bbox.left + 2.f, ctx.bbox.top + 1.f + length_vertical, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 2.f, ctx.bbox.top + 2.f, ctx.bbox.right + 1.f, ctx.bbox.top + 1.f + length_vertical, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.bottom - 1.f - length_vertical, ctx.bbox.left + 2.f, ctx.bbox.bottom - 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 2.f, ctx.bbox.bottom - 1.f - length_vertical, ctx.bbox.right + 1.f, ctx.bbox.bottom - 2.f, Color(0, 0, 0, ctx.clr.a()));


		Render::Get().RenderLine(float(ctx.bbox.left), float(ctx.bbox.top), ctx.bbox.left + length_horizontal - 1.f, float(ctx.bbox.top), ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - length_horizontal, float(ctx.bbox.top), ctx.bbox.right - 1.f, float(ctx.bbox.top), ctx.clr);
		Render::Get().RenderLine(float(ctx.bbox.left), ctx.bbox.bottom - 1.f, ctx.bbox.left + length_horizontal - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - length_horizontal, ctx.bbox.bottom - 1.f, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);

		Render::Get().RenderLine(float(ctx.bbox.left), float(ctx.bbox.top), float(ctx.bbox.left), ctx.bbox.top + length_vertical - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - 1.f, float(ctx.bbox.top), ctx.bbox.right - 1.f, ctx.bbox.top + length_vertical - 1.f, ctx.clr);
		Render::Get().RenderLine(float(ctx.bbox.left), ctx.bbox.bottom - length_vertical, float(ctx.bbox.left), ctx.bbox.bottom - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - 1.f, ctx.bbox.bottom - length_vertical, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);
		break;

	case 3:
		Render::Get().RenderBox(ctx.bbox, ctx.clr, 1.0f, 7.0f);
		//Render::Get().RenderBox(ctx.bbox, ctx.clr, 7.0f);
		break;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSkeleton()
{
	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(ctx.pl->GetModel());

	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];
		//if (ctx.pl->HandleBoneSetup(pBoneToWorldOut, 256, g_GlobalVars->curtime)) {
		if (ctx.pl->SetupBones(pBoneToWorldOut, 128, 256, g_GlobalVars->curtime)) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->GetBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
					continue;

				Render::Get().RenderLine(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, ctx.pl->IsDormant() ? ctx.clr : Color(g_Options.color_esp_skeleton));
			}
		}
	}
}

#define FLAG_MACRO std::pair<std::string, Color> // :joy:
#define FLAG(string, color) vecFlags.push_back(FLAG_MACRO(string, color)) 

void Visuals::Player::DrawFlags() /*Not the best way to do this, tbh*/
{
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
		return;
	std::vector<FLAG_MACRO> vecFlags;

	int alpha = ctx.clr.a(); // so now, this is epic
	if (g_Options.esp_flags_armor)
		if (ctx.pl->m_ArmorValue() > 0)
			FLAG(ctx.pl->GetArmorName(), Color(255, 128, 0, alpha));

	if (g_Options.esp_flags_scoped)
		if (ctx.pl->m_bIsScoped())
			FLAG("ZOOM", Color(52, 165, 207, alpha));

	if (g_Options.esp_flags_money)
		if (ctx.pl->m_iAccount())
			FLAG(std::string("$").append(std::to_string(ctx.pl->m_iAccount())), Color(0, 200, 0, alpha));

	if (g_Options.esp_flags_defusing)
		if (ctx.pl->m_bIsDefusing())
			FLAG("DEFUSING", Color(200, 0, 0, alpha));

	if (g_Options.esp_flags_c4)
		if (ctx.pl->HasC4())
			FLAG("C4", Color(200, 0, 0, alpha));

	if (g_Options.esp_flags_hostage)
		if (ctx.pl->m_hCarriedHostage() || ctx.pl->m_bIsGrabbingHostage())
			FLAG("HOSTAGE", ctx.pl->m_bIsGrabbingHostage() ? Color(200, 0, 0, alpha) : Color(52, 165, 207, alpha));

	if (g_Options.esp_flags_kit)
		if (ctx.pl->m_bHasDefuser())
			FLAG("KIT", Color(52, 165, 207, alpha));

	if (g_Options.esp_flags_flashed)
		if (ctx.pl->IsFlashed())
			FLAG("FLASHED", Color(200, 0, 0, alpha));

	int offset = 0; //smh, have to think about a better way just because of this lmao
	for (auto Text : vecFlags)
	{
		Render::Get().RenderText(Text.first, ctx.bbox.right + 4 + ((ctx.pl->m_ArmorValue() > 0 && g_Options.esp_player_armour) ? 5 : 0), ctx.bbox.top - 2 + offset, 14.f, Text.second, false, true);
		offset += 12;
	}
}

void Visuals::Player::RenderCustomSkeleton(matrix3x4_t* mat)
{
	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(ctx.pl->GetModel());

	if (pStudioModel) {
		if (mat) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->GetBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!Math::WorldToScreen(Vector(mat[i][0][3], mat[i][1][3], mat[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!Math::WorldToScreen(Vector(mat[pBone->parent][0][3], mat[pBone->parent][1][3], mat[pBone->parent][2][3]), vBonePos2))
					continue;

				Render::Get().RenderLine(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, ctx.pl->IsDormant() ? ctx.clr : Color(g_Options.color_esp_skeleton));
			}
		}
	}
}

//--------------------------------------------------------------------------------
void Visuals::PlayerChanger(ClientFrameStage_t stage)
{

	static int originalIdx = 0;

	auto pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (!pLocal) {
		originalIdx = 0;
		return;
	}

	auto getModel = [](int team)-> const char* {
		std::array models{
		("models/player/custom_player/legacy/ctm_fbi_variantb.mdl"),
		("models/player/custom_player/legacy/ctm_fbi_variantf.mdl"),
		("models/player/custom_player/legacy/ctm_fbi_variantg.mdl"),
		("models/player/custom_player/legacy/ctm_fbi_varianth.mdl"),
		("models/player/custom_player/legacy/ctm_sas_variantf.mdl"),
		("models/player/custom_player/legacy/ctm_st6_variante.mdl"),
		("models/player/custom_player/legacy/ctm_st6_variantg.mdl"),
		("models/player/custom_player/legacy/ctm_st6_varianti.mdl"),
		("models/player/custom_player/legacy/ctm_st6_variantk.mdl"),
		("models/player/custom_player/legacy/ctm_st6_variantm.mdl"),
		("models/player/custom_player/legacy/tm_balkan_variantf.mdl"),
		("models/player/custom_player/legacy/tm_balkan_variantg.mdl"),
		("models/player/custom_player/legacy/tm_balkan_varianth.mdl"),
		("models/player/custom_player/legacy/tm_balkan_varianti.mdl"),
		("models/player/custom_player/legacy/tm_balkan_variantj.mdl"),
		("models/player/custom_player/legacy/tm_leet_variantf.mdl"),
		("models/player/custom_player/legacy/tm_leet_variantg.mdl"),
		("models/player/custom_player/legacy/tm_leet_varianth.mdl"),
		("models/player/custom_player/legacy/tm_leet_varianti.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_variantf.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_variantg.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_varianth.mdl")
		};

		switch (team) {
		case 2: return static_cast<std::size_t>(g_Options.player_model_t - 1) < models.size() ? models[g_Options.player_model_t - 1] : nullptr;
		case 3: return static_cast<std::size_t>(g_Options.player_model_ct - 1) < models.size() ? models[g_Options.player_model_ct - 1] : nullptr;
		default: return nullptr;
		}
	};

	if (const auto model = getModel(pLocal->m_iTeamNum())) {
		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
			originalIdx = pLocal->m_nModelIndex();

		const auto idx = stage == ClientFrameStage_t::FRAME_RENDER_END && originalIdx ? originalIdx : g_MdlInfo->GetModelIndex(model);

		pLocal->SetModelIndex(idx);
	}
}

void GetVisFov() {

}

void DrawLine(float x1, float y1, float x2, float y2, Color color, float size)
{
	g_VGuiSurface->DrawSetColor(color);

	if (size == 1.f)
		g_VGuiSurface->DrawLine(x1, y1, x2, y2);
	else
		g_VGuiSurface->DrawFilledRect(x1 - (size / 2.f), y1 - (size / 2.f), x2 + (size / 2.f), y2 + (size / 2.f));
}

void DrawZeusRange()
{
	if (!g_EngineClient->IsConnected() || !g_EngineClient->IsInGame() || !g_Options.zeusrange)
		return;

	auto local_player = g_LocalPlayer;
	if (!local_player) return;
	if (local_player->m_iHealth() <= 0) return;

	C_BaseCombatWeapon* weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon) return;

	if (weapon->m_iItemDefinitionIndex() != WEAPON_TASER) return;

	float step = M_PI * 2.0 / 1023;
	float rad = 130.f;
	Vector origin = local_player->GetEyePos();

	static double rainbow;

	Vector screenPos;
	static Vector prevScreenPos;

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		CTraceFilter filter;
		CGameTrace trace;
		filter.pSkip = local_player;
		Ray_t t;
		t.Init(origin, pos);

		g_EngineTrace->TraceRay(t, MASK_SHOT_BRUSHONLY, &filter, &trace);

		if (g_DebugOverlay->ScreenPosition(trace.endpos, screenPos))
			continue;

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
			rainbow += 0.00001;
			if (rainbow > 1.f)
				rainbow = 0;
			Color color = Color::FromHSB(rainbow, 1.f, 1.f);
			DrawLine(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color, 1.f);
		}
		prevScreenPos = screenPos;
	}
}

//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
	player_info_t info = ctx.pl->GetPlayerInfo();

	auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, info.szName);

	Render::Get().RenderText(info.szName, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz.x * 0.5f, ctx.bbox.top - sz.y - 1, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color::White, false, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
	auto hp = ctx.pl->m_iHealth();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	float off = 7;

	int height = (box_h * hp) / 100;

	int green = int(hp * 2.55f);
	int red = 255 - green;

	int x = ctx.bbox.left - off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y - 1, x + w, y + h + 2, ctx.pl->IsDormant() ? ctx.clr : Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y, x + w - 1, y + height + 1, ctx.pl->IsDormant() ? Color(50, 50, 50, ctx.dormantfade) : Color(red, green, 0, 255), 1.f, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto armour = ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	float off = 4;

	int height = (((box_h * armour) / 100));

	int x = ctx.bbox.right + off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;
	if (armour > 1) {
		Render::Get().RenderBox(x, y, x + w, y + h, ctx.pl->IsDormant() ? ctx.clr : Color::Black, 1.0f, true);
		Render::Get().RenderBox(x + 1, y + 1, x + w - 1, y + height - 2, ctx.pl->IsDormant() ? ctx.clr : Color(0, 50, 255, 255), 1.f, true);
	}
}
auto GetGunIcon = [](short cw)
{
	switch (cw)
	{
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_T:
	case 500:
	case 505:
	case 506:
	case 507:
	case 508:
	case 509:
	case 512:
	case 514:
	case 515:
	case 516:
	case 519:
	case 520:
	case 522:
	case 523:
		return "]";
	case WEAPON_DEAGLE:
		return "A";
	case WEAPON_ELITE:
		return "B";
	case WEAPON_FIVESEVEN:
		return "C";
	case WEAPON_GLOCK:
		return "D";
	case WEAPON_HKP2000:
		return "E";
	case WEAPON_P250:
		return "F";
	case WEAPON_USP_SILENCER:
		return "G";
	case WEAPON_TEC9:
		return "H";
	case WEAPON_CZ75A:
		return "I";
	case WEAPON_REVOLVER:
		return "J";
	case WEAPON_MAC10:
		return "K";
	case WEAPON_UMP45:
		return "L";
	case WEAPON_BIZON:
		return "M";
	case WEAPON_MP7:
		return "N";
	case WEAPON_MP9:
		return "O";
	case WEAPON_P90:
		return "P";
	case WEAPON_GALILAR:
		return "Q";
	case WEAPON_FAMAS:
		return "R";
	case WEAPON_M4A1_SILENCER:
		return "T";
	case WEAPON_M4A1:
		return "S";
	case WEAPON_AUG:
		return "U";
	case WEAPON_SG556:
		return "V";
	case WEAPON_AK47:
		return "W";
	case WEAPON_G3SG1:
		return "X";
	case WEAPON_SCAR20:
		return "Y";
	case WEAPON_AWP:
		return "Z";
	case WEAPON_SSG08:
		return "a";
	case WEAPON_XM1014:
		return "b";
	case WEAPON_SAWEDOFF:
		return "c";
	case WEAPON_MAG7:
		return "d";
	case WEAPON_NOVA:
		return "e";
	case WEAPON_NEGEV:
		return "f";
	case WEAPON_M249:
		return "g";
	case WEAPON_TASER:
		return "h";
	case WEAPON_FLASHBANG:
		return "i";
	case WEAPON_HEGRENADE:
		return "j";
	case WEAPON_SMOKEGRENADE:
		return "k";
	case WEAPON_MOLOTOV:
		return "l";
	case WEAPON_DECOY:
		return "m";
	case WEAPON_INCGRENADE:
		return "n";
	case WEAPON_C4:
		return "o";
	default:
		return "";
	}
};

//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeaponName()
{

	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};
	auto weapon = ctx.pl->m_hActiveWeapon().Get();

	if (!weapon || !weapon->GetCSWeaponData()) return;


	//size_t len = wcstombs(text, text1, wcslen(text1));
	//if (len > 0u)
	//	text[len] = '\0';

	std::string ammo = string_format("[%i/%i]", weapon->m_iPrimaryReserveAmmoCount(), weapon->m_iClip1());
	auto sz2 = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, ammo.c_str());


	if (g_Options.esp_weapon_icon)
	{
		auto text = std::string(GetGunIcon(weapon->m_Item().m_iItemDefinitionIndex()));
		auto sz = g_pIconFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, text.c_str());
		Render::Get().RenderText(text, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz.x * 0.5f, ctx.bbox.bottom + 1.f, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color::White, false, true, g_pIconFont);

	}
	else
	{
		auto text = clean_item_name(weapon->GetClientClass()->m_pNetworkName);
		auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, text);
		Render::Get().RenderText(text, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz.x * 0.5f, ctx.bbox.bottom + 1.f, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color::White, false, true);

	}
	if (g_Options.show_ammo)
		Render::Get().RenderText(ammo, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz2.x * 0.5f, ctx.bbox.bottom + 1.f + sz2.y, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color::White, false, true);
}
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent)
{
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	// We don't want to Render weapons that are being held
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	if (g_Options.esp_dropped_weapons_box)
		Render::Get().RenderBox(bbox, Color(g_Options.color_esp_weapons));

	int offset = 3;


	if (g_Options.esp_droppped_ammo_bar)
	{
		if (ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_FLASHBANG &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_SMOKEGRENADE &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_MOLOTOV &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_INCGRENADE &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_DECOY &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_HEGRENADE &&
			ent->get_item_definition_index() != ItemDefinitionIndex::WEAPON_C4)
		{
			float box_w = (float)fabs(bbox.right - bbox.left);

			auto width = (((box_w * ent->m_iClip1()) / ent->GetCSWeaponData()->iMaxClip1));

			Render::Get().RenderBox(bbox.left - 1, bbox.top + 3, bbox.right + 1, bbox.top + 7, Color(0, 0, 0, 255));

			Render::Get().RenderBoxFilled(bbox.left, bbox.top + 4, bbox.right, bbox.top + 6, Color(0, 0, 0, 255));

			Render::Get().RenderBoxFilled(bbox.left, bbox.top + 4, bbox.left + (int)width, bbox.top + 6, Color::White);
			if (ent->m_iClip1() != ent->GetCSWeaponData()->iMaxClip1 && ent->m_iClip1() > 0)
			{
				Render::Get().RenderText(std::to_string(ent->m_iClip1()), ImVec2(bbox.left + (int)width, bbox.top + 2), 12.f, Color(255, 255, 255, 255), false, true);
				offset += 5;
			}
			offset += 4;
		}
	}


	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);
	if (!name)
		return;

	auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1 + offset), 14.f, Color(g_Options.color_esp_weapons), false, true);
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent)
{

	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;


	auto name = "Defuse Kit";
	auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;

	/*if (g_Options.esp_dropped_defusekits_box)
		Render::Get().RenderBox(bbox, Color(g_Options.color_esp_defusekit));*/

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color(g_Options.color_esp_defusekit), false, true);
}
//--------------------------------------------------------------------------------
RECT GetViewport()
{
	RECT viewport = { 0, 0, 0, 0 };
	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	viewport.right = w; viewport.bottom = h;

	return viewport;
}
void Visuals::RenderPlantedC4(C_BaseEntity* entity)
{
	RECT scrn = GetViewport();

	char text[64];
	std::string text2;
	float TimeRemaining = entity->m_flC4Blow() - g_GlobalVars->curtime;
	int tw2, th2;

	auto bbox = GetBBox(entity);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;


	auto name = "C4";
	auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;

	if (g_Options.esp_dropped_c4_box)
		Render::Get().RenderBox(bbox, Color(g_Options.color_esp_c4));

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color(g_Options.color_esp_c4), false, true);

	std::string buf3;
	if (entity->m_nBombSite() == 0)
		buf3 = "A";
	else if (entity->m_nBombSite() == 1)
		buf3 = "B";
	else
		buf3 = "C";

	Color time;

	if (TimeRemaining > 0) {
		if (TimeRemaining <= 10 && TimeRemaining >= 5)
			time = Color(215, 200, 0);
		else if (TimeRemaining <= 5)
			time = Color(200, 0, 0);
		else
			time = Color(73, 200, 0);
	}

	C_BasePlayer* mainent = g_LocalPlayer->IsAlive() ? g_LocalPlayer : g_LocalPlayer->m_hObserverTarget().Get();

	if (!mainent)
		return;

	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ((mainent->GetEyePos().DistTo(entity->m_vecOrigin()) - b) / c);
	float flDamage = a * expf(-d * d);

	int damage = (int)std::fmaxf(ceilf(GetArmourHealth(flDamage, mainent->m_ArmorValue())), 0.f);

	Color damagecol;

	if (damage > 0) {
		if (damage > 75)
			damagecol = Color(200, 0, 0);
		else if (damage > 40)
			damagecol = Color(215, 200, 0);
		else
			damagecol = Color(73, 200, 0);
	}

	if (TimeRemaining > 0.f) {
		sprintf(text, "%s - %.1f sec", buf3.c_str(), TimeRemaining);
		if (damage < mainent->m_iHealth())
			text2 = string_format("-%iHP", damage);
		else
			text2 = string_format("%s", "FATAL");

		if (text && bombisready && damage)
		{

			Render::Get().RenderText(text, scrn.left + 6, scrn.bottom / 2, 24.f, time, false, true, g_pSecondFont);

			Render::Get().RenderText(text2, scrn.left + 6, scrn.bottom / 2 + 20, 24.f, damagecol, false, true, g_pSecondFont);

		}

	}
}
//--------------------------------------------------------------------------------
void Visuals::RenderItemEsp(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;
	std::string itemstr = "Undefined";
	const model_t* itemModel = ent->GetModel();
	if (!itemModel)
		return;
	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(itemModel);
	if (!hdr)
		return;
	itemstr = hdr->szName;
	if (ent->GetClientClass()->m_ClassID == ClassId_CBumpMine)
		itemstr = "";
	else if (itemstr.find("case_pistol") != std::string::npos)
		itemstr = "Pistol Case";
	else if (itemstr.find("case_light_weapon") != std::string::npos)
		itemstr = "Light Case";
	else if (itemstr.find("case_heavy_weapon") != std::string::npos)
		itemstr = "Heavy Case";
	else if (itemstr.find("case_explosive") != std::string::npos)
		itemstr = "Explosive Case";
	else if (itemstr.find("case_tools") != std::string::npos)
		itemstr = "Tools Case";
	else if (itemstr.find("random") != std::string::npos)
		itemstr = "Airdrop";
	else if (itemstr.find("dz_armor_helmet") != std::string::npos)
		itemstr = "Full Armor";
	else if (itemstr.find("dz_helmet") != std::string::npos)
		itemstr = "Helmet";
	else if (itemstr.find("dz_armor") != std::string::npos)
		itemstr = "Armor";
	else if (itemstr.find("upgrade_tablet") != std::string::npos)
		itemstr = "Tablet Upgrade";
	else if (itemstr.find("briefcase") != std::string::npos)
		itemstr = "Briefcase";
	else if (itemstr.find("parachutepack") != std::string::npos)
		itemstr = "Parachute";
	else if (itemstr.find("dufflebag") != std::string::npos)
		itemstr = "Cash Dufflebag";
	else if (itemstr.find("ammobox") != std::string::npos)
		itemstr = "Ammobox";
	else if (itemstr.find("dronegun") != std::string::npos)
		itemstr = "Turrel";
	else if (itemstr.find("exojump") != std::string::npos)
		itemstr = "Exojump";
	else if (itemstr.find("healthshot") != std::string::npos)
		itemstr = "Healthshot";
	else {
		/*May be you will search some missing items..*/
		/*static std::vector<std::string> unk_loot;
		if (std::find(unk_loot.begin(), unk_loot.end(), itemstr) == unk_loot.end()) {
			Utils::ConsolePrint(itemstr.c_str());
			unk_loot.push_back(itemstr);
		}*/
		return;
	}

	if (ent->m_vecOrigin().DistTo(g_LocalPlayer->GetEyePos()) > g_Options.esp_distance_dz)
		return;
	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	auto sz = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, itemstr.c_str());
	int w = bbox.right - bbox.left;

	Render::Get().RenderText(itemstr, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color::White, false, true);
}
//--------------------------------------------------------------------------------
void Visuals::ThirdPerson() {
	if (!g_LocalPlayer)
		return;

	static size_t lastTime = 0;

	if (InputSys::Get().IsKeyDown(g_Options.misc_thirdperson_bind))
	{
		if (GetTickCount64() > lastTime) {
			g_Options.misc_thirdperson = !g_Options.misc_thirdperson;

			lastTime = GetTickCount64() + 650;
		}
	}

	if (g_Options.misc_thirdperson && g_LocalPlayer->IsAlive())
		g_Input->m_fCameraInThirdPerson = true;
	else
		g_Input->m_fCameraInThirdPerson = false;

	auto GetCorrectDistance = [](float ideal_distance) -> float
	{
		/* vector for the inverse angles */
		QAngle inverseAngles;
		g_EngineClient->GetViewAngles(&inverseAngles);

		/* inverse angles by 180 */
		inverseAngles.pitch *= -1.f, inverseAngles.yaw += 180.f;

		/* vector for direction */
		Vector direction;
		Math::AngleVectors(inverseAngles, direction);

		/* ray, trace & filters */
		Ray_t ray;
		trace_t trace;
		CTraceFilter filter;

		/* dont trace local player */
		filter.pSkip = g_LocalPlayer;

		/* create ray */
		ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + (direction * ideal_distance));

		/* trace ray */
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

		/* return the ideal distance */
		return (ideal_distance * trace.fraction) - 10.f;
	};

	QAngle angles;
	g_EngineClient->GetViewAngles(&angles);
	angles.roll = GetCorrectDistance(g_Options.misc_thirdperson_dist); // 150 is better distance
	g_Input->m_vecCameraOffset = Vector(angles.pitch, angles.yaw, angles.roll);
}

void DrawAngles(C_BasePlayer* pl) {
	auto drawAngleLine = [&](const Vector& origin, const Vector& w2sOrigin, const float& angle, const char* text, Color clr) {
		Vector forward;
		Math::AngleVectors(QAngle(0.0f, angle, 0.0f), forward);
		float AngleLinesLength = 30.0f;

		Vector w2sReal;
		if (Math::WorldToScreen(origin + forward * AngleLinesLength, w2sReal)) {
			Render::Get().RenderLine(w2sOrigin.x, w2sOrigin.y, w2sReal.x, w2sReal.y, Color::White, 1.0f);
			Render::Get().RenderText(text, w2sReal.x, w2sReal.y - 5.0f, 14.f, clr, true, true);

		}
	};

	if (!pl || !pl->GetPlayerAnimState())
		return;

	if (!pl->IsAlive())
		return;

	if (g_Options.esp_angle_lines) {
		Vector w2sOrigin;
		extern float anglereal;
		extern float anglefake;
		if (Math::WorldToScreen(pl->GetAbsOrigin(), w2sOrigin)) {
			static float view;
			if (Globals::bSendPacket)
				view = Globals::cmd->viewangles.yaw;
			drawAngleLine(pl->GetAbsOrigin(), w2sOrigin, pl->GetPlayerAnimState()->m_flGoalFeetYaw, "fake", Color(0.937f, 0.713f, 0.094f, 1.0f));
			drawAngleLine(pl->GetAbsOrigin(), w2sOrigin, pl->m_flLowerBodyYawTarget(), "lby", Color(0.0f, 0.0f, 1.0f, 1.0f));
			drawAngleLine(pl->GetAbsOrigin(), w2sOrigin, anglereal, "real", Color(0.0f, 1.0f, 0.0f, 1.0f));
			drawAngleLine(pl->GetAbsOrigin(), w2sOrigin, view, "view", Color(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

#include "damageindicator.h"
#include "legitbot/backtrack.hpp"

void Visuals::AddToDrawList() {


	draw_grenades();


	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (!g_LocalPlayer)
			continue;

		if (entity == g_LocalPlayer && !g_Input->m_fCameraInThirdPerson)
			continue;

		if (g_LocalPlayer->m_hObserverTarget().Get() == entity)
			continue;

		if (!g_Options.esp_enabled)
			continue;


		//	if(entity->IsPlayer())
				//if (g_Options.esp_visible_only && !g_LocalPlayer->CanSeePlayer((C_BasePlayer*)entity, HITBOX_CHEST, tu))
				//	continue;
		if (g_Options.esp_defuse_kit && entity->IsDefuseKit())
			RenderDefuseKit(entity);
		if (g_Options.esp_dropped_weapons && entity->IsWeapon())
			RenderWeapon(static_cast<C_BaseCombatWeapon*>(entity));
		if (entity->IsPlantedC4() && g_Options.esp_planted_c4)
			RenderPlantedC4(entity);
		if (entity->IsLoot() && g_Options.esp_items)
			RenderItemEsp(entity);
		if (entity->IsPlayer()) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {

				if (g_Options.esp_player_box)		player.RenderBox();
				if (g_Options.esp_player_skeleton)	player.RenderSkeleton();
				if (g_Options.esp_player_weapons)   player.RenderWeaponName();
				if (g_Options.esp_player_names)     player.RenderName();
				if (g_Options.esp_player_health)    player.RenderHealth();
				if (g_Options.esp_player_armour)    player.RenderArmour();
				player.DrawFlags();

				if (g_Options.esp_player_skeleton_bt && g_Backtrack.data.count(player.ctx.pl->EntIndex()) > 0) {
					auto& data = g_Backtrack.data.at(player.ctx.pl->EntIndex());
					if (data.size() > 0) {

						auto& back = data.back();
						player.RenderCustomSkeleton(back.boneMatrix);
					}
				}
			}



		}

	}

	if (g_Options.esp_angle_lines)
		DrawAngles(g_LocalPlayer);

	if (g_Options.custommodelson)
	{

		for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
		{
			auto entity = C_BaseEntity::GetEntityByIndex(i);
			auto entity1 = C_BasePlayer::GetEntityByIndex(i);
			auto* local = g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
			player_info_t pinfo;
			if (entity == nullptr)
				continue;
			if (entity == local)
				continue;
			if (entity->IsDormant())
				continue;
			if (g_EngineClient->GetPlayerInfo(i, &pinfo))
			{
				custommodels(entity);
			}
		}

	}

	if (g_Options.other_drawfov)
		DrawFOV();

	if (g_Options.zeusrange)
		DrawZeusRange();

	if (g_Options.recoilCrosshair)
		RecoilCrosshair();

	if (g_Options.esp_damageindicator)
		damage_indicators.paint();

	ConVar* cl_phys_timescale = g_CVar->FindVar("cl_phys_timescale");
	if (g_Options.cl_phys_timescale)
		cl_phys_timescale->SetValue(g_Options.cl_phys_timescale_value);
	else cl_phys_timescale->SetValue("1.0");

	if (g_Options.esp_sounds)
		RenderSounds();
#ifndef _DEBUG
	if (g_Options.esp_grenade_prediction)
		GrenadePrediction::Get().Paint(nullptr);
#endif
}

void Visuals::DrawFOV() {
	auto pWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!pWeapon)
		return;

	aimbot_settings settings;

	if (settings.enabled) {

		float fov = static_cast<float>(g_LocalPlayer->GetFOV());

		int w, h;
		g_EngineClient->GetScreenSize(w, h);



		int x = w / 2;
		int y = h / 2;
		int dy = h / 97;
		int dx = w / 97;

		QAngle punchAngle = g_LocalPlayer->m_aimPunchAngle() * (g_CVar->FindVar("weapon_recoil_scale")->GetFloat() / 2);
		x -= (dx * (punchAngle.yaw));
		y += (dy * (punchAngle.pitch));


		Vector2D screenSize = Vector2D(w, h);
		Vector2D center = screenSize * 0.5f;

		float ratio = screenSize.x / screenSize.y;
		float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));
		QAngle shit;
		g_EngineClient->GetViewAngles(&shit);
		float radiusFOV;
		if (settings.fov_type == 1)
		{
			Vector src3D, dst3D, forward;
			trace_t tr;
			Ray_t ray;
			CTraceFilter filter;

			QAngle angles = shit;
			Math::AngleVectors(angles, forward);
			filter.pSkip = g_LocalPlayer;
			src3D = g_LocalPlayer->GetEyePos();
			dst3D = src3D + (forward * 8192);

			ray.Init(src3D, dst3D);
			g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

			QAngle leftViewAngles = QAngle(angles.pitch, angles.yaw - 90.f, 0.f);
			Math::Normalize3(leftViewAngles);
			Math::AngleVectors(leftViewAngles, forward);
			forward *= settings.fov * 5.f;

			Vector maxAimAt = tr.endpos + forward;

			Vector max2D;
			if (g_DebugOverlay->ScreenPosition(maxAimAt, max2D))
				return;
			radiusFOV = fabsf(x - max2D.x);


		}
		else
			radiusFOV = tanf(DEG2RAD(g_Aimbot.GetFov())) / tanf(screenFov) * center.x;



		Render::Get().RenderCircle(x, y, radiusFOV, 128, Color(0, 0, 0, 170));

		if (settings.silent) {
			float silentRadiusFOV = tanf(DEG2RAD(settings.silent_fov)) / tanf(screenFov) * center.x;
			Render::Get().RenderCircle(x, y, silentRadiusFOV, 128, Color(255, 25, 10, 170));
		}


	}
}

bool PrecacheModel(const char* szModelName)
{
	auto m_pModelPrecacheTable = g_ClientStringTableContainer->FindTable("modelprecache");

	if (m_pModelPrecacheTable)
	{
		g_MdlInfo->FindOrLoadModel(szModelName);
		int idx = m_pModelPrecacheTable->AddString(false, szModelName);
		if (idx == INVALID_STRING_INDEX)
			return false;
	}
	return true;
}

void Visuals::custommodels(C_BaseEntity* entity)
{


	C_BaseEntity* local = (C_BaseEntity*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());



	if (entity->m_iTeamNum() == TEAM_CT & g_EngineClient->IsInGame())
	{
		switch (g_Options.customodelsct)
		{
		case 0:
			break;

		case 1:
			PrecacheModel("models/player/custom_player/kuristaja/christmas_miku/miku.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/christmas_miku/miku.mdl"));
			break;

		case 2:
			PrecacheModel("models/player/custom_player/kuristaja/harry_potter/harry.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/harry_potter/harry.mdl"));
			break;


		case 3:
			PrecacheModel("models/player/custom_player/kuristaja/picolas_cage/picolas.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/picolas_cage/picolas.mdl"));
			break;
		case 4:
			PrecacheModel("models/player/custom_player/kuristaja/santa/santa.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/santa/santa.mdl"));
			break;
		case 5:
			PrecacheModel("models/player/custom_player/voikanaa/hitman/agent47.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/voikanaa/hitman/agent47.mdl"));
			break;
		case 6:
			PrecacheModel("models/player/custom_player/kuristaja/ak/batman/batmanv2.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/ak/batman/batmanv2.mdl"));
			break;
		case 7:
			PrecacheModel("models/player/custom_player/voikanaa/acb/ezio.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/voikanaa/acb/ezio.mdl"));
			break;
		case 8:
			PrecacheModel("models/player/custom_player/kuristaja/ash/ash.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/ash/ash.mdl"));
			break;
		case 9:
			PrecacheModel("models/player/custom_player/kuristaja/doomguy/doomguy.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/doomguy/doomguy.mdl"));
			break;
		case 10:
			PrecacheModel("models/player/custom_player/kuristaja/putin/putin.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/putin/putin.mdl"));
			break;
		case 11:
			PrecacheModel("models/player/custom_player/kuristaja/skeleton/skeleton.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/skeleton/skeleton.mdl"));
			break;
		case 12:
			PrecacheModel("models/player/custom_player/kuristaja/slimer/slimer.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/slimer/slimer.mdl"));
			break;
		case 13:
			PrecacheModel("models/player/custom_player/kuristaja/spiderman/spiderman.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/spiderman/spiderman.mdl"));
			break;


		}
	}

	else if (entity->m_iTeamNum() == TEAM_TT & g_EngineClient->IsInGame())
	{
		switch (g_Options.customodelst)
		{
		case 0:
			break;

		case 1:
			PrecacheModel("models/player/custom_player/kuristaja/christmas_miku/miku.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/christmas_miku/miku.mdl"));
			break;

		case 2:
			PrecacheModel("models/player/custom_player/kuristaja/harry_potter/harry.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/harry_potter/harry.mdl"));
			break;


		case 3:
			PrecacheModel("models/player/custom_player/kuristaja/picolas_cage/picolas.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/picolas_cage/picolas.mdl"));
			break;
		case 4:
			PrecacheModel("models/player/custom_player/kuristaja/santa/santa.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/santa/santa.mdl"));
			break;
		case 5:
			PrecacheModel("models/player/custom_player/voikanaa/hitman/agent47.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/voikanaa/hitman/agent47.mdl"));
			break;
		case 6:
			PrecacheModel("models/player/custom_player/kuristaja/ak/batman/batmanv2.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/ak/batman/batmanv2.mdl"));
			break;
		case 7:
			PrecacheModel("models/player/custom_player/voikanaa/acb/ezio.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/voikanaa/acb/ezio.mdl"));
			break;
		case 8:
			PrecacheModel("models/player/custom_player/kuristaja/ash/ash.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/ash/ash.mdl"));
			break;
		case 9:
			PrecacheModel("models/player/custom_player/kuristaja/doomguy/doomguy.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/doomguy/doomguy.mdl"));
			break;
		case 10:
			PrecacheModel("models/player/custom_player/kuristaja/putin/putin.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/putin/putin.mdl"));
			break;
		case 11:
			PrecacheModel("models/player/custom_player/kuristaja/skeleton/skeleton.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/skeleton/skeleton.mdl"));
			break;
		case 12:
			PrecacheModel("models/player/custom_player/kuristaja/slimer/slimer.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/slimer/slimer.mdl"));
			break;
		case 13:
			PrecacheModel("models/player/custom_player/kuristaja/spiderman/spiderman.mdl");
			entity->ModelChangerIndex(g_MdlInfo->GetModelIndex("models/player/custom_player/kuristaja/spiderman/spiderman.mdl"));
			break;
		}
	}

}