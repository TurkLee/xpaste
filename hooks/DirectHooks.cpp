#include "DirectHooks.h"
#include "../notification/notifications.hpp"
#include "../options.hpp"

long __stdcall Hooks::hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static auto _reset = direct_hook.get_original<decltype(&hkReset)>(index::Reset);
	Menu::Get().OnDeviceLost();
	auto hr = _reset(device, pPresentationParameters);
	if (hr >= 0) Menu::Get().OnDeviceReset();
	return hr;
}

long __stdcall Hooks::hkEndScene(IDirect3DDevice9* pDevice)
{
	static auto _end_scene = direct_hook.get_original<decltype(&hkEndScene)>(index::EndScene);

	static auto viewmodel_fov = g_CVar->FindVar("viewmodel_fov");

	viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
	viewmodel_fov->SetValue(g_Options.misc_viewmodel_fov);
	static uintptr_t gameoverlay_return_address = 0;
	if (!gameoverlay_return_address) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

		char mod[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

		if (strstr(mod, "gameoverlay"))
			gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
	}

	if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress()) && g_Options.misc_anti_obs)
		return _end_scene(pDevice);

	DWORD colorwrite, srgbwrite;
	IDirect3DVertexDeclaration9* vert_dec = nullptr;
	IDirect3DVertexShader9* vert_shader = nullptr;
	DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	//removes the source engine color correction
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->GetVertexDeclaration(&vert_dec);
	pDevice->GetVertexShader(&vert_shader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();



	ImDrawList* esp_drawlist = nullptr;
	esp_drawlist = Render::Get().RenderScene();

	Menu::Get().Render();
	g_Notification.Draw();

	//Render::Get().BeginScene();


	ImGui::Render(esp_drawlist);

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	pDevice->SetVertexDeclaration(vert_dec);
	pDevice->SetVertexShader(vert_shader);

	return _end_scene(pDevice);
}