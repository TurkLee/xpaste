#include "chams.hpp"
#include <fstream>

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../hooks.hpp"
#include "../helpers/input.hpp"
#include "animfixed shit.h"

Chams::Chams()
{
	//materialRegular = g_MatSystem->FindMaterial("debug/debugambientcube", "Model textures");
	KeyValues* normalKV = new KeyValues("chamsNormal");
	KeyValues* elixirKV = new KeyValues("elixirNormal");

	normalKV->LoadFromBuffer(normalKV, "chamsNormal", "VertexLitGeneric { }");
	elixirKV->LoadFromBuffer(elixirKV, "elixirNormal", R"#("VertexLitGeneric"
{
    "$basetexture" "VGUI/white_additive"
        "$bumpmap" "de_nuke/hr_nuke/pool_water_normals_002"
        "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
        "$ignorez" "0"
        "$nofog" "1"
        "$model" "1"
        "$color2" "[1.0, 1.0, 1.0]"
        "$halflambert" "1"
        "$envmap" "env_cubemap"
        "$envmaptint" "[0.1 0.1 0.1]"
        "$envmapfresnel" "1.0"
        "$envmapfresnelminmaxexp" "[1.0, 1.0, 1.0]"
        "$phong" "1"
        "$phongexponent" "1024"
        "$phongboost" "4.0"
        "$phongfresnelranges" "[1.0, 1.0, 1.0]"
        "$rimlight" "1"
        "$rimlightexponent" "4.0"
        "$rimlightboost" "2.0"
  "Proxies"
        {
            "TextureScroll"
            {
                "textureScrollVar" "$bumptransform"
                "textureScrollRate" "0.25"
                "textureScrollAngle" "0.0"
            }
        }
}
)#");

	materialRegular = g_MatSystem->CreateMaterial("chamsNormal", normalKV);
	materialRegular->IncrementReferenceCount();
	materialElixir = g_MatSystem->CreateMaterial("elixirNormal", elixirKV);
	materialElixir->IncrementReferenceCount();
	materialFlat = g_MatSystem->FindMaterial("debug/debugdrawflat", "Model textures");
	materialGold = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
	materialGlass = g_MatSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	materialGloss = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
	materialCrystal = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
	materialCrystalBlue = g_MatSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER);
	materialSilver = g_MatSystem->FindMaterial("models/inventory_items/hydra_crystal/hydra_crystal_detail", TEXTURE_GROUP_OTHER);
	materialGlow = g_MatSystem->FindMaterial("dev/glow_armsrace", TEXTURE_GROUP_MODEL);
}

Chams::~Chams()
{
}

void Chams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba, bool modulate, bool gold, bool crystal, bool crystal2, bool silver, bool elixir)
{
	IMaterial* material = nullptr;

	if (modulate) {
		if (flat)
			material = materialFlat;
		else if (gold)
			material = materialGold;
		else if (crystal)
			material = materialCrystal;
		else if (glass)
			material = materialGlass;
		else if (crystal2)
			material = materialCrystalBlue;
		else if (silver)
			material = materialSilver;
		else if (elixir)
			material = materialElixir;
		else
			material = materialRegular;
	}

	bool bFound = false;
	IMaterialVar* pMatVar = material->FindVar("$envmaptint", &bFound);
	//if (bFound)
	//	pMatVar->SetVecValue(rgba.r() / 255.f, rgba.g() / 255.f, rgba.b() / 255.f);

	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	material->AlphaModulate(
		rgba.a() / 255.0f);

	material->IncrementReferenceCount();

	g_MdlRender->ForcedMaterialOverride(material);
}


void Chams::OnDrawModelExecute(
	IMatRenderContext* ctx,
	const DrawModelState_t& state,
	const ModelRenderInfo_t& info,
	matrix3x4_t* matrix)
{
	static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

	const auto mdl = info.pModel;

	bool is_arm = strstr(mdl->szName, "arms") != nullptr;
	bool is_player = strstr(mdl->szName, "models/player/") != nullptr;
	bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
	bool is_weapon = strstr(mdl->szName, "weapons/v_") != nullptr;

	//if (is_player && g_Options.chams_player_enabled) {
	if (std::strstr(mdl->szName, "models/player/")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_sas.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_sas_variantf.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_fbi_variantb.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_fbi_variantf.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_fbi_variantg.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_fbi_varianth.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_st6_variantg.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_st6_variante.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_st6_varianti.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_st6_variantk.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/ctm_st6_variantm.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_leet_variantf.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_leet_variantg.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_leet_varianth.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_leet_varianti.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_phoenix.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_phoenix_variantf.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_phoenix_variantg.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_phoenix_varianth.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_balkan_variantf.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_balkan_variantg.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_balkan_varianth.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_balkan_varianti.mdl")
		|| std::strstr(mdl->szName, "models/player/custom_player/legacy/tm_balkan_variantj.mdl")
		)
	{
		if (g_Options.chams_player_enabled)
		{
			// 
// Draw player Chams.
// 
			auto ent = C_BasePlayer::GetPlayerByIndex(info.entity_index);

			if (ent && ent->IsAlive() && !ent->IsDormant()) {
				C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
				C_BaseEntity* pEntity = state.m_pRenderable->GetIClientUnknown()->GetBaseEntity();
			}

			if (ent && g_LocalPlayer && ent->IsAlive()) {
				const auto clr_front = Color(g_Options.color_chams_player_visible);
				const auto clr_back = Color(g_Options.color_chams_player_occluded);

				const auto enemy = ent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();


				if (!enemy && g_Options.chams_player_enemies_only)
					return;




				if (!g_Options.chams_player_visibleonly) {
					OverrideMaterial(
						true,
						g_Options.chams_player_flat,
						g_Options.chams_player_wireframe,
						false,
						clr_back);


					fnDME(g_MdlRender, 0, ctx, state, info, matrix);

					g_MdlRender->ForcedMaterialOverride(nullptr);

					OverrideMaterial(
						false,
						g_Options.chams_player_flat,
						g_Options.chams_player_wireframe,
						false,
						clr_front);
				}
				else {
					OverrideMaterial(
						false,
						g_Options.chams_player_flat,
						g_Options.chams_player_wireframe,
						g_Options.chams_player_glass,
						clr_front);
				}
			}
		}
	}

	else if (is_sleeve && g_Options.chams_sleeves_enabled) {

		OverrideMaterial(
			false,
			g_Options.chams_sleeves_mat == 1,
			g_Options.chams_sleeves_wireframe,
			g_Options.chams_sleeves_mat == 3,
			Color(g_Options.chams_sleeves_color),
			true,
			g_Options.chams_sleeves_mat == 4,
			g_Options.chams_sleeves_mat == 2,
			g_Options.chams_sleeves_mat == 5,
			g_Options.chams_sleeves_mat == 6,
			g_Options.chams_sleeves_mat == 7

		);
		if (g_Options.chams_sleeves_reflect) {
			fnDME(g_MdlRender, 0, ctx, state, info, matrix);

			g_MdlRender->ForcedMaterialOverride(nullptr);

			g_MdlRender->ForcedMaterialOverride(materialGloss);
		}
	}
	else if (!is_arm && !is_sleeve && is_weapon && g_Options.chams_weapon_enabled)
	{
		OverrideMaterial(
			false,
			g_Options.chams_weapon_mat == 1,
			g_Options.chams_weapon_wireframe,
			g_Options.chams_weapon_mat == 3,
			Color(g_Options.chams_weapon_color),
			true,
			g_Options.chams_weapon_mat == 4,
			g_Options.chams_weapon_mat == 2,
			g_Options.chams_weapon_mat == 5,
			g_Options.chams_weapon_mat == 6,
			g_Options.chams_weapon_mat == 7

		);
		if (g_Options.chams_weapon_reflect) {
			fnDME(g_MdlRender, 0, ctx, state, info, matrix);

			g_MdlRender->ForcedMaterialOverride(nullptr);

			g_MdlRender->ForcedMaterialOverride(materialGloss);
		}

	}
	else if (is_arm && !is_sleeve) {
		auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		if (!material)
			return;
		if (g_Options.other_no_hands) {
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_MdlRender->ForcedMaterialOverride(material);
		}
		else if (g_Options.chams_arms_enabled) {

			OverrideMaterial(
				false,
				g_Options.chams_arms_mat == 1,
				g_Options.chams_arms_wireframe,
				g_Options.chams_arms_mat == 3,
				Color(g_Options.chams_arms_color),
				true,
				g_Options.chams_arms_mat == 4,
				g_Options.chams_arms_mat == 2,
				g_Options.chams_arms_mat == 5,
				g_Options.chams_arms_mat == 6,
				g_Options.chams_arms_mat == 7

			);
			if (g_Options.chams_arms_reflect) {
				fnDME(g_MdlRender, 0, ctx, state, info, matrix);

				g_MdlRender->ForcedMaterialOverride(nullptr);

				g_MdlRender->ForcedMaterialOverride(materialGloss);
			}
		}
	}
}