#pragma once

#include "../helpers/Singleton.hpp"

#include <map>
#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

struct molotov_info_t
{
	Vector position;
	float time_to_expire;
};

struct smoke_info_t
{
	Vector position;
	float time_to_expire;
};

class bomb_planted_event : public IGameEventListener2
{
public:
	~bomb_planted_event() { g_GameEvents->RemoveListener(this); }

	virtual void FireGameEvent(IGameEvent* p_event);
};

class round_end_event : public IGameEventListener2
{
public:
	~round_end_event() { g_GameEvents->RemoveListener(this); }

	virtual void FireGameEvent(IGameEvent* p_event);
};

struct SoundInfo_t {
	int guid;
	float soundTime;
	float alpha;
	Vector soundPos;
};

class VisualsRender : public Singleton<VisualsRender>
{
	friend class Singleton<VisualsRender>;
private:
public:
	vgui::HFont VisualsFont;
	vgui::HFont EspFont;
	vgui::HFont watermark_ui_font;
	vgui::HFont small_pixel_11;
	vgui::HFont verdanaBold_12;

	void CreateFonts();
	void Text(int X, int Y, const char* Text, vgui::HFont Font, Color DrawColor, bool Center);
	void TextSize(int& Width, int& Height, const char* Text, vgui::HFont Font);
	void FilledRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void OutlinedRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void Line(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void Circle(int x, int y, int r, int seg, Color color);
};

class Visuals : public Singleton<Visuals>
{
	float flHurtTime;
	int kills = 0;
	float killtime = 0.0f;
	int globalAlpha = 0;
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	Visuals();
	~Visuals();
private:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			int          dormantfade;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		} ctx;
		void DrawFlags();
		void RenderCustomSkeleton(matrix3x4_t* mat);
		bool Begin(C_BasePlayer * pl);
		void RenderBox();
		void RenderName();
		void RenderWeaponName();
		void RenderSkeleton();
		void RenderHealth();
		void RenderArmour();
		void debug_visuals();
	};

	std::vector<molotov_info_t> molotov_info;
	std::vector<smoke_info_t> smoke_info;
	std::map< int, std::vector< SoundInfo_t > > m_Sounds;

	CUtlVector<SndInfo_t> sounds;

	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void RenderItemEsp(C_BaseEntity* ent);
	void RenderSounds();
	void UpdateSounds();
	void DrawString(unsigned long font, int x, int y, Color color, unsigned long alignment, const char* msg, ...);
public:
	void AddToDrawList();
	void DrawFOV();
	virtual void FireGameEvent(IGameEvent* event);
	void custommodels(C_BaseEntity* entity);
	void QuakeKillCounter();
	void AsusWalls();
	void draw_granades(C_BaseEntity* entity);
	void draw_grenades();
	void events(IGameEvent* event);
	void Listener();
	void RecoilCrosshair();
	void ThirdPerson();
	void RenderSpectatorList();
	void Render();
	void PlayerChanger(ClientFrameStage_t stage);
};