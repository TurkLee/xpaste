#include "../hooks.hpp"
#include "../features/chams.hpp"
#include "../features/Misc.h"
#include "../options.hpp"
#include "../features/animfixed shit.h"
extern matrix3x4_t m_real_matrix[128];
extern bool m_got_real_matrix;

void __fastcall Hooks::hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	static auto ofunc = mdlrender_hook.get_original<decltype(&hkDrawModelExecute)>(index::DrawModelExecute);

	if (Misc::Get().RemoveSleeves(pInfo.pModel->szName))
		return;

	if (g_MdlRender->IsForcedMaterialOverride() &&
		!strstr(pInfo.pModel->szName, "arms") &&
		!strstr(pInfo.pModel->szName, "weapons/v_")) {
		return ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
	}
	Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

	ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
	g_MdlRender->ForcedMaterialOverride(nullptr);
}