#define NOMINMAX
#include <Windows.h>
#include "helpers/config.hpp"
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "features/radar.hpp"
#include "helpers/input.hpp"
#include "Korumalar/xorstr.h"
#include "BASS/API.h"
#include "hooks.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "render.hpp"
#include <chrono>
#include <thread>
#include <Wininet.h>
#include <string>
#pragma comment(lib, "wininet.lib")
#include "Korumalar/Atakan75p.h" 
#include "features/lazy_importer.hpp"
#include "features/skin/kit_parser.h"

void* saved_hwnd = nullptr;
using namespace std;
static int radio_muted = GetKeyState(g_Options.radiomute);

void swap_hwnd() {
	if (Menu::Get().IsVisible())
	{
		if (!saved_hwnd)
			swap(saved_hwnd, g_InputSystem->GetWindow());
	}
	else if (saved_hwnd)
		swap(saved_hwnd, g_InputSystem->GetWindow());
}


void autoaccept();

DWORD WINAPI OnDllAttach(LPVOID base)
{
	while (!GetModuleHandleA("serverbrowser.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try {
		Config->Setup();
		Utils::ConsolePrint(_xor_("Initializing...\n").c_str());

		Interfaces::Initialize();
		Utils::ConsolePrint(_xor_("Interfaces Initialized\n").c_str());

		Interfaces::Dump();
		Utils::ConsolePrint(_xor_("Interfaces Dumped\n").c_str());

		InputSys::Get().Initialize();
		Utils::ConsolePrint(_xor_("InputSys Initialized\n").c_str());

		NetvarSys::Get().Initialize();
		Utils::ConsolePrint(_xor_("NetvarSys Initialized\n").c_str());

		Render::Get().Initialize();
		Utils::ConsolePrint(_xor_("Render Initialized\n").c_str());

		Menu::Get().Initialize();
		Utils::ConsolePrint(_xor_("Menu Initialized\n").c_str());

		Hooks::Initialize();
		Utils::ConsolePrint(_xor_("Hooks Initialized\n").c_str());

		initialize_kits();
		Utils::ConsolePrint(_xor_("Kits Initialized\n").c_str());

		BASS::bass_lib_handle = BASS::bass_lib.LoadFromMemory(bass_dll_image, sizeof(bass_dll_image));

		if (BASS_INIT_ONCE())
			BASS::bass_init = TRUE;

		static auto bass_needs_reinit = false;

		static std::string channels[] = {
			(" "),
			("http://www.rockantenne.de/webradio/channels/alternative.m3u"),
			("http://listen.technobase.fm/dsl.pls"),
			("http://streams.bigfm.de/bigfm-deutschrap-128-aac"),
			("http://mp3.stream.tb-group.fm/tt.mp3"),
			("http://mp3.stream.tb-group.fm/clt.mp3"),
			("http://mp3.stream.tb-group.fm/ht.mp3"),
			("http://www.iloveradio.de/iloveradio.m3u"),
			("http://8bit.fm:8000/live"),
			("http://air.radiorecord.ru:8101/rr_320"),
			("https://dfm.hostingradio.ru/dfm96.aacp"),
			("http://ic2.101.ru:8000/v1_1"),
			("http://retroserver.streamr.ru:8043/retro256.mp3"),
			("http://dorognoe.hostingradio.ru:8000/radio"),
			("http://nashe1.hostingradio.ru/nashespb128.mp3"),
			("http://ic4.101.ru:8000/a219"),
			("http://ep256.hostingradio.ru:8052/europaplus256.mp3")

		};

		InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
			g_EngineClient->ExecuteClientCmd("toggleconsole");
			Menu::Get().Toggle();
			swap_hwnd();

			});

		Utils::ConsolePrint("Finished.\n");





		if (g_Options.misc_autoaccept)
			autoaccept();

		const auto desired_channel = g_Options.radiochannel;

		if (BASS::bass_init && desired_channel)
		{
			static auto current_channel = 0;

			if (current_channel != desired_channel || bass_needs_reinit)
			{
				bass_needs_reinit = false;
				BASS_Start();
				auto channel = channels[desired_channel];
				BASS_OPEN_STREAM(channel.c_str());
				current_channel = desired_channel;
			}

			BASS_SET_VOLUME(BASS::stream_handle, radio_muted ? 0.f : g_Options.radiovolume / 100.f);
			BASS_PLAY_STREAM();
		}
		else if (BASS::bass_init)
		{
			bass_needs_reinit = true;
			BASS_StreamFree(BASS::stream_handle);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
	catch (const std::exception& ex) {
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::ConsolePrint("Press any key to exit.\n");
		Utils::ConsoleReadKey();
		Utils::DetachConsole();

		FreeLibraryAndExitThread((HMODULE)base, 0);
	}
	return false;

}

BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif

	return TRUE;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDll, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDll);
		CreateThread(nullptr, NULL, OnDllAttach, hinstDll, NULL, nullptr);
		return TRUE;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return OnDllDetach();
		return TRUE;
	default:
		return TRUE;
	}
}