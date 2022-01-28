#pragma once
#include <set>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include <limits>
#include "valve_sdk/Misc/Color.hpp"
#define A( s ) #s
#define OPTION(type, var, val) type var = val

struct item_setting
{
	char name[32] = "Default";
	bool enabled = false;
	int stickers_place = 0;
	int definition_vector_index = 0;
	int definition_index = 0;
	bool   enabled_stickers = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	bool stat_trak = 0;
	float wear = FLT_MIN;
	char custom_name[32] = "";
};

struct statrack_setting
{
	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};

template <typename T = bool>
class Var {
public:
	std::string name;
	std::shared_ptr<T> value;
	int32_t size;
	Var(std::string name, T v) : name(name) {
		value = std::make_shared<T>(v);
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T*() { return &*value; }
	operator T() const { return *value; }
	//operator T*() const { return value; }
};

enum AntiAimPitch_t
{
	ANTIAIM_PITCH_NONE,
	ANTIAIM_PITCH_DOWN,
	ANTIAIM_PITCH_UP,
	ANTIAIM_PITCH_DANCE,
	ANTIAIM_PITCH_FAKEUP,
	ANTIAIM_PITCH_FAKEDOWN,
	ANTIAIM_PITCH_RANDOM
};

enum AntiAimYaw_t
{
	ANTIAIM_YAW_NONE,
	ANTIAIM_YAW_SPIN,
	ANTIAIM_YAW_STATIC_FORWARD,
	ANTIAIM_YAW_STATIC_RIGHT,
	ANTIAIM_YAW_STATIC_BACKWARDS,
	ANTIAIM_YAW_STATIC_LEFT,
	ANTIAIM_YAW_BACKWARDS,
	ANTIAIM_YAW_LEFT,
	ANTIAIM_YAW_RIGHT,
	ANTIAIM_YAW_SIDE,
	ANTIAIM_YAW_FAKE_LBY1,
	ANTIAIM_YAW_FAKE_LBY2,
	ANTIAIM_YAW_JITTER,
	ANTIAIM_YAW_BACKJITTER,
	ANTIAIM_YAW_FAKE_SIDE_LBY,
	ANTIAIM_YAW_RANDOM
};

enum WEAPON_GROUPS {
	PISTOLS,
	RIFLES,
	SHOTGUNS,
	SCOUT,
	AUTO,
	AWP,
	SMG,
	UNKNOWN
};

struct aimbot_settings {
	int autofire_key = 0;
	bool enabled = false;
	bool autofire = false;
	bool on_key = true;
	int key = 0;
	bool deathmatch = false;
	bool autopistol = false;
	bool check_smoke = false;
	bool check_flash = false;
	bool check_jump = false;
	bool autowall = false;
	bool silent = false;
	bool antiaimlock = false;
	bool rcs = false;
	bool rcs_fov_enabled = false;
	bool rcs_smooth_enabled = false;
	bool humanize = false;
	float curviness = false;
	struct {
		bool enabled = false;
		int ticks = 6;
	} backtrack;
	bool only_in_zoom = true;
	int aim_type = 1;
	int smooth_type;
	int priority = 0;
	int fov_type = 0;
	int rcs_type = 0;
	int hitbox = 1;
	float fov = 0.f;
	float silent_fov = 0.f;
	float rcs_fov = 0.f;
	float smooth = 1;
	float rcs_smooth = 1;
	int shot_delay = 0;
	int kill_delay = 0;
	int rcs_x = 100;
	int rcs_y = 100;
	int rcs_start = 1;
	int min_damage = 1;
};

class Options
{
public:
	//
    //RAGEBOT
    //
	OPTION(bool, rage_enabled, false);
	OPTION(bool, rage_resolver, false);
	OPTION(bool, rage_autor8, false);

	OPTION(float, HeadScale, 0.f);
	OPTION(float, BodyScale, 0.f);

	OPTION(bool, autoscope, false);
	OPTION(int, rage_mindmg, 55);
	OPTION(int, rage_mpsize, 0);
	OPTION(int, rage_hitchance, 50);

	OPTION(int, RageAimbotHitchanceAuto, 50);
	OPTION(int, RageAimbotMinDmgAuto, 50);

	OPTION(int, RageAimbotHitchanceScout, 50);
	OPTION(int, RageAimbotMinDmgScout, 50);

	OPTION(int, RageAimbotHitchanceAWP, 50);
	OPTION(int, RageAimbotMinDmgAWP, 50);

	OPTION(int, RageAimbotHitchancePistols, 50);
	OPTION(int, RageAimbotMinDmgPistols, 50);

	OPTION(int, RageAimbotHitchanceDeagle, 50);
	OPTION(int, RageAimbotMinDmgDeagle, 50);

	OPTION(int, RageAimbotHitchanceOther, 50);
	OPTION(int, RageAimbotMinDmgOther, 50);

	bool rage_autostop;
	bool rage_hitscan[7];
	OPTION(bool, rage_predict_enemy_lby, true);
	int doubletab;
	int fastshoot;
	int RageAimbotBaimAfter = 0;

	// 
	// ANTIAIM
	// 
	OPTION(float, antiaim_edge_dist, 0.0f);
	OPTION(bool, antiaim_antiresolver, false);
	OPTION(bool, AntiAim_enable, false);

	OPTION(int, antiaim_pitch, ANTIAIM_PITCH_NONE);
	OPTION(int, antiaim_yaw, ANTIAIM_YAW_NONE);
	OPTION(int, antiaim_yaw_fake, ANTIAIM_YAW_NONE);
	OPTION(float, antiaim_spin_speed, 0.0f);
	OPTION(bool, antiaim_knife, false);
	OPTION(int, tankAntiaimKey, 0);
	OPTION(bool, antiuntrusted, true);

	int settings_category = 0;
	int weapon_group = 0;
	int settings_weapon = 0;
	aimbot_settings                legitbot_items_all;
	std::map<int, aimbot_settings> legitbot_items;
	std::map<int, aimbot_settings> legitbot_items_groups;
	OPTION(bool, esp_preview, false);
	OPTION(bool, misc_anti_obs, true);
	OPTION(bool, other_nightmode, false);
	OPTION(float, other_nightmode_size, 0.05f);
	OPTION(bool, maykilceksin, false);
	OPTION(bool, theme, false);
	int nightmode_brightness{ 100 };
	float clr_sky[4]{ 1.0f,1.0f,1.0f,255.0f };
	OPTION(bool, aspect_ratio_enable, false);
	OPTION(float, aspect_ratio, 1.05);
	OPTION(bool, gorus_saydamduvarbakim, false);
	OPTION(bool, esp_weapon_icon, false);
	bool skin_preview = false;
	bool show_cur = true;
    std::map<int, statrack_setting> statrack_items;
	std::map<int, item_setting> m_items;
	std::map<std::string, std::string> m_icon_overrides;
	bool deathmatch = false;
	OPTION(int, bhop_hit_chance, 0);
	OPTION(int, hops_restricted_limit, 0);
	OPTION(int, max_hops_hit, 0);
	OPTION(bool, misc_human_bhop, false);
	OPTION(bool, misc_reveal_money, false);
	OPTION(bool, fake_chams, false);
	float esp_localplayer_chams_color_visible[4] = { 0.15f, 0.7f, 1.f, 1.0f };
	float esp_localplayer_chams_color_invisible[4] = { 0.f, 0.2f, 1.f, 1.f };
	float esp_localplayer_fakechams_color_invisible[4] = { 1.f, 0.f, 0.f, 1.f };
	float esp_localplayer_fakechams_color_visible[4] = { 1.f, 0.f, 0.f, 1.0f };
	OPTION(bool, esp_localplayer_chams, false);
	OPTION(bool, esp_localplayer_chams_xyz, false);
	OPTION(int, esp_localplayer_chams_type, 0);
	OPTION(int, esp_localplayer_fakechams_type, 0);
	OPTION(bool, gorus_diger_mermiizi, false);
	float gorus_diger_mermiizi_color[4] = { 0.012f, 0.788f, 0.663f, 1.f };
	OPTION(int, misc_minwalk_bind, 0);
	OPTION(bool, misc_minwalk, false);
	OPTION(int, misc_minwalk_amount, 0);

	OPTION(int, selected_theme, 0);
	OPTION(bool, misc_watermark, false);
	//ESP
	float color_esp_sounds[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	OPTION(bool, esp_sounds, false);
	OPTION(bool, esp_optimise, false);
	OPTION(float, esp_sounds_time, 0.5f);
	OPTION(float, esp_sounds_radius, 15.0f);
	OPTION(bool, esp_flags_armor, false);
	OPTION(bool, esp_flags_kit, false);
	OPTION(bool, esp_flags_flashed, false);
	OPTION(bool, esp_flags_hostage, false);
	OPTION(bool, esp_flags_scoped, false);
	OPTION(bool, esp_flags_money, false);
	OPTION(bool, esp_flags_c4, false);
	OPTION(bool, esp_flags_defusing, false);
	int   RadarType = 0;
	bool Act = 0;
	int	  RadarStyle = 0;
	float Zoom = 1;
	int   Size = 1;
	OPTION(bool, misc_radar, false);
	OPTION(bool, misc_radar_enemyonly, false);
	struct {
		struct {
			Color ally = { 0, 255, 0 };
			Color enemy = { 255, 0, 0 };
		} radar;
	} colors;
	struct
	{
		bool enabled = false;
		bool textured = false;

		bool ingame = false;

		bool enemies = false;
		bool teammates = false;

		int range = 2000;
		int size = 250;
		int points_size = 3;
	} radar;
	OPTION(int, layer, 0);
	OPTION(int, radiochannel, 0);
	OPTION(int, radiovolume, 0);
	OPTION(int, radiomute, 0);
	int dista = 0;
	int distb = 0;
	OPTION(bool, kbot, false);
	OPTION(bool, nosmoke, false);
	OPTION(bool, ANIMFIX, false);
	OPTION(bool, nozoom, false);
	OPTION(bool, debug_showposes, false);
	OPTION(bool, debug_showactivities, false);
	OPTION(int, fakelags, 0);

	OPTION(bool, enable_trigger, false);
	OPTION(int, trigger_key, false);
	OPTION(bool, trigger_head, false);
	OPTION(bool, trigger_arms, false);
	OPTION(bool, trigger_stomatch, false);
	OPTION(bool, trigger_legs, false);
	OPTION(bool, trigger_chest, false);
	OPTION(bool, autostop2, false);
	OPTION(bool, trigger_smokecheck, false);
	OPTION(bool, trigger_flashcheck, false);
	OPTION(bool, trigger_jumpcheck, false);
	OPTION(bool, trigger_magnet, false);
	OPTION(bool, enable_trigger_auto, false);
	OPTION(int, trigger_delay, 1.f);
	OPTION(int, trigger_hitchance, 0.f);
	OPTION(bool, enable_show_trigger, false);
	///
	OPTION(bool, cl_phys_timescale, false);
	OPTION(float, cl_phys_timescale_value, 0.1f);
	OPTION(float, flash, 255.f);
	OPTION(bool, misc_nosleeves, false);
     OPTION(bool, misc_fastduck, false);
	OPTION(bool, antiscreen, false);
	OPTION(bool, resolver,false);
	OPTION(bool, eefktelr_sekmekapali, false);
	OPTION(float , retrack, 15.f);
	///
	OPTION(int, player_model_t, 0);
	OPTION(int, player_model_ct, 0);
	OPTION(bool, wasd, false);
	OPTION(bool, zeusrange, false);
	OPTION(bool, misc_autodefuse, false);
	OPTION(bool, solbicak, false);
	OPTION(bool, misc_disableblur, false);
	OPTION(bool, misc_rainbowbar, false);
	OPTION(int, misc_chatspam, false);
	OPTION(bool, skychanger, false);
	OPTION(int, skychanger_mode, 0);
	OPTION(bool, buylog, false);
	OPTION(int, misc_sky_boxes, 0);

	char spam[128];
	OPTION(bool, use_all_weapons, false);
	OPTION(bool, esp_angle_lines, false);
	OPTION(bool, esp_speclist, false);
	OPTION(bool, esp_damageindicator, false);
	OPTION(bool, hitsound, false);
	OPTION(bool, esp_damageindicator_color, false);
	OPTION(bool, show_ammo, false);
	OPTION(bool, misc_autoaccept, false);
	OPTION(bool, edgejump, false);
	OPTION(int, edgejump_bind, 0);
	OPTION(bool, namestealer, false);
	OPTION(bool, fakeduck, false);
	OPTION(int, fakeduck_bind, 0);
	OPTION(bool, misc_desync_ad, false);
	
	OPTION(int, misc_theme, 0);

	OPTION(int, misc_desync, 0);
	OPTION(int, misc_desync_bind, 0);

	OPTION(bool, autoblock, false);
	OPTION(bool, hiteffect, false);
	OPTION(int, autoblock_bind, 0);
	OPTION(bool, misc_customclan, false);
	char customclan[16];
	OPTION(bool, menu_language, false);
	// 
	// ESP
	// 
	OPTION(bool, esp_enabled, false);
	OPTION(bool, esp_visible_only, false);
	OPTION(bool, esp_enemy_only, false);
	OPTION(bool, esp_farther, false);
	OPTION(int, esp_player_box, 0);
	OPTION(bool, foottrils, false);
	OPTION(float, foottrails_xshift, 0.1f);
	OPTION(float, foottrails_yshift, 0.1f);
	OPTION(float, foottrails_zshift, 14.f);
	OPTION(bool, esp_player_skeleton, false);
	OPTION(bool, esp_player_skeleton_bt, false);
	OPTION(bool, esp_player_names, false);
	OPTION(bool, esp_player_health, false);
	OPTION(bool, esp_player_armour, false);
	OPTION(bool, esp_player_weapons, false);

	OPTION(bool, chams_arms_enabled, false);
	OPTION(bool, chams_arms_wireframe, false);
	OPTION(bool, chams_arms_reflect, false);
	OPTION(int, chams_arms_mat, false);
	float chams_arms_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	bool kill_counter;

	OPTION(bool, chams_sleeves_enabled, false);
	OPTION(bool, chams_sleeves_wireframe, false);
	OPTION(bool, chams_sleeves_reflect, false);
	OPTION(int, chams_sleeves_mat, false);
	float chams_sleeves_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	OPTION(bool, chams_weapon_enabled, false);
	OPTION(bool, recoilCrosshair, false);
	OPTION(bool, chams_weapon_wireframe, false);
	OPTION(bool, chams_weapon_reflect, false);
	OPTION(int, chams_weapon_mat, false);
	float chams_weapon_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	OPTION(bool, esp_dropped_weapons, false);	
	OPTION(bool, esp_droppped_ammo_bar, false);
	OPTION(bool, esp_dropped_weapons_box, false);
	OPTION(bool, misc_autozeus, false);

	OPTION(bool, crosshair, false);	
	OPTION(bool, esp_defuse_kit, false);
	OPTION(bool, esp_dropped_defusekits_box, false);
	OPTION(bool, esp_dropped_c4_box, false);
	float color_esp_c4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_defusekit[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_items, false);
	OPTION(float, esp_distance_dz, 1024.f);	
	float color_esp_weapons[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_esp_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_skeleton[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_esp_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	OPTION(bool, nightmode, false);
	float nightmode_intensity = 0.30f;
	float sky_color[3] = { 0.15f, 0.15f, 0.15f };
	//
	// CHAMS
	//
	OPTION(bool, chams_player_enabled, false);
	OPTION(bool, chams_player_enemies_only, false);
	OPTION(bool, chams_player_wireframe, false);
	OPTION(bool, chams_player_fake, false);
	OPTION(bool, chams_player_flat, false);
	OPTION(bool, chams_player_visibleonly, false);
	OPTION(bool, chams_player_glass, false);
	float color_chams_player_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_player_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float color_chams_real[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	bool grenades_enable = false;

	bool smoke_timer = false;
	bool smoke_bar = true;
	bool molotov_timer = false;
	bool molotov_bar = true;

	float color_smoke[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_molotov[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_decoy[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_flash[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_frag[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float color_bar_smoke_back[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_bar_smoke_main[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	float color_bar_molotov_back[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float color_bar_molotov_main[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	float color_bullet_tracer[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	bool visuals_bullet_tracer = false;

	// 
	// GLOW
	// 
	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_pulsing, false);
	OPTION(bool, glow_enemy_only, false);
	float color_glow[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	// 
	// OTHER
	// 
	OPTION(bool, other_drawfov, false);
	OPTION(float, other_drawfov_fov, 0.f);
	OPTION(bool, other_no_hands, false);
	OPTION(bool, esp_grenade_prediction, false);
	OPTION(float, other_mat_ambient_light_r, 0.0f);
	OPTION(float, other_mat_ambient_light_g, 0.0f);
	OPTION(float, other_mat_ambient_light_b, 0.0f);
	OPTION(bool, other_mat_ambient_light_rainbow, false);

	//
	// MISC
	//
	OPTION(bool, DiscordRichPresense, false);
	OPTION(bool, misc_bhop, false);
	OPTION(bool, misc_autostrafer, false);
	OPTION(bool, misc_clantag, false);
	OPTION(bool, misc_clantaga, false);
	OPTION(bool, misc_thirdperson, false);
	OPTION(int, misc_thirdperson_bind, 0);
	OPTION(bool, misc_rankreveal, false);
	OPTION(float, misc_thirdperson_dist, 50.f);
	OPTION(int, misc_viewmodel_fov, 68);
	OPTION(int, misc_fov, 90);	

	OPTION(bool, fakelag_enabled, false);
	OPTION(bool, fakelag_standing, false);
	OPTION(bool, fakelag_moving, false);
	OPTION(bool, fakelag_unducking, false);
	OPTION(bool, eventlogger_hurt, false);
	OPTION(bool, eventlogger_player_buy, false);
	OPTION(bool, eventlogger_planting, false);
	OPTION(bool, eventlogger_defusing, false);

	OPTION(int, fakelag_mode, 0);
	OPTION(int, fakelag_factor, 0);

	Color accent_color = Color(255, 255, 255, 100);
	OPTION(bool, custommodelson, false);
	OPTION(int, customodelsct, 0);
	OPTION(int, customodelst, 0);
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, std::string> skinNames;
};

extern Options g_Options;
extern bool   g_Unload;

