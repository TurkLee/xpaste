#include "config.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../menu.hpp"
#include "../helpers/input.hpp"
#include "../options.hpp"
#include "../features/skin/item_definitions.h"

// For ints, chars, floats, and bools
void CConfig::SetupValue(int& value, int def, std::string category, std::string name) { value = def; ints.push_back(new ConfigValue< int >(category, name, &value, def)); }
void CConfig::SetupValue(char* value, char* def, std::string category, std::string name) { value = def; chars.push_back(new ConfigValue< char >(category, name, value, *def)); }
void CConfig::SetupValue(float& value, float def, std::string category, std::string name) { value = def; floats.push_back(new ConfigValue< float >(category, name, &value, def)); }
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name) { value = def; bools.push_back(new ConfigValue< bool >(category, name, &value, def)); }
void CConfig::SetupColor(float value[4], std::string name) {
	SetupValue(value[0], value[0], ("colors"), name + "_r");
	SetupValue(value[1], value[1], ("colors"), name + "_g");
	SetupValue(value[2], value[2], ("colors"), name + "_b");
	SetupValue(value[3], value[3], ("colors"), name + "_a");
}

void CConfig::SetupRage() {

	SetupValue(g_Options.misc_minwalk, false, ("rage"), ("misc_minwalk"));
	SetupValue(g_Options.misc_minwalk_bind, 0, ("rage"), ("misc_minwalk_bind"));

}

void CConfig::SetupLegit() {
	/*LegitBot*/
	SetupValue(g_Options.resolver, 0, ("reso"), ("enable"));
	for (auto& [key, val] : k_weapon_names) {
		SetupValue(g_Options.legitbot_items[key].autofire_key, 1, (val), ("autofire_key"));
		SetupValue(g_Options.legitbot_items[key].autofire, false, (val), ("autofire"));
		SetupValue(g_Options.legitbot_items[key].deathmatch, false, (val), ("deathmatch"));
		SetupValue(g_Options.legitbot_items[key].autopistol, false, (val), ("autopistol"));
		SetupValue(g_Options.legitbot_items[key].check_smoke, false, (val), ("smoke_check"));
		SetupValue(g_Options.legitbot_items[key].check_flash, false, (val), ("flash_check"));
		SetupValue(g_Options.legitbot_items[key].check_jump, false, (val), ("jump_check"));

		SetupValue(g_Options.legitbot_items[key].backtrack.ticks, 0, (val), ("backtrack_time"));
		SetupValue(g_Options.legitbot_items[key].backtrack.enabled, 0, (val), ("backtrack_enabled"));

		SetupValue(g_Options.legitbot_items[key].autowall, false, (val), ("autowall"));
		SetupValue(g_Options.legitbot_items[key].silent, false, (val), ("silent"));
		SetupValue(g_Options.legitbot_items[key].on_key, true, (val), ("on_key"));
		SetupValue(g_Options.legitbot_items[key].autofire, false, (val), ("autofire"));
		SetupValue(g_Options.legitbot_items[key].key, 1, (val), ("key"));
		SetupValue(g_Options.legitbot_items[key].enabled, false, (val), ("aim_enabled"));
		SetupValue(g_Options.legitbot_items[key].rcs, false, (val), ("rcs"));
		SetupValue(g_Options.legitbot_items[key].rcs_fov_enabled, false, (val), ("rcs_fov_enabled"));
		SetupValue(g_Options.legitbot_items[key].rcs_smooth_enabled, false, (val), ("rcs_smooth_enabled"));

		SetupValue(g_Options.legitbot_items[key].only_in_zoom, false, (val), ("only_in_zoom"));
		SetupValue(g_Options.legitbot_items[key].aim_type, 1, (val), ("aim_type"));
		SetupValue(g_Options.legitbot_items[key].priority, 0, (val), ("priority"));
		SetupValue(g_Options.legitbot_items[key].fov_type, 0, (val), ("fov_type"));
		SetupValue(g_Options.legitbot_items[key].rcs_type, 0, (val), ("rcs_type"));
		SetupValue(g_Options.legitbot_items[key].smooth_type, 0, (val), ("smooth_type"));
		SetupValue(g_Options.legitbot_items[key].hitbox, 1, (val), ("hitbox"));
		SetupValue(g_Options.legitbot_items[key].fov, 0.f, (val), ("fov"));
		SetupValue(g_Options.legitbot_items[key].silent_fov, 0.f, (val), ("silent_fov"));
		SetupValue(g_Options.legitbot_items[key].rcs_fov, 0.f, (val), ("rcs_fov"));
		SetupValue(g_Options.legitbot_items[key].smooth, 1, (val), ("smooth"));
		SetupValue(g_Options.legitbot_items[key].rcs_smooth, 1, (val), ("rcs_smooth"));
		SetupValue(g_Options.legitbot_items[key].shot_delay, 0, (val), ("shot_delay"));
		SetupValue(g_Options.legitbot_items[key].kill_delay, 0, (val), ("kill_delay"));
		SetupValue(g_Options.legitbot_items[key].rcs_x, 100, (val), ("rcs_x"));
		SetupValue(g_Options.legitbot_items[key].rcs_y, 100, (val), ("rcs_y"));
		SetupValue(g_Options.legitbot_items[key].rcs_start, 1, (val), ("rcs_start"));
		SetupValue(g_Options.legitbot_items[key].min_damage, 1, (val), ("min_damage"));
	}

	for (auto& [key, val] : weapon_groups)
	{
		SetupValue(g_Options.legitbot_items[key].autofire_key, 1, (val), ("autofire_key"));
		SetupValue(g_Options.legitbot_items[key].autofire, false, (val), ("autofire"));
		SetupValue(g_Options.legitbot_items[key].deathmatch, false, (val), ("deathmatch"));
		SetupValue(g_Options.legitbot_items[key].autopistol, false, (val), ("autopistol"));
		SetupValue(g_Options.legitbot_items[key].check_smoke, false, (val), ("smoke_check"));
		SetupValue(g_Options.legitbot_items[key].check_flash, false, (val), ("flash_check"));
		SetupValue(g_Options.legitbot_items[key].check_jump, false, (val), ("jump_check"));

		SetupValue(g_Options.legitbot_items[key].backtrack.ticks, 0, (val), ("backtrack_time"));
		SetupValue(g_Options.legitbot_items[key].backtrack.enabled, 0, (val), ("backtrack_enabled"));

		SetupValue(g_Options.legitbot_items[key].autowall, false, (val), ("autowall"));
		SetupValue(g_Options.legitbot_items[key].silent, false, (val), ("silent"));
		SetupValue(g_Options.legitbot_items[key].on_key, true, (val), ("on_key"));
		SetupValue(g_Options.legitbot_items[key].autofire, false, (val), ("autofire"));
		SetupValue(g_Options.legitbot_items[key].key, 1, (val), ("key"));
		SetupValue(g_Options.legitbot_items[key].enabled, false, (val), ("aim_enabled"));
		SetupValue(g_Options.legitbot_items[key].rcs, false, (val), ("rcs"));
		SetupValue(g_Options.legitbot_items[key].rcs_fov_enabled, false, (val), ("rcs_fov_enabled"));
		SetupValue(g_Options.legitbot_items[key].rcs_smooth_enabled, false, (val), ("rcs_smooth_enabled"));

		SetupValue(g_Options.legitbot_items[key].only_in_zoom, false, (val), ("only_in_zoom"));
		SetupValue(g_Options.legitbot_items[key].aim_type, 1, (val), ("aim_type"));
		SetupValue(g_Options.legitbot_items[key].priority, 0, (val), ("priority"));
		SetupValue(g_Options.legitbot_items[key].fov_type, 0, (val), ("fov_type"));
		SetupValue(g_Options.legitbot_items[key].rcs_type, 0, (val), ("rcs_type"));
		SetupValue(g_Options.legitbot_items[key].smooth_type, 0, (val), ("smooth_type"));
		SetupValue(g_Options.legitbot_items[key].hitbox, 1, (val), ("hitbox"));
		SetupValue(g_Options.legitbot_items[key].fov, 0.f, (val), ("fov"));
		SetupValue(g_Options.legitbot_items[key].silent_fov, 0.f, (val), ("silent_fov"));
		SetupValue(g_Options.legitbot_items[key].rcs_fov, 0.f, (val), ("rcs_fov"));
		SetupValue(g_Options.legitbot_items[key].smooth, 1, (val), ("smooth"));
		SetupValue(g_Options.legitbot_items[key].rcs_smooth, 1, (val), ("rcs_smooth"));
		SetupValue(g_Options.legitbot_items[key].shot_delay, 0, (val), ("shot_delay"));
		SetupValue(g_Options.legitbot_items[key].kill_delay, 0, (val), ("kill_delay"));
		SetupValue(g_Options.legitbot_items[key].rcs_x, 100, (val), ("rcs_x"));
		SetupValue(g_Options.legitbot_items[key].rcs_y, 100, (val), ("rcs_y"));
		SetupValue(g_Options.legitbot_items[key].rcs_start, 1, (val), ("rcs_start"));
		SetupValue(g_Options.legitbot_items[key].min_damage, 1, (val), ("min_damage"));
	}

	auto& settings_cur = g_Options.legitbot_items_all;
	SetupValue(settings_cur.autofire_key, 1, ("aimbot_all"), ("autofire_key"));
	SetupValue(settings_cur.autofire, false, ("aimbot_all"), ("autofire"));
	SetupValue(settings_cur.deathmatch, false, ("aimbot_all"), ("deathmatch"));
	SetupValue(settings_cur.autopistol, false, ("aimbot_all"), ("autopistol"));
	SetupValue(settings_cur.check_smoke, false, ("aimbot_all"), ("smoke_check"));
	SetupValue(settings_cur.check_flash, false, ("aimbot_all"), ("flash_check"));
	SetupValue(settings_cur.check_jump, false, ("aimbot_all"), ("jump_check"));

	SetupValue(settings_cur.backtrack.ticks, 0, ("aimbot_all"), ("backtrack_time"));
	SetupValue(settings_cur.backtrack.enabled, 0, ("aimbot_all"), ("backtrack_enabled"));

	SetupValue(settings_cur.autowall, false, ("aimbot_all"), ("autowall"));
	SetupValue(settings_cur.silent, false, ("aimbot_all"), ("silent"));
	SetupValue(settings_cur.on_key, true, ("aimbot_all"), ("on_key"));
	SetupValue(settings_cur.autofire, false, ("aimbot_all"), ("autofire"));
	SetupValue(settings_cur.key, 1, ("aimbot_all"), ("key"));
	SetupValue(settings_cur.enabled, false, ("aimbot_all"), ("aim_enabled"));
	SetupValue(settings_cur.rcs, false, ("aimbot_all"), ("rcs"));
	SetupValue(settings_cur.rcs_fov_enabled, false, ("aimbot_all"), ("rcs_fov_enabled"));
	SetupValue(settings_cur.rcs_smooth_enabled, false, ("aimbot_all"), ("rcs_smooth_enabled"));

	SetupValue(settings_cur.only_in_zoom, false, ("aimbot_all"), ("only_in_zoom"));
	SetupValue(settings_cur.aim_type, 1, ("aimbot_all"), ("aim_type"));
	SetupValue(settings_cur.priority, 0, ("aimbot_all"), ("priority"));
	SetupValue(settings_cur.fov_type, 0, ("aimbot_all"), ("fov_type"));
	SetupValue(settings_cur.rcs_type, 0, ("aimbot_all"), ("rcs_type"));
	SetupValue(settings_cur.smooth_type, 0, ("aimbot_all"), ("smooth_type"));
	SetupValue(settings_cur.hitbox, 1, ("aimbot_all"), ("hitbox"));
	SetupValue(settings_cur.fov, 0.f, ("aimbot_all"), ("fov"));
	SetupValue(settings_cur.silent_fov, 0.f, ("aimbot_all"), ("silent_fov"));
	SetupValue(settings_cur.rcs_fov, 0.f, ("aimbot_all"), ("rcs_fov"));
	SetupValue(settings_cur.smooth, 1, ("aimbot_all"), ("smooth"));
	SetupValue(settings_cur.rcs_smooth, 1, ("aimbot_all"), ("rcs_smooth"));
	SetupValue(settings_cur.shot_delay, 0, ("aimbot_all"), ("shot_delay"));
	SetupValue(settings_cur.kill_delay, 0, ("aimbot_all"), ("kill_delay"));
	SetupValue(settings_cur.rcs_x, 100, ("aimbot_all"), ("rcs_x"));
	SetupValue(settings_cur.rcs_y, 100, ("aimbot_all"), ("rcs_y"));
	SetupValue(settings_cur.rcs_start, 1, ("aimbot_all"), ("rcs_start"));
	SetupValue(settings_cur.min_damage, 1, ("aimbot_all"), ("min_damage"));
	/*TRIGGER*/
	///////////////////////////////////
	SetupValue(g_Options.enable_trigger, false, ("trigger"), ("trigger_enable"));
	SetupValue(g_Options.trigger_key, false, ("trigger"), ("trigger_key"));
	SetupValue(g_Options.trigger_head, false, ("trigger"), ("trigger_head"));
	SetupValue(g_Options.trigger_arms, false, ("trigger"), ("trigger_arms"));
	SetupValue(g_Options.trigger_chest, false, ("trigger"), ("trigger_chest"));
	SetupValue(g_Options.trigger_stomatch, false, ("trigger"), ("trigger_stomach"));
	SetupValue(g_Options.trigger_legs, false, ("trigger"), ("trigger_legs"));
	SetupValue(g_Options.trigger_delay, 0.f, ("trigger"), ("trigger_delay"));
	SetupValue(g_Options.trigger_flashcheck, false, ("trigger"), ("trigger_flashcheck"));
	SetupValue(g_Options.trigger_smokecheck, false, ("trigger"), ("trigger_smokecheck"));
	SetupValue(g_Options.autostop2, false, ("trigger"), ("auto_stop_trigger"));
	SetupValue(g_Options.enable_trigger_auto, false, ("trigger"), ("triggerbot_automatic"));
	SetupValue(g_Options.enable_show_trigger, false, ("trigger"), ("triggerbot_enabled"));
	SetupValue(g_Options.trigger_magnet, false, ("aimbot"), ("trigger_magnet_aimbot"));
	SetupValue(g_Options.trigger_hitchance, 0.f, ("trigger"), ("trigger_hitchance"));
}

void CConfig::SetupVisuals() {
	SetupValue(g_Options.esp_enabled, false, ("esp"), ("enabled"));
	SetupValue(g_Options.esp_visible_only, false, ("esp"), ("esp_visible_only"));
	SetupValue(g_Options.show_ammo, false, ("esp"), ("show_ammo"));
	SetupValue(g_Options.crosshair, false, ("esp"), ("crosshair"));
	SetupValue(g_Options.nozoom, false, ("esp"), ("nozoom"));
	SetupValue(g_Options.esp_enemy_only, false, ("esp"), ("enemies_only"));
	SetupValue(g_Options.esp_player_skeleton, false, ("esp"), ("player_skeleton"));
	SetupValue(g_Options.esp_player_box, 0, ("esp"), ("player_boxes_type"));
	SetupValue(g_Options.esp_player_names, false, ("esp"), ("player_names"));
	SetupValue(g_Options.esp_player_health, false, ("esp"), ("player_health"));
	SetupValue(g_Options.esp_player_armour, false, ("esp"), ("player_armour"));
	SetupValue(g_Options.esp_player_weapons, false, ("esp"), ("player_weapons"));
	SetupValue(g_Options.esp_weapon_icon, false, ("esp"), ("player_weapon_icons"));
	SetupValue(g_Options.esp_dropped_weapons, false, ("esp"), ("dropped_weapons"));
	SetupValue(g_Options.esp_dropped_weapons_box, false, ("esp"), ("esp_dropped_weapons_box"));
	SetupValue(g_Options.esp_droppped_ammo_bar, false, ("esp"), ("esp_droppped_ammo_bar"));
	SetupValue(g_Options.esp_defuse_kit, false, ("esp"), ("defuse_kit"));
	SetupValue(g_Options.esp_planted_c4, false, ("esp"), ("planted_c4"));
	SetupValue(g_Options.recoilCrosshair, false, ("esp"), ("recoil"));
	SetupValue(g_Options.hiteffect, false, ("esp"), ("effect"));
	SetupValue(g_Options.kill_counter, false, ("esp"), ("counter"));
	SetupValue(g_Options.esp_dropped_c4_box, false, ("esp"), ("esp_dropped_c4_box"));
	SetupValue(g_Options.esp_items, false, ("esp"), ("items"));
	SetupValue(g_Options.esp_grenade_prediction, false, ("esp"), ("grenade_prediction"));
	SetupValue(g_Options.esp_sounds, false, ("esp"), ("sounds"));
	SetupValue(g_Options.esp_sounds_time, 0.5f, ("esp"), ("sounds_time"));
	SetupValue(g_Options.esp_sounds_radius, 15.0f, ("esp"), ("sounds_radius"));
	SetupValue(g_Options.esp_angle_lines, false, ("esp"), ("angle_lines"));
	SetupValue(g_Options.chams_player_visibleonly, false, ("esp"), ("chams_player_visibleonly"));
	SetupValue(g_Options.smoke_bar, false, ("esp"), ("smoke_bar"));
	SetupValue(g_Options.smoke_timer, false, ("esp"), ("smoke_timer"));
	SetupValue(g_Options.molotov_bar, false, ("esp"), ("molotov_bar"));
	SetupValue(g_Options.molotov_timer, false, ("esp"), ("molotov_timer"));
	SetupValue(g_Options.grenades_enable, false, ("esp"), ("grenades_enable"));
	SetupValue(g_Options.glow_enabled, false, ("glow"), ("enabled"));
	SetupValue(g_Options.glow_pulsing, false, ("glow"), ("glow_pulsing"));
	SetupValue(g_Options.glow_enemy_only, false, ("glow"), ("glow_enemy_only"));
	SetupValue(g_Options.chams_player_enabled, false, ("chams"), ("player_enabled"));
	SetupValue(g_Options.chams_player_enemies_only, false, ("chams"), ("player_enemies_only"));
	SetupValue(g_Options.chams_player_wireframe, false, ("chams"), ("player_wireframe"));
	SetupValue(g_Options.chams_player_flat, false, ("chams"), ("player_flat"));
	SetupValue(g_Options.chams_player_glass, false, ("chams"), ("player_glass"));
	SetupValue(g_Options.chams_sleeves_enabled, false, ("chams"), ("chams_sleeves_enabled"));
	SetupValue(g_Options.chams_weapon_enabled, false, ("chams"), ("chams_weapon_enabled"));
	SetupValue(g_Options.chams_arms_enabled, false, ("chams"), ("chams_arms_enabled"));
	SetupValue(g_Options.chams_weapon_wireframe, false, ("chams"), ("chams_weapon_wireframe"));
	SetupValue(g_Options.chams_weapon_reflect, false, ("chams"), ("chams_weapon_reflect"));
	SetupValue(g_Options.nightmode, false, ("other"), ("nightmode"));
	SetupValue(g_Options.misc_sky_boxes, 0, ("misc"), ("misc_sky_boxes"));
	SetupValue(g_Options.nightmode_brightness, 100, ("misc"), ("misc_nightmode_brightness"));
	SetupValue(g_Options.other_mat_ambient_light_r, false, ("other"), ("mat_ambient_light_r"));
	SetupValue(g_Options.other_mat_ambient_light_g, false, ("other"), ("mat_ambient_light_g"));
	SetupValue(g_Options.other_mat_ambient_light_b, false, ("other"), ("mat_ambient_light_b"));
	SetupValue(g_Options.esp_speclist, false, ("other"), ("esp_speclist"));
	SetupValue(g_Options.esp_damageindicator, false, ("other"), ("esp_damageindicator"));
	SetupValue(g_Options.esp_damageindicator_color, false, ("other"), ("esp_damageindicator_color"));
	SetupValue(g_Options.hitsound, false, ("other"), ("hitsound"));
	SetupValue(g_Options.Act, false, ("other"), ("Act"));
	SetupValue(g_Options.other_drawfov, false, ("other"), ("drawfov"));
	SetupValue(g_Options.other_no_hands, false, ("other"), ("no_hands"));
	SetupValue(g_Options.esp_flags_armor, false, ("esp"), ("esp_flags_armor"));
	SetupValue(g_Options.esp_flags_defusing, false, ("esp"), ("esp_flags_defusing"));
	SetupValue(g_Options.esp_flags_kit, false, ("esp"), ("esp_flags_kit"));
	SetupValue(g_Options.esp_flags_hostage, false, ("esp"), ("esp_flags_hostage"));
	SetupValue(g_Options.esp_flags_scoped, false, ("esp"), ("esp_flags_scoped"));
	SetupValue(g_Options.esp_flags_money, false, ("esp"), ("esp_flags_money"));
	SetupValue(g_Options.esp_flags_c4, false, ("esp"), ("esp_flags_c4"));
	SetupValue(g_Options.esp_flags_flashed, false, ("esp"), ("esp_flags_flashed"));
}

void CConfig::SetupMisc() {
	SetupValue(g_Options.misc_bhop, false, ("misc"), ("bhop"));
	SetupValue(g_Options.misc_autostrafer, false, ("misc"), ("misc_autostrafer"));
	SetupValue(g_Options.misc_rankreveal, false, ("misc"), ("misc_rankreveal"));
	SetupValue(g_Options.namestealer, false, ("misc"), ("namestealer"));
	SetupValue(g_Options.edgejump, false, ("misc"), ("edgejump"));
	SetupValue(g_Options.autoblock, false, ("misc"), ("autoblock"));
	SetupValue(g_Options.zeusrange, false, ("misc"), ("zeusrange"));
	SetupValue(g_Options.aspect_ratio_enable, false, ("misc"), ("aspect_ratio_enable"));
	SetupValue(g_Options.aspect_ratio, 0.f, ("misc"), ("aspect_ratio"));
	SetupValue(g_Options.nosmoke, false, ("misc"), ("nosmoke"));
	SetupValue(g_Options.misc_nosleeves, false, ("misc"), ("misc_nosleeves"));
	//SetupValue(g_Options.flash, 255.f, ("misc"), ("flash"));
	SetupValue(g_Options.fakeduck, false, ("misc"), ("fakeduck"));
	SetupValue(g_Options.fakeduck_bind, 0, ("misc"), ("fakeduck_bind"));
	SetupValue(g_Options.eventlogger_player_buy, false, ("misc"), ("eventlogger_player_buy"));
	SetupValue(g_Options.eventlogger_planting, false, ("misc"), ("eventlogger_planting"));
	SetupValue(g_Options.eventlogger_defusing, false, ("misc"), ("eventlogger_defusing"));
	SetupValue(g_Options.misc_autodefuse, false, ("misc"), ("misc_autodefuse"));
	SetupValue(g_Options.misc_watermark, false, ("misc"), ("misc_watermark"));
	SetupValue(g_Options.misc_rainbowbar, false, ("misc"), ("misc_rainbowbar"));
	SetupValue(g_Options.retrack, 0.f, ("misc"), ("retrack"));
	SetupValue(g_Options.misc_reveal_money, false, ("misc"), ("misc_reveal_money"));
	SetupValue(g_Options.cl_phys_timescale, false, ("misc"), ("cl_phys_timescale"));
	SetupValue(g_Options.cl_phys_timescale_value, 0.f, ("misc"), ("cl_phys_timescale_value"));
	SetupValue(g_Options.misc_fastduck, false, ("misc"), ("misc_fastduck"));
	SetupValue(g_Options.misc_human_bhop, false, ("misc"), ("misc_human_bhop"));
	SetupValue(g_Options.bhop_hit_chance, 0, ("misc"), ("bhop_hit_chance"));
	SetupValue(g_Options.hops_restricted_limit, 0, ("misc"), ("hops_restricted_limit"));
	SetupValue(g_Options.max_hops_hit, 0, ("misc"), ("max_hops_hit"));
	SetupValue(g_Options.kbot, false, ("misc"), ("kbot"));
	SetupValue(g_Options.esp_optimise, false, ("misc"), ("esp_optimise"));
	SetupValue(g_Options.misc_thirdperson, false, ("misc"), ("thirdperson"));
	SetupValue(g_Options.misc_thirdperson_bind, 0, ("misc"), ("thirdperson_bind"));
	SetupValue(g_Options.misc_clantag, false, ("misc"), ("clantag"));
	SetupValue(g_Options.solbicak, false, ("misc"), ("Knifes2s"));
	SetupValue(g_Options.misc_autoaccept, false, ("misc"), ("autoaccept"));
	SetupValue(g_Options.misc_desync, 0, ("misc"), ("desync"));
	SetupValue(g_Options.misc_desync_bind, 0, ("misc"), ("desync_bind"));
	SetupValue(g_Options.misc_thirdperson_dist, 50.f, ("misc"), ("thirdperson_dist"));
	SetupValue(g_Options.misc_viewmodel_fov, 68, ("misc"), ("viewmodel_fov"));
	SetupValue(g_Options.fakelag_enabled, false, ("fakelag"), ("enabled"));
	SetupValue(g_Options.fakelag_standing, false, ("fakelag"), ("standing"));
	SetupValue(g_Options.fakelag_moving, false, ("fakelag"), ("moving"));
	SetupValue(g_Options.fakelag_unducking, false, ("fakelag"), ("unducking"));
	SetupValue(g_Options.fakelag_mode, 0, ("fakelag"), ("mode"));
	SetupValue(g_Options.fakelag_factor, 0, ("fakelag"), ("factor"));
	SetupValue(g_Options.theme, false, ("misc"), ("theme"));

}

void CConfig::SetupSkins() {
	for (auto& key : weapon_names) {
		SetupValue(g_Options.m_items[key.definition_index].name, "Default", (key.name), ("name"));
		SetupValue(g_Options.m_items[key.definition_index].definition_vector_index, 0, (key.name), ("definition_vector_index"));
		SetupValue(g_Options.m_items[key.definition_index].definition_index, 1, (key.name), ("definition_index"));
		SetupValue(g_Options.m_items[key.definition_index].paint_kit_vector_index, 0, (key.name), ("paint_kit_vector_index"));
		SetupValue(g_Options.m_items[key.definition_index].paint_kit_index, 0, (key.name), ("paint_kit_index"));
		SetupValue(g_Options.m_items[key.definition_index].definition_override_vector_index, 0, (key.name), ("definition_override_vector_index"));
		SetupValue(g_Options.m_items[key.definition_index].definition_override_index, 0, (key.name), ("definition_override_index"));
		SetupValue(g_Options.m_items[key.definition_index].seed, 0, (key.name), ("seed"));
		SetupValue(g_Options.m_items[key.definition_index].stat_trak, 0, (key.name), ("stat_trak"));
		SetupValue(g_Options.m_items[key.definition_index].wear, 0.0f, (key.name), ("wear"));
		SetupValue(g_Options.m_items[key.definition_index].custom_name, "", (key.name), ("custom_name"));
	}
}

void CConfig::SetupColors() {
	SetupColor(g_Options.color_esp_visible, "color_esp_visible");
	SetupColor(g_Options.color_esp_occluded, "color_esp_occluded");
	SetupColor(g_Options.color_esp_skeleton, "color_esp_skeleton");
	SetupColor(g_Options.color_glow, "color_glow");
	SetupColor(g_Options.color_chams_player_visible, "color_chams_player_visible");
	SetupColor(g_Options.color_chams_player_occluded, "color_chams_player_occluded");
	SetupColor(g_Options.chams_arms_color, "chams_arms_color");
	SetupColor(g_Options.chams_sleeves_color, "chams_sleeves_color");
	SetupColor(g_Options.chams_weapon_color, "chams_weapon_color");
	SetupColor(g_Options.color_esp_sounds, "color_esp_sounds");
	SetupColor(g_Options.color_esp_weapons, "color_esp_weapons");
	SetupColor(g_Options.color_esp_c4, "color_esp_c4");
	SetupColor(g_Options.color_smoke, "color_smoke");
	SetupColor(g_Options.color_bar_smoke_main, "color_bar_smoke_main");
	SetupColor(g_Options.color_bar_smoke_back, "color_bar_smoke_back");
	SetupColor(g_Options.color_flash, "color_flash");
	SetupColor(g_Options.color_frag, "color_frag");
	SetupColor(g_Options.color_decoy, "color_decoy");
	SetupColor(g_Options.color_molotov, "color_molotov");
	SetupColor(g_Options.color_bar_molotov_main, "color_bar_molotov_main");
	SetupColor(g_Options.color_bar_molotov_back, "color_bar_molotov_back");
	SetupColor(g_Options.clr_sky, ("misc_nightmode_clr_sky"));

}

void CConfig::Setup() {
	CConfig::SetupRage();
	CConfig::SetupLegit();
	CConfig::SetupVisuals();
	CConfig::SetupMisc();
	CConfig::SetupSkins();
	CConfig::SetupColors();
}

#include "../helpers/utils.hpp"
void CConfig::Save(const std::string& name) {
	if (name.empty())
		return;

	CreateDirectoryA(u8"C:\\Xhanger\\", NULL);
	std::string file = u8"C:\\Xhanger\\" + name;

	for (auto value : ints) {
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	}

	for (auto value : floats) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	for (auto value : bools) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(const std::string& name) {
	if (name.empty())
		return;

	g_ClientState->ForceFullUpdate();

	CreateDirectoryA(u8"C:\\Xhanger\\", NULL);
	std::string file = u8"C:\\Xhanger\\" + name;

	char value_l[32] = { '\0' };
	for (auto value : ints) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0", value_l, 32, file.c_str()); *value->value = atoi(value_l);
	}

	for (auto value : floats) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0.0f", value_l, 32, file.c_str()); *value->value = atof(value_l);
	}

	for (auto value : bools) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "false", value_l, 32, file.c_str()); *value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();