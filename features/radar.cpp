#include "radar.hpp"
#include "..//options.hpp"
#include <d3dx9tex.h>

std::string Radar::parseString(std::string szBefore, std::string szSource)
{
	if (!szBefore.empty() && !szSource.empty() && (szSource.find(szBefore) != std::string::npos))
	{
		std::string t = strstr(szSource.c_str(), szBefore.c_str());
		t.erase(0, szBefore.length());
		size_t firstLoc = t.find("\"", 0);
		size_t secondLoc = t.find("\"", firstLoc + 1);
		t = t.substr(firstLoc + 1, secondLoc - 3);
		return t;
	}
	else
		return "";
}

Vector Radar::WorldToMap(Vector vWorldPosition)
{
	const int iMapCenter = RADAR_MAP_SIZE / 2;
	float range = ((float)g_Options.radar.range) / 1000.f;
	// World2Map
	Vector vMapPosition;
	vMapPosition.x = (vWorldPosition.x - m_vMapOrigin.x) / m_flMapScale;
	vMapPosition.y = (vWorldPosition.y - m_vMapOrigin.y) / -m_flMapScale;

	// Map2Panel
	Vector vOffset;
	vOffset.x = vMapPosition.x - iMapCenter;
	vOffset.y = vMapPosition.y - iMapCenter;

	// Scale // TODO: Is it right? ((m_fZoom * m_fFullZoom) / OVERVIEW_MAP_SIZE)
	float fScale = range / RADAR_MAP_SIZE;
	vOffset.x *= fScale;
	vOffset.y *= fScale;

	Vector vFinalPos;
	vFinalPos.x = (m_flMapSize * 0.5f) + (m_flMapSize * vOffset.x);
	vFinalPos.y = (m_flMapSize * 0.5f) + (m_flMapSize * vOffset.y);
	vFinalPos.z = 0;

	return vFinalPos;
}

bool Radar::OnMapLoad(const char* pszMapName)
{
	m_bLoaded = false;

	char szPath[256] = "\0";
	sprintf_s(szPath, R"(csgo\resource\overviews\%s_radar.dds)", pszMapName);
	if (FAILED(D3DXCreateTextureFromFileA(g_D3DDevice9, szPath, &RadarMapImage)))
	{
		return false;
	}

	sprintf_s(szPath, R"(.\csgo\resource\overviews\%s.txt)", pszMapName);
	std::ifstream ifs(szPath);
	if (ifs.bad())
	{
		return false;
	}
	const std::string szInfo((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	if (szInfo.empty())
	{
		return false;
	}
	m_vMapOrigin.x = std::stoi(parseString("\"pos_x\"", szInfo));
	m_vMapOrigin.y = std::stoi(parseString("\"pos_y\"", szInfo));
	m_flMapScale = std::stod(parseString("\"scale\"", szInfo));

	m_bLoaded = true;

	return true;
}

bool Radar::RotatePoint(ImVec2& p)
{
	QAngle vAngle;

	g_EngineClient->GetViewAngles(&vAngle);
	const float yaw = DEG2RAD(vAngle.yaw - 90.f);
	const float s = sin(yaw);
	const float c = cos(yaw);
	const float cs = (m_flMapSize / 2);
	const float cx = m_flMapX + cs;
	const float cy = m_flMapY + cs;

	// translate point back to origin
	p.x -= cx;
	p.y -= cy;

	// rotate point
	const float xnew = p.x * c - p.y * s;
	const float ynew = p.x * s + p.y * c;

	// translate point back
	p.x = xnew + cx;
	p.y = ynew + cy;

	return true;
}

bool Radar::RenderMap(float screenx, float screeny)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	const ImVec2 DrawSize = ImGui::GetWindowSize();

	m_flMapX = screenx;
	m_flMapY = screeny;
	m_flMapSize = DrawSize.y;
	m_flMapSize = DrawSize.x;

	// Vars
	int x = m_flMapX;
	int y = m_flMapY;
	int size = m_flMapSize;
	const int center_size = (size / 2);

	// Scale map
	const float range = ((float)g_Options.radar.range) / 1000.f;

	const float new_size = size * range;
	const float size_diff = new_size - size;
	size = new_size;
	x -= size_diff / 2;
	y -= size_diff / 2;

	// Locate map by our local player
	if (g_LocalPlayer)
	{
		const Vector world_origin = g_LocalPlayer->m_vecOrigin();
		const Vector map_origin = WorldToMap(world_origin);
		x += (center_size - map_origin.x);
		y += (center_size - map_origin.y);
	}

	// Prepare imgui shit
	ImVec2 uv_a(0, 0), uv_c(1, 1), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
	ImVec2 a(x, y), c(x + size, y + size), b(c.x, a.y), d(a.x, c.y);

	// Rotate

	RotatePoint(a);
	RotatePoint(b);
	RotatePoint(c);
	RotatePoint(d);

	Draw->PushTextureID(RadarMapImage);
	Draw->PrimReserve(6, 4);
	Draw->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, 0xFFFFFFFF);
	Draw->PopTextureID();

	return true;
}

void Radar::CalcRadarPoint(Vector vOrigin, float& screenx, float& screeny, bool ChechOut)
{
	QAngle vAngle;

	g_EngineClient->GetViewAngles(&vAngle);

	float range = ((float)g_Options.radar.range) / 1000.f;

	ImVec2 DrawPos = ImGui::GetWindowPos();
	ImVec2 DrawSize = ImGui::GetWindowSize();

	float r_1, r_2;
	float x_1, y_1;

	Vector vEyeOrigin = g_LocalPlayer->GetEyePos();

	r_1 = -(vOrigin.y - vEyeOrigin.y);
	r_2 = vOrigin.x - vEyeOrigin.x;
	float Yaw = vAngle.yaw - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20.f;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20.f;

	x_1 *= range;
	y_1 *= range;

	float sizX = DrawSize.x / 2;
	float sizY = DrawSize.y / 2;

	x_1 += sizX;
	y_1 += sizY;

	x_1 += DrawPos.x;
	y_1 += DrawPos.y;

	screenx = x_1;
	screeny = y_1;

}

void Radar::OnRenderPlayer()
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();

	for (int i = 0; i <= g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pPlayer = C_BasePlayer::GetPlayerByIndex(i);

		if (pPlayer && pPlayer->IsPlayer())
		{

			if (pPlayer == g_LocalPlayer)
				continue;

			if (pPlayer->IsDormant())
				continue;

			if (!pPlayer->IsAlive())
				continue;

			bool is_enemy = pPlayer->IsEnemy();

			if (is_enemy && !g_Options.radar.enemies)
				continue;

			if (!is_enemy && !g_Options.radar.teammates)
				continue;

			float screenx = 0;
			float screeny = 0;

			CalcRadarPoint(pPlayer->m_vecOrigin(), screenx, screeny);

			Color clr = pPlayer->IsEnemy() ? g_Options.colors.radar.enemy : g_Options.colors.radar.ally;

			Draw->AddCircleFilled(ImVec2(screenx, screeny), g_Options.radar.points_size, ImColor(clr.r(), clr.g(), clr.b(), clr.a()));
		}
	}
}

void Radar::OnRender()
{
	if (g_Options.radar.ingame && g_EngineClient->IsInGame())
	{
		for (auto i = 1; i <= g_EngineClient->GetMaxClients(); ++i)
		{
			C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
			if (!entity)
				continue;

			if (entity == g_LocalPlayer)
				continue;

			const bool is_enemy = entity->IsEnemy();

			if (is_enemy && !g_Options.radar.enemies)
				continue;

			if (!is_enemy && !g_Options.radar.teammates)
				continue;

			if (!entity->m_bSpotted())
				entity->m_bSpotted() = true;
		}
	}

	if (g_Options.radar.enabled)
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
		ImGui::SetNextWindowSize(ImVec2(g_Options.radar.size, g_Options.radar.size));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Radar", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		{
			ImDrawList* Draw = ImGui::GetWindowDrawList();

			const ImVec2 DrawPos = ImGui::GetCursorScreenPos();

			static std::string mapname;
			if (mapname != g_EngineClient->GetLevelNameShort())
			{
				mapname = g_EngineClient->GetLevelNameShort();
				OnMapLoad(mapname.c_str());
			}

			if (g_Options.radar.textured && g_EngineClient->IsInGame() && m_bLoaded)
				RenderMap(DrawPos.x, DrawPos.y);

			if (g_EngineClient->IsInGame() && g_LocalPlayer)
				OnRenderPlayer();
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
}