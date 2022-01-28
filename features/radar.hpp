#pragma once

#include "../valve_sdk/sdk.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../menu.hpp"
#include <d3d9.h>
#include <fstream>
#include <map>
#include <vector>
#include <string_view>
#include <memory>

#define RADAR_MAP_SIZE 1024
#define RADAR_ICON_SIZE 15
#define PI 3.14159265

class Radar
{
public:
	Radar() {}

	void OnRender();
	bool OnMapLoad(const char * pszMapName);
private:
	Vector m_vMapOrigin = Vector();
	float m_flMapScale = NULL;
	int m_iScreenWidth = NULL, m_iScreenHeight = NULL;
	float m_flMapX = NULL;
	float m_flMapY = NULL;
	float m_flMapSize = NULL;
	bool m_bLoaded = false;
	IDirect3DTexture9 *RadarMapImage = nullptr;
	void OnRenderPlayer();
	std::string parseString(std::string szBefore, std::string szSource);
	Vector WorldToMap(Vector vWorldPosition);
	void CalcRadarPoint(Vector vOrigin, float & screenx, float & screeny, bool ChechOut = true);
	bool RenderMap(float screenx, float screeny);
	bool RotatePoint(ImVec2 & p);
};

inline Radar* g_Radar;