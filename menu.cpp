#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define NOMINMAX
#include <thread>
#include <Windows.h>
#include <chrono>
#include "Menu.hpp"
#pragma comment(lib, "Wininet")
#include <WinInet.h>
#include <experimental/filesystem>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"
#include "helpers/config.hpp"
#include "features/Misc.h"
#include "render.hpp"
#include <experimental/filesystem>
#include "Korumalar/AtakanınAnnesi.h"
#include "hooks.hpp"
#include <windows.h>
#include <string>
#include <cstdio>
#pragma comment(lib,"advapi32.lib")
#include "BASS/bass.h"
#include "Korumalar/xorstr.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "features/visuals.hpp"
#include "features/skin/item_definitions.h"
#include "features/nightmode.hpp"

#include "notification/notifications.hpp"	
#include "Logger/event_logger.hpp"
#include "features/skin/kit_parser.h"
#include "features/skin/skins.h"
#include "Korumalar/Atakan75.hpp"


#include <windows.h>
#include <string>
#include <cstdio>
#pragma comment(lib,"advapi32.lib")

const char* customModelsct[] =
{
"off",
"Christmas Miku",
"Harry Potter",
"Pickle Rick",
"Santa",
"Hitman",
"Batman",
"Assasin",
"R6S Ash",
"Doomguy",
"Putin",
"Skeleton",
"Slime",
"Spiderman",
};

const char* customModelst[] =
{
"off",
"Christmas Miku",
"Harry Potter",
"Pickle Rick",
"Santa",
"Hitman",
"Batman",
"Assasin",
"R6S Ash",
"Doomguy",
"Putin",
"Skeleton",
"Slime",
"Spiderman"
};

const char* DZRank[] = {
   "Ignored",
   "Lab Rat I",
   "Lab Rat II",
   "Sprinting Hare I",
   "Sprinting Hare II",
   "Wild Scout I",
   "Wild Scout II",
   "Wild Scout Elite",
   "Hunter Fox I",
   "Hunter Fox II",
   "Hunter Fox III",
   "Hunter Fox Elite",
   "Timber Wolf",
   "Ember Wolf",
   "Wildfire Wolf",
   "The Howling Alpha"
};

const char* MMRank[] = {
   "Ignored",
   "Silver I",
   "Silver II",
   "Silver III",
   "Silver IV",
   "Silver Elite",
   "Silver Elite Master",
   "Gold Nova I",
   "Gold Nova II",
   "Gold Nova III",
   "Gold Nova Master",
   "Master Guardian I",
   "Master Guardian II",
   "Master Guardian Elite",
   "Distinguished Master Guardian",
   "Legendary Eagle",
   "Legendary Eagle Master",
   "Supreme Master First Class",
   "The Global Elite"
};

static const char* bans[] =
{
	"none",
	"you were kicked from the last match (competitive cooldown)",
	"you killed too many teammates (competitive cooldown)",
	"you killed a teammate at round start (competitive cooldown)",
	"you failed to reconnect to the last match (competitive cooldown)",
	"you abandoned the last match (competitive cooldown)",
	"you did too much damage to your teammates (competitive cooldown)",
	"you did too much damage to your teammates at round start (competitive cooldown)",
	"this account is permanently untrusted (global cooldown)",
	"you were kicked from too many recent matches (competitive cooldown)",
	"convicted by overwatch - majorly disruptive (global cooldown)",
	"convicted by overwatch - minorly disruptive (global cooldown)",
	"resolving matchmaking state for your account (temporary cooldown)",
	"resolving matchmaking state after the last match (temporary cooldown)",
	"this account is permanently untrusted (global cooldown)",
	"(global cooldown)",
	"you failed to connect by match start. (competitive cooldown)",
	"you have kicked too many teammates in recent matches (competitive cooldown)",
	"congratulations on your recent competitive wins! before you play competitive matches further please wait for matchmaking servers to calibrate your skill group placement based on your lastest performance. (temporary cooldown)",
	"a server using your game server login token has been banned. your account is now permanently banned from operating game servers, and you have a cooldown from connecting to game servers. (global cooldown)"
};

const char* skyboxes[] = {
	(u8"Normal Gece Modu"),
	(u8"Uzay Güneşi"),
	(u8"Uzaydan Dünya"),
	(u8"Okyanus Altı Uzay"),
	(u8"Dumanlı Uzay"),
	(u8"Galaksiler Arası"),
	(u8"Yıldızlar"),
	(u8"Uzaydan Dünya (gece)"),
	(u8"Spektrum"),
	(u8"Mavi Dünya"),
	(u8"Galaksiler Arası")
};
const char* AntiaimPitchNames[] =
{
("None"),
("Down"),
("Up"),
("Dance"),
("Fake Up"),
("Fake Down"),
("Random")
};

const char* AntiaimYawNames[] =
{
("None"),
("Spin"),
("Static Forward"),
("Static Right"),
("Static Backwards"),
("Static Left"),
("Backwards"),
("Left"),
("Right"),
("Side"),
("Fake LBY 1"),
("Fake LBY 2"),
("Jitter"),
("Backjitter"),
("Fake Side LBY"),
("Random")
};
bool OwOButtonXD(const char* label, const ImVec2& size_arg, bool inuse, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);


	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemEdited(id);
	// Render
	const ImU32 col = ImGui::GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	//RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);


	auto turuncu = ImColor(114, 137, 218);
	auto koyuturuncu = ImColor(114, 137, 218, 230);
	if (inuse)
	{

		window->DrawList->AddRectFilledMultiColor(ImVec2(bb.Min.x, bb.Max.y - 3), ImVec2(bb.Max.x, bb.Max.y - 0), koyuturuncu, koyuturuncu, koyuturuncu, koyuturuncu);
	}
	else
	{
		ImGui::RenderNavHighlight(bb, id);
		window->DrawList->AddRectFilledMultiColor(ImVec2(bb.Min.x, bb.Max.y - 1), ImVec2(bb.Max.x, bb.Max.y - 0), turuncu, turuncu, turuncu, turuncu);
	}



	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();
	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);

	return pressed;
}
bool OwOButton(const char* label, bool inuse, const ImVec2& size_arg)
{
	return OwOButtonXD(label, size_arg, inuse, 0);
}
std::string GetMachineGUID()
{
	std::string ret;
	char value[64];
	DWORD size = _countof(value);
	DWORD type = REG_SZ;
	HKEY key;
	LONG retKey = ::RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &key);
	LONG retVal = ::RegQueryValueExA(key, "MachineGuid", nullptr, &type, (LPBYTE)value, &size);
	if (retKey == ERROR_SUCCESS && retVal == ERROR_SUCCESS) {
		ret = value;
	}
	::RegCloseKey(key);
	return ret;
}

static std::string guid = GetMachineGUID();

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadString(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}


namespace Preview
{
	std::string FindUrl(std::string paintName, std::string weapon)
	{
		std::ifstream inFile;
		inFile.open(".\\csgo\\scripts\\items\\items_game_cdn.txt");
		std::string line;
		std::string search = paintName.append("=");
		while (std::getline(inFile, line))
		{
			if (line.find(search, 0) != std::string::npos)
			{
				if (line.find(weapon, 0) != std::string::npos)
				{
					return line.substr(line.find(search)).erase(0, search.length());
				}
			}
		}
		return "nf";
	}

	std::string DownloadBytes(const char* const szUrl)
	{
		HINTERNET hOpen = NULL;
		HINTERNET hFile = NULL;
		char* lpBuffer = NULL;
		DWORD dwBytesRead = 0;
		char* data = 0;
		DWORD dataSize = 0;

		hOpen = InternetOpenA("Xhanger", NULL, NULL, NULL, NULL);
		if (!hOpen) return "";

		hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD, NULL);

		if (!hFile) {
			InternetCloseHandle(hOpen);
			return "";
		}

		std::string output;
		do {
			char buffer[2000];
			InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
			output.append(buffer, dwBytesRead);
		} while (dwBytesRead);

		InternetCloseHandle(hFile);
		InternetCloseHandle(hOpen);

		return output;
	}
}
std::string imData;
std::string linklenmk = "";
void downloadImage()
{
	imData = Preview::DownloadBytes(linklenmk.c_str());
	D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, imData.data(), imData.length(), 512, 384, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &profil);
}

bool resimayarla = true;
namespace fs = std::experimental::filesystem;
static std::string selected_weapon_name = "";
static std::string selected_skin_name = "";
static auto definition_vector_index = 0;
IDirect3DTexture9* m_skin_texture = nullptr;

const char* ___(const char* english, const char* turkish)
{
	return g_Options.menu_language ? turkish : english;
}

const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};

static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

bool KeyBind(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0))
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);
	//const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	//const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (IsKeyPressedMap(ImGuiKey_Escape)) {
			k = 0;
			ImGui::ClearActiveID();
		}
		else {
			k = key;
		}
	}



	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = u8"Yok";
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.50f, 0.50f, 0.50f, 1.00f));

	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_FrameBg]), true, style.FrameRounding);
	ImGui::PopStyleColor();
	if (k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, KeyNames[k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, _c(u8"<Bir tuşa basınız>"));
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Max.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}
static char* boxes[] = { "Off", "Regular", "Edge", "Rounded" };

Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

	*viewCheck = y_1 < 0;

	x_1 *= zoom;
	y_1 *= zoom;

	int sizX = sizeX / 2;
	int sizY = sizeY / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;


	x_1 += posX;
	y_1 += posY;


	return Vector(x_1, y_1, 0);
}

void RadarRender()
{
	if (ImGui::Begin("Radar", &g_Options.Act, ImVec2(200, 200), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddLine(ImVec2(pos.x + siz.x * 0.5f, pos.y), ImVec2(pos.x + siz.x * 0.5f, pos.y + siz.y), ImColor(70, 70, 70, 255), 1.f);
		draw_list->AddLine(ImVec2(pos.x, pos.y + siz.y * 0.5f), ImVec2(pos.x + siz.x, pos.y + siz.y * 0.5f), ImColor(70, 70, 70, 255), 1.f);

		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
		{
			auto pLocalEntity = g_LocalPlayer;
			if (pLocalEntity)
			{
				Vector LocalPos = pLocalEntity->GetEyePos();
				QAngle ang;
				g_EngineClient->GetViewAngles(&ang);
				for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
				{
					C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

					if (!pBaseEntity)
						continue;
					if (pBaseEntity->IsDormant() || !pBaseEntity->IsAlive())
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;

					bool bIsEnemy = pLocalEntity->m_iTeamNum() != pBaseEntity->m_iTeamNum();
					if (!bIsEnemy)
						continue;

					bool viewCheck = false;
					Vector EntityPos = RotatePoint(pBaseEntity->m_vecOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.yaw, g_Options.Zoom, &viewCheck, 0.F);

					int s = g_Options.Size;
					switch (g_Options.RadarStyle)
					{
					case 0:
					{
						draw_list->AddRect(ImVec2(EntityPos.x - s, EntityPos.y - s), ImVec2(EntityPos.x + s, EntityPos.y + s), ImColor(254, 24, 110, 255));
						break;
					}
					case 1:
					{
						draw_list->AddRectFilled(ImVec2(EntityPos.x - s, EntityPos.y - s), ImVec2(EntityPos.x + s, EntityPos.y + s), ImColor(254, 24, 110, 255));
						break;
					}
					case 2:
					{
						draw_list->AddCircle(ImVec2(EntityPos.x, EntityPos.y), s, ImColor(254, 24, 110, 255));
						break;
					}
					case 3:
					{
						draw_list->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, ImColor(254, 24, 110, 255));
						break;
					}
					default:
						break;
					}
				}
			}
		}
	}
	ImGui::End();
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan(GetModuleHandleA("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h)
{
	bool values[N] = { false };
	values[activetab] = true;
	for (auto i = 0; i < N; ++i) {
		if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) activetab = i;
		if (i < N - 1) ImGui::SameLine();
	}
}
std::string zamans;
std::string png = ".php";
std::string kadi;
std::string koruma;
std::string suress;

std::string bir;

ImVec2 main_pos;
ImVec2 main_size;
void Menu::Initialize()
{
	CreateStyle();
	std::string link3 = _c("https://xhangeryazilim.com/koruma.php?hwid=") + guid;

	koruma = DownloadString(link3.c_str());

	std::string link4 = _c("https://xhangeryazilim.com/1.php");

	bir = DownloadString(link4.c_str());

	std::string link1 = _c("https://xhangeryazilim.com/xhanger.php?hwid=") + guid;

	kadi = DownloadString(link1.c_str());


	std::string link123 = _c("https://xhangeryazilim.com/hack/surekontrol.php?login=") + kadi;

	suress = DownloadString(link123.c_str());

	std::string link2 = _c("https://xhangeryazilim.com/aksebraym.php?login=") + kadi;

	zamans = DownloadString(link2.c_str());

	linklenmk = _c("https://xhangeryazilim.com/foto2.php?login=") + kadi;

	_visible = false;
}

void Menu::Shutdown()
{
	_visible = false;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void RenderRadio() {
	if (strlen(BASS::bass_metadata) > 0 && g_Options.radiochannel) {

		ImGui::Begin(u8"Şuan Çalıyor", nullptr, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text(g_Options.radiomute ? "Durduruldu" : BASS::bass_metadata);
		ImGui::End();
	}
}

class ColorXd
{
public:

	ColorXd() {
		SetColor(0, 0, 0, 255);
	}

	ColorXd(uint8_t r, uint8_t g, uint8_t b) {
		SetColor(r, g, b, 255);
	}

	ColorXd(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		SetColor(r, g, b, a);
	}

	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		_color[0] = (uint8_t)r;
		_color[1] = (uint8_t)g;
		_color[2] = (uint8_t)b;
		_color[3] = (uint8_t)a;
	}

	void GetColor(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const {
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	uint8_t& operator[](int index) {
		return _color[index];
	}

	const uint8_t& operator[](int index) const {
		return _color[index];
	}

	bool operator == (const Color& rhs) const {
		return (*((int*)this) == *((int*)&rhs));
	}

	bool operator != (const Color& rhs) const {
		return !(operator==(rhs));
	}

	int r() { return _color[0]; }
	int g() { return _color[1]; }
	int b() { return _color[2]; }
	int a() { return _color[3]; }

	int GetR() { return _color[0]; }
	int GetG() { return _color[1]; }
	int GetB() { return _color[2]; }
	int GetA() { return _color[3]; }

	void SetR(uint8_t _i) { _color[0] = _i; }
	void SetG(uint8_t _i) { _color[1] = _i; }
	void SetB(uint8_t _i) { _color[2] = _i; }
	void SetA(uint8_t _i) { _color[3] = _i; }

	ColorXd& operator=(const ColorXd& rhs) {
		*(int*)(&_color[0]) = *(int*)&rhs._color[0];
		return *this;
	}

	ColorXd operator+(const ColorXd& rhs) const {
		int red = _color[0] + rhs._color[0];
		int green = _color[1] + rhs._color[1];
		int blue = _color[2] + rhs._color[2];
		int alpha = _color[3] + rhs._color[3];

		red = red > 255 ? 255 : red;
		green = green > 255 ? 255 : green;
		blue = blue > 255 ? 255 : blue;
		alpha = alpha > 255 ? 255 : alpha;

		return ColorXd(red, green, blue, alpha);
	}

	ColorXd operator-(const ColorXd& rhs) const {
		int red = _color[0] - rhs._color[0];
		int green = _color[1] - rhs._color[1];
		int blue = _color[2] - rhs._color[2];
		int alpha = _color[3] - rhs._color[3];

		red = red < 0 ? 0 : red;
		green = green < 0 ? 0 : green;
		blue = blue < 0 ? 0 : blue;
		alpha = alpha < 0 ? 0 : alpha;
		return ColorXd(red, green, blue, alpha);
	}

	operator const uint8_t* () {
		return (uint8_t*)(&_color[0]);
	}

	static ColorXd FromHSB(float hue, float saturation, float brightness)
	{

		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return ColorXd(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return ColorXd(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return ColorXd(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return ColorXd(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return ColorXd(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return ColorXd(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	ImU32 GetU32()
	{
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8) + (_color[0] & 0xff);
	}

private:
	uint8_t _color[4];
};

void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float& flRainbow)
{
	ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

	ColorXd colColor(0, 0, 0, 255);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		ColorXd colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
	}
}

void background()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
	static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

	ImGui::Begin("##background", nullptr, flags);
	static float flRainbow;
	float flSpeed = 0.0003f;
	int curWidth = 1;
	ImVec2 curPos = ImGui::GetCursorPos();
	ImVec2 curWindowPos = ImGui::GetWindowPos();
	curPos.x += curWindowPos.x;
	curPos.y += curWindowPos.y;
	int size;
	int y;
	g_EngineClient->GetScreenSize(y, size);
	DrawRectRainbow(curPos.x - 10, curPos.y - 5, ImGui::GetWindowSize().x + size, curPos.y + -8, flSpeed, flRainbow);
	ImGui::End();
	ImGui::PopStyleColor();
}
static int weapon_index = 7;
static int weapon_vector_index = 0;
struct WeaponName_t {
	constexpr WeaponName_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};

std::vector< WeaponName_t> WeaponNames =
{
{ WEAPON_AK47, "AK-47" },
{ WEAPON_AUG, "AUG" },
{ WEAPON_AWP, "AWP" },
{ WEAPON_CZ75A, "CZ75 Auto" },
{ WEAPON_DEAGLE, "Desert Eagle" },
{ WEAPON_ELITE, "Dual Berettas" },
{ WEAPON_FAMAS, "FAMAS" },
{ WEAPON_FIVESEVEN, "Five-SeveN" },
{ WEAPON_G3SG1, "G3SG1" },
{ WEAPON_GALILAR, "Galil AR" },
{ WEAPON_GLOCK, "Glock-18" },
{ WEAPON_M249, "M249" },
{ WEAPON_M4A1_SILENCER, "M4A1-S" },
{ WEAPON_M4A1, "M4A4" },
{ WEAPON_MAC10, "MAC-10" },
{ WEAPON_MAG7, "MAG-7" },
{ WEAPON_MP7, "MP7" },
{ WEAPON_MP5, "MP5" },
{ WEAPON_MP9, "MP9" },
{ WEAPON_NEGEV, "Negev" },
{ WEAPON_NOVA, "Nova" },
{ WEAPON_HKP2000, "P2000" },
{ WEAPON_P250, "P250" },
{ WEAPON_P90, "P90" },
{ WEAPON_BIZON, "PP-Bizon" },
{ WEAPON_REVOLVER, "R8 Revolver" },
{ WEAPON_SAWEDOFF, "Sawed-Off" },
{ WEAPON_SCAR20, "SCAR-20" },
{ WEAPON_SSG08, "SSG 08" },
{ WEAPON_SG556, "SG 553" },
{ WEAPON_TEC9, "Tec-9" },
{ WEAPON_UMP45, "UMP-45" },
{ WEAPON_USP_SILENCER, "USP-S" },
{ WEAPON_XM1014, "XM1014" },
};
static int settings_weap;

void RenderCurrentWeaponButton()
{
	if (!g_EngineClient->IsConnected() || !g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;
	auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon)
		return;
	if (!g_Options.use_all_weapons) {
		if (ImGui::Button("Current")) {
			short wpn_idx = weapon->m_Item().m_iItemDefinitionIndex();
			auto wpn_it = std::find_if(WeaponNames.begin(), WeaponNames.end(), [wpn_idx](const WeaponName_t& a) {
				return a.definition_index == wpn_idx;
				});
			if (wpn_it != WeaponNames.end()) {
				weapon_index = wpn_idx;
				weapon_vector_index = std::abs(std::distance(WeaponNames.begin(), wpn_it));
			}
		}
	}
	else
	{
		short wpn_idx = weapon->m_Item().m_iItemDefinitionIndex();
		auto wpn_it = std::find_if(WeaponNames.begin(), WeaponNames.end(), [wpn_idx](const WeaponName_t& a) {
			return a.definition_index == wpn_idx;
			});
		if (wpn_it != WeaponNames.end()) {
			weapon_index = wpn_idx;
			weapon_vector_index = std::abs(std::distance(WeaponNames.begin(), wpn_it));
		}
	}

}
static char* legit_tabs[] =
{
	u8"Tümü",
	u8"Grup",
	u8"Silah"
};
static char* weapon_groups_array[] =
{
u8"zeus",
u8"Tabanca",
u8"SMG",
u8"Pompalı",
u8"Ağır",
u8"Tüfek",
u8"Sniper",
u8"Bıçak"
};
void Menu::Render()
{
	aimbot_settings* settings;

	ImGui::GetIO().MouseDrawCursor = _visible;

	static bool once{ false };
	if (!once) {
		g_Notification.Push(_c("xhangeryazilim.com"), _c(u8"Tekrardan Hoşgeldiniz ;)"));
		once = true;
	}

	if (g_Options.misc_rainbowbar)
		background();

	if (g_Options.esp_speclist)
		Visuals::Get().RenderSpectatorList();

	if (g_Options.Act)
		RadarRender();

	RenderRadio();

	if (!_visible)
		return;

	static int selected_Tab = 0;
	static int MenuSelect = 0;
	static int ayimbottab = 0;
	static int esp_tab2 = 0;

	static std::string old_name_skin = "";
	static std::string old_name_weap = "";

	const char* c1 = suress.c_str();

	const std::string d1 = bir.c_str();

	static bool oncexD{ false };
	if (!oncexD) {
		notify::screen::notify(_c("Xhanger"), _c(u8"Hoşgeldin ") + kadi, Color::Red);
		oncexD = true;
	}
	char* chamstypes[] = { ("Material"), ("Flat"), ("Glass"), ("Crystal"), ("Gold"), ("Dark Crystal"), ("Silver"), ("Elixir") };
	char* chatspamtype[] = { ("Off"), ("Xhanger"), ("Custom") };

	const char* Hitboxx[] =
	{
		___(_c(u8"Lütfen Hitbox Seçiniz"), _c("Please Select Hitbox")),
		___(_c(u8"Kafa"), _c("Head")),
		___(_c(u8"Boyun"), _c("Neck")),
		___(_c(u8"Göğüs"), _c("Chest")),
		___(_c(u8"Mide"), _c("Stomach")),
		___(_c(u8"Çoklu Nokta"), _c("Multipoint"))
	};

	const char* items[] = { ___(_c(u8"Kapalı"), _c("Off")), ___(_c(u8"Statik"), _c("Static")), ___(_c(u8"Dengeli"), _c("Balance")) };

	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	float x = w * 0.5f - 300.0f;
	float y = h * 0.5f - 200.0f;

	ImGui::SetNextWindowPos(ImVec2{ x, y }, ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2{ 800, 600 }, ImGuiCond_Once);



	std::string tessst = _c("xhanger");
	static int AxeTab = 0;
	int _c = 0;
	static bool _csinshookwin = false;
	static int menus = 0;
	static int test = 0;
	static int btn = 5;
	static int   skin_tab = 0;
	main_pos = ImGui::GetWindowPos();
	main_size = ImGui::GetWindowSize();


	if (ImGui::Begin("1", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {

		const char* radio[] = {
					_c(u8"Kapalı"),
					_c("Post Melone Better Now"),
					_c("HIGHEST IN A ROOM"),
					_c("HvH"),
					_c("Teatime"),
					_c("Clubtime"),
					_c("Housetime"),
					_c("Iluvradio"),
					_c("8bit"),
					_c("DFM"),
					_c("Radio Energy"),
					_c("LikeFM")

		};
		const char* opt_Chams_Mat[] = { _c("Texture"),
_c("Texture-XQZ"),
_c("Gold-XQZ"),
_c("Crystal-XQZ"),
_c("Bubble-XQZ"),
_c("Black Chrome-XQZ"),
_c("Condom-XQZ"),
_c("Pulse-XQZ"),
_c("TEST XQZ"),
		};
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImGuiStyle& style = ImGui::GetStyle();

		//Content Oran = 180x75 ImGui::Dummy(180,75);
		//Butonlar = 85x50 
		if (g_Options.theme == false)
		{
			colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.82f, 0.82f, 0.82f, 0.94f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.67f, 0.67f, 0.67f, 0.98f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.39f, 0.39f, 0.97f);
			colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(1.00f, 0.00f, 0.00f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.31f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
			colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
			colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			if (menus == 0)
			{
				ImGui::GetWindowDrawList()->AddImage(rage, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 1)
			{
				ImGui::GetWindowDrawList()->AddImage(aimbot, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 2)
			{
				ImGui::GetWindowDrawList()->AddImage(visual, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 3)
			{
				ImGui::GetWindowDrawList()->AddImage(misc, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 4)
			{
				ImGui::GetWindowDrawList()->AddImage(env, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 5)
			{
				ImGui::GetWindowDrawList()->AddImage(settingss, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 6)
			{
				ImGui::GetWindowDrawList()->AddImage(gamewhite, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
		}
		else
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.87f, 0.85f, 0.92f, 0.58f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.27f, 0.20f, 0.39f, 0.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.87f, 0.85f, 0.92f, 0.30f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.20f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.68f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.46f, 0.27f, 0.80f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.34f, 0.19f, 0.63f, 0.45f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.34f, 0.19f, 0.63f, 0.35f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.20f, 0.39f, 0.57f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.20f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.19f, 0.63f, 0.31f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.34f, 0.19f, 0.63f, 0.80f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.19f, 0.63f, 0.24f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.34f, 0.19f, 0.63f, 0.44f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.86f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.34f, 0.19f, 0.63f, 0.76f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.86f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.87f, 0.85f, 0.92f, 0.32f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.87f, 0.85f, 0.92f, 0.78f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.87f, 0.85f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.34f, 0.19f, 0.63f, 0.20f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.34f, 0.19f, 0.63f, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.87f, 0.85f, 0.92f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.87f, 0.85f, 0.92f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.34f, 0.19f, 0.63f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.34f, 0.19f, 0.63f, 0.43f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
			if (menus == 0)
			{
				ImGui::GetWindowDrawList()->AddImage(rageblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 1)
			{
				ImGui::GetWindowDrawList()->AddImage(aimbotblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 2)
			{
				ImGui::GetWindowDrawList()->AddImage(visualblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 3)
			{
				ImGui::GetWindowDrawList()->AddImage(miscblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 4)
			{
				ImGui::GetWindowDrawList()->AddImage(envblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 5)
			{
				ImGui::GetWindowDrawList()->AddImage(settingssblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
			if (menus == 6)
			{
				ImGui::GetWindowDrawList()->AddImage(gameblack, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));

			}
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::BeginGroup();

		//Oran 1 - Rage = 280
		ImGui::SetCursorPos(ImVec2(0, 54));
		if (ImGui::Button(_c("##Rage"), ImVec2(225, 54))) menus = 0;
		ImGui::SetCursorPos(ImVec2(0, 107));
		if (ImGui::Button(_c("##Aimbot"), ImVec2(225, 54))) menus = 1;
		//Oran 2 - Aimbot = 380
		ImGui::SetCursorPos(ImVec2(0, 157));
		if (ImGui::Button(_c("##Visual"), ImVec2(225, 54))) menus = 2;
		//Oran 3 - Visual = 490
		ImGui::SetCursorPos(ImVec2(0, 210));
		if (ImGui::Button(_c(u8"##Diğer"), ImVec2(225, 54))) menus = 3;
		//Oran 4 - Diğer = 585
		ImGui::SetCursorPos(ImVec2(0, 267));
		if (ImGui::Button(_c("##Env"), ImVec2(225, 54))) menus = 4;
		//Oran 5 - Env. = 675
		ImGui::SetCursorPos(ImVec2(0, 325));
		if (ImGui::Button(_c("##Ayarlar"), ImVec2(225, 54))) menus = 5;
		//Oran 6 - Ayarlar = 755
		ImGui::SetCursorPos(ImVec2(0, 381));
		if (ImGui::Button(_c("##Game"), ImVec2(225, 54))) menus = 6;
		ImGui::EndGroup();
		//Child = 715x420
		ImGui::PopStyleColor(3);
		if (resimayarla)
		{
			resimayarla = false;
			std::thread download(downloadImage);
			download.detach();
			ImGui::SetCursorPos(ImVec2(30, 75));

		}
		const char* dttt[]
		{
			_c("Off"),
			_c("Onkey"),
			_c("Constant")
		};

		ImGui::SetCursorPos(ImVec2(5, 542));
		ImGui::Image(profil, ImVec2(45, 45));
		ImGui::SetCursorPos(ImVec2(70, 547));
		ImGui::Text(_c(u8"%s"), kadi.c_str());
		ImGui::SetCursorPos(ImVec2(70, 567));
		ImGui::Text(_c(u8"Kalan Süre : %s"), zamans.c_str());


		ImGui::SetCursorPos(ImVec2(244, 70));


		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 20);
		ImGui::BeginChild(_c("##Content"), ImVec2(576, 540));
		{
			if (menus == 0)
			{
				static int   ragebottabs = 0;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
				if (ImGui::OwOButton(u8"RAGE##tab", ragebottabs == 0 ? true : false, ImVec2(286, 50)))ragebottabs = 0;
				ImGui::SameLine(287);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"SİLAH##tab", ragebottabs == 1 ? true : false, ImVec2(286, 50)))ragebottabs = 1;

				ImGui::PopStyleVar();

				if (ragebottabs == 0)
				{
					if (g_Options.theme == false) {
						ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.90f, 0.90f, 0.9f, 1.00f));
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.447f, 0.537f, 0.855f, 1.000f));
						ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.447f, 0.537f, 0.855f, 1.000f));
					}
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 38);
					ImGui::BeginGroupBox("##rahebod", ImVec2(500, 470));
					{
						ImGui::Columns(2);



						ImGui::BeginChild("##2223");

						const char* priority[] = { _c("Head"),_c("Stomach"), _c("Pelvis") };
						ImGui::SetCursorPosX(5);
						ImGui::SetCursorPosY(20);
						ImGui::Checkbox(_c(u8"Aktif"), &g_Options.rage_enabled);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Oto-Durma"), &g_Options.rage_autostop);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Oto-R8"), &g_Options.rage_autor8);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Oto-Scope"), &g_Options.autoscope);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Resolver"), &g_Options.rage_resolver);
						ImGui::SetCursorPosX(5);
						ImGui::SliderInt(_c(u8"Baim After"), &g_Options.RageAimbotBaimAfter, 0, 10);
						ImGui::SetCursorPosX(6);
						ImGui::SliderFloat(_c(u8"Kafa Ölçeği"), &g_Options.HeadScale, 0.f, 100.f, "%1.f");
						ImGui::SetCursorPosX(6);
						ImGui::SliderFloat(_c(u8"Vücut Ölçeği"), &g_Options.BodyScale, 0.f, 100.f, "%1.f");
						ImGui::SetCursorPosX(6);
						if (ImGui::BeginCombo(_c(u8"Vuruş Bölgesi"), u8"Tıkla ve Seç!"))
						{
							ImGui::Selectable(_c(u8"Kafa"), &g_Options.rage_hitscan[0], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Boyun"), &g_Options.rage_hitscan[1], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Göğüs"), &g_Options.rage_hitscan[2], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Mide"), &g_Options.rage_hitscan[3], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Karın"), &g_Options.rage_hitscan[4], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Kollar"), &g_Options.rage_hitscan[5], ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Bacaklar"), &g_Options.rage_hitscan[6], ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}
						ImGui::SetCursorPosX(6);


						ImGui::EndChild();

						ImGui::NextColumn();

						ImGui::BeginChild("##22223");
						ImGui::SetCursorPosX(5);
						ImGui::SetCursorPosY(20);
						ImGui::Checkbox(_c(u8"AntiAim'i Aktif Et"), &g_Options.AntiAim_enable);
						ImGui::SetCursorPosX(5);
						if (g_Options.AntiAim_enable) {
							ImGui::Text(_c("Anti-Aim Real Yaw"));
							ImGui::SetCursorPosX(5);
							ImGui::Combo(_c("##Anti-Aim Real Yaw"), &g_Options.antiaim_yaw, AntiaimYawNames, IM_ARRAYSIZE(AntiaimYawNames));
							ImGui::SetCursorPosX(5);
							ImGui::Text(_c("Anti-Aim Fake Yaw"));
							ImGui::SetCursorPosX(5);
							ImGui::Combo(_c("##Anti-Aim Fake Yaw"), &g_Options.antiaim_yaw_fake, AntiaimYawNames, IM_ARRAYSIZE(AntiaimYawNames));
							ImGui::SetCursorPosX(5);
							ImGui::Text(_c("Anti-Aim Pitch"));
							ImGui::SetCursorPosX(5);
							ImGui::Combo(_c("##Anti-Aim Pitch"), &g_Options.antiaim_pitch, AntiaimPitchNames, IM_ARRAYSIZE(AntiaimPitchNames));
							ImGui::SetCursorPosX(5);
							ImGui::SliderFloat(_c(u8"Dönme Hızı"), &g_Options.antiaim_spin_speed, 0.0f, 50.0f);
							ImGui::SetCursorPosX(5);
							ImGui::SliderFloat(_c("Edge Distance"), &g_Options.antiaim_edge_dist, 0.0f, 250.f);
							ImGui::SetCursorPosX(5);
							ImGui::SetCursorPosX(5);
							ImGui::Checkbox(_c(u8"Bıçakta Anti-Aim"), &g_Options.antiaim_knife);
						}
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c("Anti-Resolver"), &g_Options.antiaim_antiresolver);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Yavaş Yürüme"), &g_Options.misc_minwalk);
						if (g_Options.misc_minwalk) {
							ImGui::Text(_c("SlowWalk Key"));
							KeyBind(u8"##slowwalkkey", g_Options.misc_minwalk_bind);
							ImGui::SliderInt(_c(u8"Slowwalk Speed"), &g_Options.misc_minwalk_amount, 0, 100, "%d");
						}
						ImGui::SetCursorPosX(5);
						ImGui::Text(_c(u8"Tank Antiaim Tuşu"));
						ImGui::SetCursorPosX(5);
						KeyBind(u8"##TankAAkey", g_Options.tankAntiaimKey);

						ImGui::EndChild();
					}
					ImGui::EndGroupBox();
					if (g_Options.theme == false) {
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
					}


				}
				if (ragebottabs == 1)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("##Weapons", ImVec2(600, 425), true);
					{


						ImGui::SliderInt(_c(u8"Vuruş Şansı Auto"), &g_Options.RageAimbotHitchanceAuto, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar Auto"), &g_Options.RageAimbotMinDmgAuto, 0, 100, "%d");
						ImGui::Separator();

						ImGui::SliderInt(_c(u8"Vuruş Şansı Scout"), &g_Options.RageAimbotHitchanceScout, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar Scout"), &g_Options.RageAimbotMinDmgScout, 0, 100, "%d");
						ImGui::Separator();

						ImGui::SliderInt(_c(u8"Vuruş Şansı AWP"), &g_Options.RageAimbotHitchanceAWP, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar AWP"), &g_Options.RageAimbotMinDmgAWP, 0, 100, "%d");
						ImGui::Separator();

						ImGui::SliderInt(_c(u8"Vuruş Şansı Pistols"), &g_Options.RageAimbotHitchancePistols, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar Pistols"), &g_Options.RageAimbotMinDmgPistols, 0, 100, "%d");
						ImGui::Separator();

						ImGui::SliderInt(_c(u8"Vuruş Şansı Deagle/R8"), &g_Options.RageAimbotHitchanceDeagle, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar Deagle/R8"), &g_Options.RageAimbotMinDmgDeagle, 0, 100, "%d");
						ImGui::Separator();

						ImGui::SliderInt(_c(u8"Vuruş Şansı Other"), &g_Options.RageAimbotHitchanceOther, 0, 100, "%d");
						ImGui::SliderInt(_c(u8"Minimum hasar Other"), &g_Options.RageAimbotMinDmgOther, 0, 100, "%d");
						ImGui::Separator();

					}
					ImGui::EndChild();
					ImGui::EndGroup();

				}
			}

			if (menus == 1)
			{
				static int   legittab_xd = 0;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
				if (ImGui::OwOButton(u8"LEGİTBOT##tab", legittab_xd == 0 ? true : false, ImVec2(286, 50)))legittab_xd = 0;
				ImGui::SameLine(287);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"TRİGGER##tab", legittab_xd == 1 ? true : false, ImVec2(286, 50)))legittab_xd = 1;
				ImGui::PopStyleVar();
				if (legittab_xd == 0)
				{
					ImGui::BeginGroupBox("##visuals_body");
					{
						//	ImGui::Columns(3, NULL, true);
						ImGui::BeginChild("##aimbot.weapons", ImVec2(185, 475), false);
						{
							ImGui::DeadlySeparator(u8"Aimbot");
							ImGui::Separator();
							static int   settings_category = 0;
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
							if (ImGui::OwOButton(u8"Tümü##tab", settings_category == 0 ? true : false, ImVec2(64, 50)))settings_category = 0;
							ImGui::SameLine(64);
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
							if (ImGui::OwOButton(u8"Gruplu##tab", settings_category == 1 ? true : false, ImVec2(64, 50)))settings_category = 1;
							ImGui::PopStyleVar();
							ImGui::SameLine(128);
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
							if (ImGui::OwOButton(u8"Tekli##tab", settings_category == 2 ? true : false, ImVec2(64, 50)))settings_category = 2;
							ImGui::PopStyleVar();
							ImGui::PushItemWidth(-1);

							if (settings_category == 0)
							{
								settings = &g_Options.legitbot_items_all;
							}
							else if (settings_category == 1)
							{
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

								ImGui::Combo("group##weapons_groups", &g_Options.weapon_group, weapon_groups_array, IM_ARRAYSIZE(weapon_groups_array));
								settings = &g_Options.legitbot_items_groups[g_Options.weapon_group];
								ImGui::PopStyleVar();
								ImGui::PopStyleColor(7);
							}
							else if (settings_category == 2)
							{
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

								if (ImGui::Combo(
									"weapon", &g_Options.settings_weapon,
									[](void* data, int32_t idx, const char** out_text)
									{
										auto vec = reinterpret_cast<std::vector< WeaponName_t >*>(data);
										*out_text = vec->at(idx).name;
										return true;
									}, (void*)(&WeaponNames), WeaponNames.size()))
									ImGui::PopStyleVar();
									ImGui::PopStyleColor(7);
									{
										settings_weap = WeaponNames[g_Options.settings_weapon].definition_index;
									}

									settings = &g_Options.legitbot_items[settings_weap];

									RenderCurrentWeaponButton();
									ImGui::Checkbox(u8"Otomatik Ayarla", &g_Options.use_all_weapons);

							}
							ImGui::PopItemWidth();
							ImGui::Separator();
							ImGui::PushItemWidth(-1);
							ImGui::Checkbox(u8"Aktif Et##3333", &settings->enabled);
							ImGui::Checkbox(u8"Dost Ateşi", &settings->deathmatch);

							if (g_Options.settings_category == 0)
								ImGui::Checkbox(u8"Oto Tabanca", &settings->autopistol);

							else if (g_Options.settings_category == 1)
							{
								if (g_Options.weapon_group == 1)
								{
									ImGui::Checkbox(u8"Oto Tabanca", &settings->autopistol);
								}
							}

							else if (g_Options.settings_category == 2)
							{
								if (settings_weap == WEAPON_P250 ||
									settings_weap == WEAPON_USP_SILENCER ||
									settings_weap == WEAPON_GLOCK ||
									settings_weap == WEAPON_FIVESEVEN ||
									settings_weap == WEAPON_TEC9 ||
									settings_weap == WEAPON_DEAGLE ||
									settings_weap == WEAPON_ELITE ||
									settings_weap == WEAPON_HKP2000) {
									ImGui::Checkbox(u8"Oto Tabanca", &settings->autopistol);
								}
							}

							ImGui::Checkbox(u8"Smoke Kontrol", &settings->check_smoke);
							ImGui::Checkbox(u8"Flash Kontrol ", &settings->check_flash);
							ImGui::Checkbox(u8"Zıplama Kontrol", &settings->check_jump);
							ImGui::Checkbox(u8"Backtrack", &settings->backtrack.enabled);
							ImGui::Checkbox(u8"Oto Duvar", &settings->autowall);
							ImGui::Checkbox(u8"Curve", &settings->humanize);
							ImGui::Checkbox(u8"Silent", &settings->silent);
							ImGui::Checkbox(u8"Anti Aimlock", &settings->antiaimlock);
							if (weapon_index == WEAPON_AWP || weapon_index == WEAPON_SSG08 ||
								weapon_index == WEAPON_AUG || weapon_index == WEAPON_SG556) {
								ImGui::Checkbox(u8"Scope'da iken", &settings->only_in_zoom);
							}

							ImGui::Checkbox(u8"Tuş ile", &settings->on_key);

							if (settings->on_key) {
								ImGui::Text(u8"Tuş");
								ImGui::SameLine();
								KeyBind(u8"##Key bind", settings->key);
							}

							ImGui::Checkbox(u8"Otomatik ateş", &settings->autofire);
							if (settings->autofire) {
								ImGui::Text(u8"Tuş İle");
								ImGui::SameLine();
								KeyBind("##Auto Fire bind", settings->autofire_key);
							}

							ImGui::PopItemWidth();
						}
						ImGui::EndChild();

						//	ImGui::NextColumn();
						ImGui::SameLine(200);
						ImGui::BeginChild("##aimbot.misc", ImVec2(170, 450), false);
						{
							ImGui::DeadlySeparator(u8"Diğer");
							ImGui::Separator();
							ImGui::PushItemWidth(-1);
							static char* priorities[] = {
							"Fov",
							"Health",
							"Damage",
							"Distance"
							};
							static char* aim_types[] = {
							"Hitbox",
							"Nearest"
							};
							static char* fov_types[] = {
							"Static",
							"Dynamic"
							};
							static char* smooth_types[] = {
							"Slow near target",
							"Linear"
							};
							static char* hitbox_list[] = {
							"Head",
							"Neck",
							"Pelvis",
							"Stomach",
							"Lower Chest",
							"Chest",
							"Upper Chest",
							"Left Thigh",
							"Right Thigh",
							};
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));

							ImGui::Text(u8"Aim Türü:");
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

							ImGui::Combo("##aimbot.aim_type", &settings->aim_type, aim_types, IM_ARRAYSIZE(aim_types));
							if (settings->aim_type == 0) {
								ImGui::Text(u8"Vuruş Bölgesi:");
								ImGui::Combo("##aimbot.hitbox", &settings->hitbox, hitbox_list, IM_ARRAYSIZE(hitbox_list));
							}
							ImGui::PopStyleVar();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();
							ImGui::PopStyleColor();

							ImGui::Text(u8"Öncelik:");
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));
							ImGui::Combo("##aimbot.priority", &settings->priority, priorities, IM_ARRAYSIZE(priorities));
							ImGui::PopStyleVar();
							ImGui::PopStyleColor(7);
							ImGui::Text(u8"FOV Türü:");
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));
							ImGui::Combo("##aimbot.type", &settings->fov_type, fov_types, IM_ARRAYSIZE(fov_types));
							ImGui::PopStyleVar();
							ImGui::PopStyleColor(7);
							ImGui::Text(u8"Yumuşaklık Türü:");
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

							ImGui::Combo("##aimbot.smooth_type", &settings->smooth_type, smooth_types, IM_ARRAYSIZE(smooth_types));
							ImGui::PopStyleVar();
							ImGui::PopStyleColor(7);
							ImGui::SliderFloat("##aimbot.fov", &settings->fov, 0, 20, "Fov: %.2f");
							if (settings->silent) {
								ImGui::SliderFloat("##aimbot.silent_fov", &settings->silent_fov, 0, 20, u8"Silent Fov: %.2f");
							}
							ImGui::SliderFloat("##aimbot.smooth", &settings->smooth, 1, 15, u8"Yumuşaklık: %.2f");
							if (!settings->silent) {
								ImGui::SliderInt("##aimbot.shot_delay", &settings->shot_delay, 0, 100, u8"Vuruş Gecikmesi: %.0f");
							}
							ImGui::SliderInt("##aimbot.kill_delay", &settings->kill_delay, 0, 1000, u8"Ölüm Gecikmesi: %.0f");
							if (settings->humanize)
								ImGui::SliderFloat("##aimbot.curve", &settings->curviness, -10.f, 10.f, u8"Curve: %.0f");
							if (settings->backtrack.enabled) {
								ImGui::SliderInt("##aimbot_backtrack_ticks", &settings->backtrack.ticks, 0, 12, u8"BackTrack Tiki: %i");
							}
							if (settings->autowall) {
								ImGui::SliderInt("##aimbot.min_damage", &settings->min_damage, 1, 100, u8"Min Hasar: %.0f");
							}
							ImGui::PopItemWidth();
						}
						ImGui::EndChild();
						ImGui::SameLine();
						ImGui::BeginChild("##aimbot.rcs", ImVec2(165, 450), false);
						{
							ImGui::DeadlySeparator("Sekme Kontrol");
							ImGui::Separator();
							ImGui::PushItemWidth(-1);
							ImGui::SetCursorPosX(5);
							ImGui::SetCursorPosY(33);
							ImGui::Checkbox(u8"Aktif Et##aimbot.rcs", &settings->rcs);
							if (settings->rcs) {


								ImGui::Text(u8"Sekme Kontrol Türü:");
								static char* rcs_types[] = {
								"Standalone",
								"Aim"
								};
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

								ImGui::Combo("##aimbot.rcs_type", &settings->rcs_type, rcs_types, IM_ARRAYSIZE(rcs_types));
								ImGui::PopStyleVar();
								ImGui::PopStyleColor(7);
								ImGui::SetCursorPosX(5);

								ImGui::Checkbox(u8"FOV İle Sekmeme", &settings->rcs_fov_enabled);
								if (settings->rcs_fov_enabled) {
									ImGui::SliderFloat("##aimbot.rcs_fov", &settings->rcs_fov, 0, 20, u8"Sekmeme Fov: %.2f");
								}
								ImGui::SetCursorPosX(5);

								ImGui::Checkbox(u8"Yumuşaklık İle Sekmeme", &settings->rcs_smooth_enabled);
								if (settings->rcs_smooth_enabled) {
									ImGui::SliderFloat("##aimbot.rcs_smooth", &settings->rcs_smooth, 1, 15, u8"Sekmeme Yumuşaklık: %.2f");
								}
								ImGui::SetCursorPosX(5);

								ImGui::SliderInt("##aimbot.rcs_x", &settings->rcs_x, 0, 100, u8"X Yönünde Sekmeme: %d");
								ImGui::SetCursorPosX(5);

								ImGui::SliderInt("##aimbot.rcs_y", &settings->rcs_y, 0, 100, u8"Y Yönünde Sekmeme: %d");
								ImGui::SetCursorPosX(5);

								ImGui::SliderInt("##aimbot.rcs_start", &settings->rcs_start, 1, 30, u8"Sekmeme Başlangıç: %d");
								ImGui::PopItemWidth();
							}
						}
						ImGui::EndChild();
						//ImGui::Columns(1, NULL, false);
					}
					ImGui::EndGroupBox();
				}
				if (legittab_xd == 1)
				{
					ImGui::BeginChild(_c("##trigger_bot_first"), ImVec2(220, 190), true);
					{
						ImGui::Text(_c(u8"Triggerbot: "));
						ImGui::Separator();
						ImGui::SetCursorPosX(5);

						ImGui::Checkbox(_c(u8"Aktif##48231315"), &g_Options.enable_show_trigger);
						if (g_Options.enable_show_trigger) {

							if (!g_Options.enable_trigger_auto) {
								ImGui::SetCursorPosX(5);

								ImGui::Checkbox(_c(u8"Tuşlu trigger##444"), &g_Options.enable_trigger);
								if (g_Options.enable_trigger) {
									ImGui::SameLine();
									KeyBind(_c(u8"##onkey##bindtriggerkey"), g_Options.trigger_key);

								}
							}
							if (!g_Options.enable_trigger) {
								ImGui::SetCursorPosX(5);

								ImGui::Checkbox(_c(u8"Otomatik trigger"), &g_Options.enable_trigger_auto);
							}
						}
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Otomatik durma"), &g_Options.autostop2);
						ImGui::SetCursorPosX(5);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Körken oyuncuları yok say"), &g_Options.trigger_flashcheck);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Sis arkası oyuncuları yok say"), &g_Options.trigger_smokecheck);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(_c(u8"Zıplarken oyuncuları yok say"), &g_Options.trigger_jumpcheck);


					}
					ImGui::EndChild();


					ImGui::SameLine(220);

					ImGui::BeginChild(_c(u8"##trigger_bot_third"), ImVec2(352, 190), true);
					{
						ImGui::Text(_c(u8"Vuruş Bölgesi: "));
						ImGui::Separator();
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

						if (ImGui::BeginCombo(_c(u8"Bölge"), _c(u8"Seçiniz...")))
						{
							ImGui::Selectable(_c(u8"Kafa"), &g_Options.trigger_head, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Göğüs"), &g_Options.trigger_chest, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Mide"), &g_Options.trigger_stomatch, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Kol"), &g_Options.trigger_arms, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c(u8"Bacak"), &g_Options.trigger_legs, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

							ImGui::EndCombo();
						}
						ImGui::PopStyleVar();
						ImGui::PopStyleColor(7);
						ImGui::Text(_c(u8"Ayarlar : "));
						ImGui::Separator();
						ImGui::SetCursorPosX(5);
						ImGui::SliderInt(_c("##trigger_bot_delay"), &g_Options.trigger_delay, 1, 100, _c(u8"Triggerbot gecikmesi: %.0f"));

						ImGui::SetCursorPosX(5);
						ImGui::SliderInt(_c("##trigger_bot_hitchance"), &g_Options.trigger_hitchance, 0, 100, _c(u8"Triggerbot vuruş şansı: %.0f"));
					}
					ImGui::EndChild();




				}

			}

			if (menus == 2)
			{
				static int   visuals_tab = 0;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
				if (ImGui::OwOButton(u8"ESP##tab", visuals_tab == 0 ? true : false, ImVec2(143, 50)))visuals_tab = 0;
				ImGui::SameLine(143);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"DÜNYA##tab", visuals_tab == 1 ? true : false, ImVec2(143, 50)))visuals_tab = 1;
				ImGui::PopStyleVar();
				ImGui::SameLine(286);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"CHAMS##tab", visuals_tab == 2 ? true : false, ImVec2(143, 50)))visuals_tab = 2;
				ImGui::PopStyleVar();
				ImGui::SameLine(429);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"GLOW##tab", visuals_tab == 3 ? true : false, ImVec2(143, 50)))visuals_tab = 3;
				ImGui::PopStyleVar();
				ImGui::PushItemWidth(-1);

				if (visuals_tab == 0)
				{
					ImGui::BeginGroup();
					//ImGui::Columns(2);

					ImGui::BeginChild(_c(u8"##ESP1"), ImVec2(296, 465), true);
					{
						ImGui::DeadlySeparator(_c(u8"ESP"));
						ImGui::Separator();
						ImGui::SetCursorPosX(5);
						ImGui::SetCursorPosY(35);
						ImGui::Checkbox(___(_c(u8"Aktif##ESP"), _c(u8"Enabled##ESP")), &g_Options.esp_enabled);
						ImGui::SetCursorPosX(5);

						ImGui::Checkbox(___(_c(u8"ESP Öngörüş##ESP"), _c(u8"ESP Preview##ESP")), &g_Options.esp_preview);
						ImGui::SetCursorPosX(5);

						ImGui::Checkbox(_c(u8"Backtrack İskeleti"), &g_Options.esp_player_skeleton_bt);
						ImGui::SetCursorPosX(5);
						if (g_Options.esp_enabled) {
							ImGui::ColorEdit3(___(_c(u8"Görünürken##31"), _c(u8"Visible##31")), g_Options.color_esp_visible);
							ImGui::SetCursorPosX(5);



							ImGui::ColorEdit3(___(_c(u8"Görünmezken##31"), _c(u8"Occluded##31")), g_Options.color_esp_occluded);

						}

						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Sadece Görünürken"), _c(u8"Visible only")), &g_Options.esp_visible_only);

						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Sadece Düşman"), _c(u8"Enemy only")), &g_Options.esp_enemy_only);

						ImGui::SetCursorPosX(5);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

						ImGui::Combo(___(_c(u8"Kutu"), _c(u8"Box")), &g_Options.esp_player_box, boxes, IM_ARRAYSIZE(boxes));
						ImGui::PopStyleColor(7);
						ImGui::PopStyleVar();
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"İskelet"), _c(u8"Skeleton")), &g_Options.esp_player_skeleton);
						if (g_Options.esp_player_skeleton) {
							ImGui::SetCursorPosX(5);
							ImGui::ColorEdit4(___(_c(u8"İskelet Rengi"), _c(u8"Skeleton Color")), g_Options.color_esp_skeleton);

						}

						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"İsim"), _c(u8"Name")), &g_Options.esp_player_names);

						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Can"), _c(u8"Health")), &g_Options.esp_player_health);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Armor"), _c(u8"Armour")), &g_Options.esp_player_armour);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Silah"), _c(u8"Weapon")), &g_Options.esp_player_weapons);
						ImGui::SetCursorPosX(5);
						if (g_Options.esp_player_weapons)
						{
							ImGui::Checkbox(___(_c(u8"Icon"), _c(u8"Icon")), &g_Options.esp_weapon_icon);

						}
						ImGui::Checkbox(___(_c(u8"Mermi"), _c(u8"Ammo")), &g_Options.show_ammo);
					}
					ImGui::EndChild();
					ImGui::SameLine(300);
					//	ImGui::NextColumn();
					ImGui::BeginChild(_c(u8"##ESP3"), ImVec2(276, 375), true);
					{
						ImGui::DeadlySeparator(___(_c(u8"Hasar Yardımcısı"), _c("Hit Helper")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Hasar göstergesi"), _c(u8"Damage Indicator")), &g_Options.esp_damageindicator);
						if (g_Options.esp_damageindicator)
							ImGui::Checkbox(___(_c(u8"HP'yi göster"), _c(u8"Show HP")), &g_Options.esp_damageindicator_color);
						ImGui::Text(_c(u8"Ses ESP"));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Aktif Et##ESPSOUND"), _c(u8"Enabled##ESPSOUND")), &g_Options.esp_sounds);
						if (g_Options.esp_sounds)
						{
							ImGui::SliderFloat(___(_c(u8"Zamanı"), _c(u8"Duration")), &g_Options.esp_sounds_time, 0.5f, 3.0f, "%.3f ms");

							ImGui::SliderFloat(___(_c(u8"Büyüklüğü"), _c(u8"Radius")), &g_Options.esp_sounds_radius, 15.0f, 150.0f, "%.1f");
							ImGui::SameLine();
							ImGui::ColorEdit4(_c("##Sound color"), g_Options.color_esp_sounds, ImGuiColorEditFlags_NoInputs);
						}
						ImGui::Text(_c(u8"Thirdperson"));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Aktif Et##TP"), _c(u8"Enabled##TP")), &g_Options.misc_thirdperson);
						if (g_Options.misc_thirdperson)
						{
							ImGui::SliderFloat(___(_c(u8"Thirdperson Uzaklığı"), _c(u8"ThirdPerson Distance")), &g_Options.misc_thirdperson_dist, 50.f, 500.f);
							KeyBind(___(_c(u8"Tuş##TP"), _c(u8"Bind##TP")), g_Options.misc_thirdperson_bind);
						}
						ImGui::Text(_c(u8"Viewmodel"));
						ImGui::Separator();
						ImGui::SliderInt(_c("FOV1"), &g_Options.misc_viewmodel_fov, 68, 120, "%d");
						ImGui::SliderInt(_c("FOV2"), &g_Options.misc_fov, 90, 150, "%d");

					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}

				if (visuals_tab == 1)
				{
					ImGui::DeadlySeparator(___(_c(u8"Dünya"), _c("World")));
					ImGui::Separator();
					ImGui::SetCursorPosX(5);
					ImGui::SetCursorPosY(45);
					ImGui::Checkbox(___(_c(u8"Düşmüş Silahlar"), _c(u8"Dropped Weapons")), &g_Options.esp_dropped_weapons);
					if (g_Options.esp_dropped_weapons)
					{
						ImGui::SetCursorPosX(5);

						ImGui::Checkbox(___(_c(u8"Düşmüş Silahlar Kutusu"), _c(u8"Dropped Weapons Box")), &g_Options.esp_dropped_weapons_box);
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Düşmüş Silahlar Mermisi"), _c(u8"Dropped Weapons Ammo")), &g_Options.esp_droppped_ammo_bar);
						ImGui::SetCursorPosX(5);
						ImGui::ColorEdit3(_c(u8"Düşmüş Silahlar Rengi"), g_Options.color_esp_weapons);

					}

					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(___(_c(u8"İmha Kiti"), _c(u8"Defuse Kit")), &g_Options.esp_defuse_kit);
					/*if (g_Options.esp_defuse_kit)
					{
						ImGui::Checkbox(___(u8"İmha Kiti Kutusu", u8"Defuse Kit Box"), &g_Options.esp_dropped_defusekits_box);
						ImGui::ColorEdit3(u8"İmha Kiti Rengi", g_Options.color_esp_defusekit);

					}*/
					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(___(_c(u8"Kurulmuş C4"), _c(u8"Planted C4")), &g_Options.esp_planted_c4);
					if (g_Options.esp_planted_c4)
					{
						ImGui::SetCursorPosX(5);
						ImGui::Checkbox(___(_c(u8"Kurulmuş C4 Kutusu"), _c(u8"Planted C4 Box")), &g_Options.esp_dropped_c4_box);
						ImGui::SetCursorPosX(5);
						ImGui::ColorEdit3(_c(u8"Kurulmuş C4 Rengi"), g_Options.color_esp_c4);
					}
					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(___(_c(u8"İtem Esp"), _c(u8"Item Esp")), &g_Options.esp_items);
					/*ImGui::Checkbox(_c("Pistol Case"), &Vars.esp_case_pistol);
					ImGui::Checkbox(_c("Light Case"), &Vars.esp_case_light_weapon);
					ImGui::Checkbox(_c("Heavy Case"), &Vars.esp_case_heavy_weapon);
					ImGui::Checkbox(_c("Explosive Case"), &Vars.esp_case_explosive);
					ImGui::Checkbox(_c("Tools case"), &Vars.esp_case_tools);
					ImGui::Checkbox(_c("Airdrop"), &Vars.esp_random);
					ImGui::Checkbox(_c("Full Armor"), &Vars.esp_dz_armor_helmet);
					ImGui::Checkbox(_c("Helmet"), &Vars.esp_dz_helmet);
					ImGui::Checkbox(_c("Armor"), &Vars.esp_dz_armor);
					ImGui::Checkbox(_c("Tablet Upgrade"), &Vars.esp_upgrade_tablet);
					ImGui::Checkbox(_c("Briefcase"), &Vars.esp_briefcase);
					ImGui::Checkbox(_c("Parachute"), &Vars.esp_parachutepack);
					ImGui::Checkbox(_c("Cash Dufflebag"), &Vars.esp_dufflebag);
					ImGui::Checkbox(_c("Ammobox"), &Vars.esp_ammobox);*/
					if (g_Options.esp_items)
						ImGui::SetCursorPosX(5);
					ImGui::SliderFloat(___(_c(u8"Uzaklığı"), _c(u8"Distance")), &g_Options.esp_distance_dz, 64.0f, 8192.f, "%.1fu");
					ImGui::Spacing();
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.80f, 0.80f, 0.80f, 1.00f));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

					if (ImGui::BeginCombo(___(_c(u8"Flaglar"), _c(u8"Flags")), "..."))
					{
						ImGui::Selectable((___(_c(u8"Armor"), _c(u8"Armour"))), &g_Options.esp_flags_armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Çözüyor"), _c(u8"Defusing"))), &g_Options.esp_flags_defusing, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"İmha Kiti"), _c(u8"Defuse Kit"))), &g_Options.esp_flags_kit, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Rehine Taşıyıcısı"), _c(u8"Hostage Carrier"))), &g_Options.esp_flags_hostage, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Dürbün Açık"), _c(u8"Scoped"))), &g_Options.esp_flags_scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Para"), _c(u8"Money"))), &g_Options.esp_flags_money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Bomba"), _c(u8"Bomb"))), &g_Options.esp_flags_c4, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable((___(_c(u8"Flash"), _c(u8"Flashed"))), &g_Options.esp_flags_flashed, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
					ImGui::PopStyleColor(8);
					ImGui::PopStyleVar();
					//		ImGui::Checkbox(___(u8"Gece Modu", u8"Nightmode"), &g_Options.nightmode);
						/*	ImGui::Checkbox("Nightmode", &g_Options.other_nightmode);
							if (g_Options.other_nightmode)
								ImGui::SliderFloat("Size", &g_Options.other_nightmode_size, 0.05f, 1.0f, "%.3f");*/
					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(_c("Gece Modu"), &g_Options.nightmode);
					ImGui::SetCursorPosX(5);
					ImGui::ColorEdit4(_c("Hava Rengi"), g_Options.clr_sky, ImGuiColorEditFlags_NoInputs);
					if (g_Options.nightmode) {
						ImGui::SetCursorPosX(5);
						ImGui::SliderInt(_c(u8"Parlaklık"), &g_Options.nightmode_brightness, 0, 100);
						ImGui::SetCursorPosX(5);
						ImGui::Combo(_c("Skybox"), &g_Options.misc_sky_boxes, skyboxes, IM_ARRAYSIZE(skyboxes));
						if (ImGui::Button(u8"Güncelle"))
						{
							NightMode::Get().remove();
							Sleep(400);
							NightMode::Get().apply();
						}
					}
					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(___(_c(u8"Gökkuşağı Modu"), _c(u8"Rainbow Mode")), &g_Options.other_mat_ambient_light_rainbow);
					const char* time_type[] = { _c("bar"), _c("text") };

					bool* smoke_time_types[2]{ &g_Options.smoke_bar, &g_Options.smoke_timer };
					bool* molotov_time_types[2]{ &g_Options.molotov_bar, &g_Options.molotov_timer };

					const char* grenadesarray[] = { _c("smoke"), _c("flahsbang"), _c("he grenade"), _c("decoy"), _c("molotov") };
					static int selected_grenade;

					ImGui::SetCursorPosX(5);
					ImGui::Checkbox(_c("granade's##check"), &g_Options.grenades_enable);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

					ImGui::Combo(_c("grenade"), &selected_grenade, grenadesarray, IM_ARRAYSIZE(grenadesarray));
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(7);
					if (selected_grenade == 0)
					{
						ImGui::SetCursorPosX(5);
						ImGui::ColorEdit3(_c("name##smoke"), g_Options.color_smoke);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

						ImGui::MultiCombo(_c("time display type###smoke"), time_type, smoke_time_types, 2);
						ImGui::PopStyleColor(7);
						ImGui::PopStyleVar();
						if (smoke_time_types[0])
						{
							ImGui::ColorEdit3(_c("bar main##smoke"), g_Options.color_bar_smoke_main);
							ImGui::ColorEdit3(_c("bar back##smoke"), g_Options.color_bar_smoke_back);
						}
					}
					else if (selected_grenade == 1)
					{
						ImGui::ColorEdit3(_c("name##flashbang"), g_Options.color_flash);
					}
					else if (selected_grenade == 2)
					{
						ImGui::ColorEdit3(_c("name##hegrenade"), g_Options.color_frag);
					}
					else if (selected_grenade == 3)
					{
						ImGui::ColorEdit3(_c("name##decoy"), g_Options.color_decoy);
					}
					else if (selected_grenade == 4)
					{
						ImGui::ColorEdit3(_c("name##molotov"), g_Options.color_molotov);
						ImGui::MultiCombo(_c("time display type##molotov"), time_type, molotov_time_types, 2);
						if (molotov_time_types[0])
						{
							ImGui::ColorEdit3(_c("bar main##molotov"), g_Options.color_bar_molotov_main);
							ImGui::ColorEdit3(_c("bar back##molotov"), g_Options.color_bar_molotov_back);
						}
					}

				}

				if (visuals_tab == 2)
				{
					ImGui::BeginChild(_c(u8"##CHAMS1"), ImVec2(250, 375), true);
					{
						ImGui::DeadlySeparator(_c(u8"Chams"));
						ImGui::Separator();

						ImGui::Checkbox(___(_c(u8"Aktif Et##3224"), _c(u8"Enabled##2234")), &g_Options.chams_player_enabled);
						if (g_Options.chams_player_enabled)
						{
							ImGui::Checkbox(___(_c(u8"Sadece Düşman##3232"), _c(u8"Enemy Only##23232")), &g_Options.chams_player_enemies_only);
							ImGui::ColorEdit4(___(_c(u8"Görünürken##123123"), _c(u8"Visible##123123")), g_Options.color_chams_player_visible);
							ImGui::ColorEdit4(___(_c(u8"Duvar Arkası##123123"), _c(u8"Occluded##12321")), g_Options.color_chams_player_occluded);
							ImGui::Checkbox(___(_c(u8"Çizgili##23123"), _c(u8"WireFrame##12321")), &g_Options.chams_player_wireframe);
							ImGui::Checkbox(___(_c(u8"Düz##123123"), _c(u8"Flat##123123")), &g_Options.chams_player_flat);

							ImGui::Checkbox(___(_c(u8"Sadece Görünürken##312312"), _c(u8"Visible Only##123123")), &g_Options.chams_player_visibleonly);
						}
						ImGui::Separator();

						ImGui::Text(___(_c("Kol Chams"), _c("Arm Chams")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Aktif##45645646"), _c("Enable##45645646")), &g_Options.chams_arms_enabled);

						if (g_Options.chams_arms_enabled)
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

							ImGui::Combo(___(_c(u8"Materyal##154534"), _c("Material##154534")), &g_Options.chams_arms_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
							ImGui::PopStyleColor(7);
							ImGui::PopStyleVar();
							ImGui::Checkbox(___(_c(u8"Çizgili##5454"), _c(u8"WireFrame##5454")), &g_Options.chams_arms_wireframe);
							ImGui::Checkbox(___(_c(u8"Reflektif##35215f"), _c(u8"Reflective##35215f")), &g_Options.chams_arms_reflect);

							ImGui::ColorEdit4(___(_c(u8"Kol Rengi"), _c("Arms Color")), g_Options.chams_arms_color);
						}


						ImGui::Separator();
						ImGui::Text(___(_c(u8"Yerel Oyuncu Chamsı"), _c("Local Player Chams")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Yerel Oyuncu Chams##laa"), _c("Local Player Chams##laa")), &g_Options.esp_localplayer_chams);
						if (g_Options.esp_localplayer_chams)
						{
							ImGui::Checkbox(___(_c(u8"Sahte Açı Chams##laa"), _c("Fake Angle Chams##laa")), &g_Options.fake_chams);
							if (g_Options.fake_chams)
							{
								ImGui::Separator();
								ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
								ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

								ImGui::Combo(___(_c(u8"Sahte Açı Materyal##laa"), _c("Fake Angle Chams Material##laa")), &g_Options.esp_localplayer_fakechams_type, opt_Chams_Mat, 9);
								ImGui::PopStyleColor(7);
								ImGui::PopStyleVar();
							}
							ImGui::Text(___(_c(u8"Sahte Chams Color"), _c("Fake Angle Chams Color")));
							ImGui::Separator();
							ImGui::ColorEdit4(_c("##esp_localplayer_fakechams_color_picker"), g_Options.esp_localplayer_fakechams_color_visible, false);
						}
					}

					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild(_c(u8"##CHAMS2"), ImVec2(250, 375), true);
					{
						ImGui::DeadlySeparator(___(_c(u8"Silah Chams"), _c("Weapon Chams")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Aktif Et##1231234"), _c(u8"Enabled##412321321")), &g_Options.chams_weapon_enabled);
						if (g_Options.chams_weapon_enabled)
						{
							ImGui::ColorEdit4(___(_c(u8"Renk##WeaponColor"), _c(u8"##WeaponColor")), g_Options.chams_weapon_color);
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

							ImGui::Combo(_c("Materials##21"), &g_Options.chams_weapon_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
							ImGui::PopStyleColor(7);
							ImGui::PopStyleVar();
							ImGui::Checkbox(___(_c(u8"Çizgili"), _c(u8"WireFrame")), &g_Options.chams_weapon_wireframe);
							ImGui::Checkbox(___(_c(u8"Reflektif"), _c(u8"Reflective")), &g_Options.chams_weapon_reflect);
						}
						ImGui::Text(___(_c(u8"Kıyıfet Chams"), _c("Sleeves Chams")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Aktif Et##341231238"), _c(u8"Enabled##341231238")), &g_Options.chams_sleeves_enabled);
						if (g_Options.chams_sleeves_enabled)
						{
							ImGui::ColorEdit4(___(_c(u8"Renk##SleevesColor"), _c(u8"##SleevesColor")), g_Options.chams_sleeves_color);
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.80f, 0.80f, 0.80f, 0.00f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.80f, 0.80f, 0.80f, 0.05f));
							ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.80f, 0.80f, 0.80f, 0.07f));

							ImGui::Combo(_c("Materials##441"), &g_Options.chams_sleeves_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
							ImGui::PopStyleColor(7);
							ImGui::PopStyleVar();
							ImGui::Checkbox(___(_c(u8"Çizgili##123123"), _c(u8"WireFrame##123213")), &g_Options.chams_sleeves_wireframe);
							ImGui::Checkbox(___(_c(u8"Material##123123"), _c(u8"Reflective##12312321")), &g_Options.chams_sleeves_reflect);
						}
					}
					ImGui::EndChild();
				}

				if (visuals_tab == 3)
				{
					ImGui::DeadlySeparator(_c(u8"Glow"));
					ImGui::Separator();
					ImGui::Checkbox(___(_c(u8"Glow Aktif Et##33124"), _c(u8"Glow Enabled##3421321")), &g_Options.glow_enabled);
					if (g_Options.glow_enabled)
					{
						ImGui::Checkbox(___(_c(u8"Parlayan"), _c("Pulsing")), &g_Options.glow_pulsing);
						ImGui::Checkbox(___(_c(u8"Sadece Düşmanlar##3234"), _c(u8"Enemies Only##3324")), &g_Options.glow_enemy_only);
						ImGui::ColorEdit4(___(_c(u8"Glow Rengi##3234"), _c(u8"Glow Color##3324")), g_Options.color_glow);

					}
				}
			}

			if (menus == 3)
			{




				ImGui::BeginChild(u8"##MISC1", ImVec2(257, 375), true);
				{
					static int   movtab = 0;
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
					if (ImGui::OwOButton(u8"KISIM 1##tab", movtab == 0 ? true : false, ImVec2(120, 50)))movtab = 0;
					ImGui::SameLine(120);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					if (ImGui::OwOButton(u8"KISIM 2##tab", movtab == 1 ? true : false, ImVec2(120, 50)))movtab = 1;
					ImGui::PopStyleVar();
					ImGui::PushItemWidth(-1);
					ImGui::DeadlySeparator(___(_c(u8"Hareket Ayarları"), _c(u8"Movement Settings")));
					ImGui::Separator();
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PopStyleVar();

					if (movtab == 0)
					{
						ImGui::Spacing();
						ImGui::Checkbox(___(_c(u8"Sahte Eğilme"), _c(u8"Fake Duck")), &g_Options.fakeduck);
						if (g_Options.fakeduck)
							KeyBind(___(_c(u8"Sahte Eğilme Tuşu"), _c(u8"Fake Duck Bind")), g_Options.fakeduck_bind);
						ImGui::Checkbox(___(_c(u8"Otomatik Bunny"), _c(u8"Bunny Hop")), &g_Options.misc_bhop);
						ImGui::Checkbox(___(_c(u8"Otomatik Strafe"), _c(u8"Auto Strafe")), &g_Options.misc_autostrafer);
						if (g_Options.misc_autostrafer)
							ImGui::SliderFloat(___(_c(u8"Tepki Süresi"), _c(u8"Retrack Speed")), &g_Options.retrack, 1.f, 15.f);
						ImGui::Checkbox(___(_c(u8"Hızlı Eğilme"), _c(u8"Fast Duck")), &g_Options.misc_fastduck);
						ImGui::Checkbox(___(_c(u8"Ay Yürüşü"), _c(u8"Moonwalk")), &g_Options.maykilceksin);
						ImGui::Checkbox(___(_c(u8"Edge Zıplama"), _c(u8"Edge Jump")), &g_Options.edgejump);
						if (g_Options.edgejump)
							KeyBind(___(_c(u8"Tuş##EDGE"), _c(u8"Bind##EDGE")), g_Options.edgejump_bind);
						ImGui::Text(___(_c(u8"Legit Anti-Aim"), _c("Legit Anti-Aim")));
						ImGui::Separator();
						ImGui::Combo(___(_c(u8"Aktif Et##laa"), _c("Enable##laa")), &g_Options.misc_desync, items, 3);
						if (g_Options.misc_desync)
						{
							ImGui::Checkbox(___(_c(u8"Otomatik Yönlendirme"), _c("Auto Direction")), &g_Options.misc_desync_ad);
							KeyBind(___(_c(u8"Legit AA Tuşu"), _c("Legit AA Bind")), g_Options.misc_desync_bind);
							ImGui::Checkbox(___(_c(u8"Sahte Açı Çizgileri"), _c(u8"Angle Lines")), &g_Options.esp_angle_lines);
						}
						ImGui::Checkbox(___(_c(u8"Otomatik Bloklama"), _c(u8"Auto Block")), &g_Options.autoblock);
						if (g_Options.autoblock)
							KeyBind(___(_c(u8"Tuş##AB"), _c(u8"Bind##AB")), g_Options.autoblock_bind);
					}

					if (movtab == 1)
					{
						ImGui::Spacing();
						ImGui::Checkbox(___(_c(u8"Sahte Lag"), _c(u8"Fake Lag")), &g_Options.fakelag_enabled);
						if (g_Options.fakelag_enabled)
						{
							ImGui::Checkbox(___(_c(u8"Ayakta"), _c(u8"Standing")), &g_Options.fakelag_standing);
							ImGui::SameLine(85);
							ImGui::Checkbox(___(_c(u8"Yürürken"), _c(u8"Moving")), &g_Options.fakelag_moving);
							ImGui::SameLine(170);
							ImGui::Checkbox(_c("Unducking"), &g_Options.fakelag_unducking);
							ImGui::Combo(___(_c(u8"Mod##FK"), _c(u8"Mode##FK")), &g_Options.fakelag_mode, std::vector<std::string>{"Factor", "Switch", "Adaptive", "Random", "Legit peek"});
							ImGui::SliderInt(___(_c(u8"Faktör##FK"), _c(u8"Factor##FK")), &g_Options.fakelag_factor, 1, 15);
						}



						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"İnsanlaştırılmış Bhop"), _c("Humanised Bhop")), &g_Options.misc_human_bhop);
						if (g_Options.misc_human_bhop)
						{
							ImGui::SliderInt(___(_c(u8"Bhop Vuruş Şansı"), _c("Bhop Hit Chance")), &g_Options.bhop_hit_chance, 0, 100, "%.0f %%");
							ImGui::SliderInt(___(_c(u8"Zıplama Limiti"), _c("Hops Limit")), &g_Options.hops_restricted_limit, 0, 12);
							ImGui::SliderInt(___(_c(u8"Maksimum Zıplama Limiti"), _c("Max Hops Limit")), &g_Options.max_hops_hit, 0, 12);

						}
						ImGui::Checkbox(u8"Dalga Efekti", &g_Options.hiteffect);

						/* if (ImGui::ImageButtonWithText(miscblack, u8"Görüş", u8"Chams vb.", ImVec2(40, 40)))
						{
							menus = 0;
						} */
						ImGui::Checkbox(u8"Oyun İçi Radar Hilesi", &g_Options.radar.ingame);
						ImGui::Checkbox(u8"Vuruş Efekti", &g_Options.kill_counter);
						ImGui::Checkbox(u8"Sekme Crosshair", &g_Options.recoilCrosshair);
						//ImGui::Checkbox(u8"Ekran Efekti 1", &g_Options.ekranefekti);
						ImGui::Separator();
					}

				}
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild(_c(u8"##MISC2"), ImVec2(375, 375), true);
				{
					ImGui::DeadlySeparator(___(_c(u8"Diğer Eklentiler"), _c("Other Addons")));
					ImGui::Separator();
					static int   otherAddTab = 0;
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
					if (ImGui::OwOButton(u8"KISIM 1##tab", otherAddTab == 0 ? true : false, ImVec2(100, 50)))otherAddTab = 0;
					ImGui::SameLine(100);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					if (ImGui::OwOButton(u8"KISIM 2##tab", otherAddTab == 1 ? true : false, ImVec2(100, 50)))otherAddTab = 1;
					ImGui::PopStyleVar();
					ImGui::SameLine(200);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					if (ImGui::OwOButton(u8"KISIM 3##tab", otherAddTab == 2 ? true : false, ImVec2(100, 50)))otherAddTab = 2;
					ImGui::PopStyleVar();
					ImGui::PushItemWidth(-1);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PopStyleVar();
					if (otherAddTab == 0)
					{
						ImGui::Spacing();

						ImGui::Checkbox(___(_c(u8"Filigran"), _c("Watermark")), &g_Options.misc_watermark);
						ImGui::Checkbox(___(_c(u8"Anti-OBS"), _c(u8"Anti-OBS")), &g_Options.misc_anti_obs);
						ImGui::Checkbox(___(_c(u8"Oto Maç Kabul"), _c(u8"Auto-Accept")), &g_Options.misc_autoaccept);
						ImGui::Checkbox(_c(u8"Rank Gösterici"), &g_Options.misc_rankreveal);
						ImGui::Checkbox(___(_c(u8"İzleyici Listesi"), _c(u8"Spectator List")), &g_Options.esp_speclist);
						ImGui::Checkbox(___(_c(u8"Blur Kapat"), _c(u8"Disable Blur")), &g_Options.misc_disableblur);
						ImGui::Checkbox(___(_c(u8"Rengarenk Bar"), _c(u8"Rainbow Bar")), &g_Options.misc_rainbowbar);
						//ImGui::Checkbox(___(_c(u8"Saydam Duvar"), _c(u8"AsusWall")), &g_Options.gorus_saydamduvarbakim);
						ImGui::Checkbox(___(_c(u8"Sol El Bıçak"), _c(u8"Left Hand Knife")), &g_Options.solbicak);
						ImGui::Checkbox(___(_c(u8"Parayı Göster"), _c(u8"Reveal Money")), &g_Options.misc_reveal_money);
						ImGui::Combo(___(_c(u8"Chat Spam##154534"), _c("Chat Spam##154534")), &g_Options.misc_chatspam, chatspamtype, IM_ARRAYSIZE(chatspamtype));
						if (g_Options.misc_chatspam == 2)
						{
							ImGui::InputText(u8"", g_Options.spam, sizeof(g_Options.spam));

						}
						ImGui::Checkbox(___(_c(u8"Yer Çekimi"), _c("Gravity")), &g_Options.cl_phys_timescale);
						if (g_Options.cl_phys_timescale)
						{
							ImGui::SliderFloat(___(_c(u8"Ölçek"), _c("Scale")), &g_Options.cl_phys_timescale_value, 0, 1, "%.1f");
						}
						ImGui::Checkbox(_c("Zeus Range"), &g_Options.zeusrange);
					}

					if (otherAddTab == 1)
					{
						ImGui::Spacing();

						ImGui::Checkbox(___(_c(u8"Aspect Ratio Aktif"), _c(u8"Aspect Ratio Enabled")), &g_Options.aspect_ratio_enable);
						if (g_Options.aspect_ratio_enable)
						{
							ImGui::SliderFloat(_c("Aspect Ratio##Features"), &g_Options.aspect_ratio, 0, 10.0);
						}

						ImGui::Checkbox(___(_c(u8"Bıçak Bot"), _c(u8"Knife Bot")), &g_Options.kbot);
						ImGui::Checkbox(_c("Bullet Tracer"), &g_Options.visuals_bullet_tracer);
						ImGui::ColorEdit4(_c("##bullettracercolor"), g_Options.color_bullet_tracer, false);

						ImGui::Checkbox(___(_c(u8"FOV Çiz"), _c(u8"Draw Fov")), &g_Options.other_drawfov);
						ImGui::Checkbox(___(_c(u8"Bomba Yardımcısı"), _c(u8"Grenade Prediction")), &g_Options.esp_grenade_prediction);
						ImGui::Checkbox(___(_c(u8"Kalıcı Crosshair"), _c(u8"Crosshair")), &g_Options.crosshair);
						ImGui::Checkbox(___(_c(u8"Vuruş Sesi"), _c(u8"Hit Sound")), &g_Options.hitsound);
						ImGui::Text(___(_c(u8"Efektler"), _c("Effects")));
						ImGui::Separator();
						ImGui::Checkbox(___(_c(u8"Elleri Sil"), _c(u8"No Hands")), &g_Options.other_no_hands);
						ImGui::Checkbox(___(_c(u8"Sis Yok"), _c("No Smoke")), &g_Options.nosmoke);
						ImGui::SliderFloat(___(_c(u8"Flash Opaklığı"), _c(u8"Flash Ammount")), &g_Options.flash, 0.f, 255.f);
						ImGui::Checkbox(___(_c(u8"FPS'inizi Optimize Edin!"), _c("Optimise")), &g_Options.esp_optimise);
						ImGui::Checkbox(___(_c(u8"Dürbünü Kaldır"), _c(u8"Remove Scope")), &g_Options.nozoom);
					}

					if (otherAddTab == 2)
					{
						ImGui::Spacing();
						/*		ImGui::Text(___(u8"Hava Değiştirici", "Skybox Changer"));
								ImGui::Separator();
								ImGui::Checkbox(___(u8"Aktif##378999654", "Enable##378999654"), &g_Options.skychanger);

								ImGui::Combo(___(u8"Liste", "List"), &g_Options.skychanger_mode, skyList, IM_ARRAYSIZE(skyList));
								if (g_Options.skychanger)
								{
									Misc::Get().SkyChanger();
								}*/
						ImGui::Separator();
						ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.80f, 0.80f, 0.80f, 1.00f));
						if (ImGui::BeginCombo(_c("Logger"), _c("Select"))) {

							//ImGui::Selectable("Hurt", &g_Options.eventlogger_hurt, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c("Buy"), &g_Options.eventlogger_player_buy, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c("Planting"), &g_Options.eventlogger_planting, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(_c("Defusing"), &g_Options.eventlogger_defusing, ImGuiSelectableFlags_DontClosePopups);

							ImGui::EndCombo();
						}
						ImGui::PopStyleColor();
						const char* Style[] =
						{
							_c("1 style"),
							_c("2 style"),
							_c("3 style"),
							_c("4 style")
						};

						ImGui::Text(_c("Radar"));
						ImGui::Checkbox(_c("Enabled##1112"), &g_Options.Act);
						if (g_Options.Act)
						{
							ImGui::Combo(_c("Style"), &g_Options.RadarStyle, Style, ARRAYSIZE(Style));
							ImGui::SliderFloat(_c("Range"), &g_Options.Zoom, 0, 4);
							ImGui::SliderInt(_c("Size##132"), &g_Options.Size, 0, 10);
						}
						ImGui::Separator();
						ImGui::Combo(_c("Radio Channel"), &g_Options.radiochannel, radio, IM_ARRAYSIZE(radio));
						ImGui::SliderInt(_c("Volume"), &g_Options.radiovolume, 0, 100);
						KeyBind(_c("Mute"), g_Options.radiomute);

						ImGui::Checkbox(___(_c(u8"Otomatik C4 Çözme"), _c("Auto Defuse")), &g_Options.misc_autodefuse);
						ImGui::Checkbox(___(_c(u8"Kıyafeti Sil"), _c("No Sleeves")), &g_Options.misc_nosleeves);


					}

				}
				ImGui::EndChild();
			}

			if (menus == 4)
			{
				auto& entries = g_Options.m_items;

				static int   skin_tab = 0;
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
				if (ImGui::OwOButton(u8"SKİN##tab", skin_tab == 0 ? true : false, ImVec2(100, 50)))skin_tab = 0;
				ImGui::SameLine(100);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"RANK##tab", skin_tab == 1 ? true : false, ImVec2(100, 50)))skin_tab = 1;
				ImGui::PopStyleVar();
				ImGui::SameLine(200);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::OwOButton(u8"MODEL##tab", skin_tab == 2 ? true : false, ImVec2(100, 50)))skin_tab = 2;
				ImGui::PopStyleVar();
				ImGui::PushItemWidth(-1);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PopStyleVar();

				if (skin_tab == 0)
				{
					ImGui::Columns(2, nullptr, false);

					ImGui::BeginChild(_c("Silah Seçimi##skin window"), ImVec2(0.f, 0.f), true);
					{
						ImGui::ListBoxHeader(_c("Silahlar##sjinstab"), ImVec2(0, 277));
						{
							for (size_t w = 0; w < k_weapon_names.size(); w++)
							{
								switch (w)
								{
								case 0:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("knife"));
									break;
								case 1:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("glove"));
									break;
								case 2:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("pistols"));
									break;
								case 12:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("semi-rifle"));
									break;
								case 19:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("rifle"));
									break;
								case 26:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("sniper-rifle"));
									break;
								case 30:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("machingun"));
									break;
								case 32:
									ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), _c("shotgun"));
									break;
								}

								if (ImGui::Selectable(k_weapon_names[w].name, definition_vector_index == w))
								{
									definition_vector_index = w;
								}
							}
						}
						ImGui::ListBoxFooter();

						static float next_enb_time = 0;

						float time_to_next_up = g_GlobalVars->curtime;

						time_to_next_up = std::clamp(next_enb_time - g_GlobalVars->curtime, 0.f, 1.f);

						std::string name = _c(u8"Güncelle (");
						name += std::to_string(time_to_next_up);
						name.erase(12, 16);
						name += ")";

						if (ImGui::Button(name.c_str(), ImVec2(221, 0)))
						{
							if (next_enb_time <= g_GlobalVars->curtime)
							{
								static auto clear_hud_weapon_icon_fn =
									reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
										Utils::PatternScan(GetModuleHandleA(_c("client.dll")), _c("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC")));

								auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

								if (element)
								{
									auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
									if (hud_weapons != nullptr)
									{

										if (*hud_weapons->get_weapon_count())
										{
											for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
												i = clear_hud_weapon_icon_fn(hud_weapons, i);

											typedef void(*ForceUpdate) (void);
											static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
											FullUpdate();

											g_ClientState->ForceFullUpdate();
										}
									}
								}

								next_enb_time = g_GlobalVars->curtime + 1.f;
							}
						}
					}
					ImGui::EndChild();

					ImGui::NextColumn();

					ImGui::BeginChild(_c(u8"Skin Seçimi##skin tab"), ImVec2(0, 0), true);
					{
						static int selected_tab_skins = 0;

						auto& selected_entry = entries[k_weapon_names[definition_vector_index].definition_index];
						auto& satatt = g_Options.statrack_items[k_weapon_names[definition_vector_index].definition_index];
						selected_entry.definition_index = k_weapon_names[definition_vector_index].definition_index;
						selected_entry.definition_vector_index = definition_vector_index;

						static int   skin_tabxd = 0;
						static char* skin_tabss[] = { (u8"Genel"), (u8"Skin Seçimi") };

						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
						render_tabs(skin_tabss, skin_tabxd, (ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().WindowPadding.x * 2.0f) / _countof(skin_tabss), 25.0f);
						ImGui::PopStyleVar();

						if (selected_entry.definition_index != GLOVE_T_SIDE &&
							selected_entry.definition_index != GLOVE_CT_SIDE &&
							selected_entry.definition_index != WEAPON_KNIFE &&
							selected_entry.definition_index != WEAPON_KNIFE_T)
						{
							selected_weapon_name = k_weapon_names_preview[definition_vector_index].name;
						}
						else
						{
							if (selected_entry.definition_index == GLOVE_T_SIDE ||
								selected_entry.definition_index == GLOVE_CT_SIDE)
							{
								selected_weapon_name = k_glove_names_preview.at(selected_entry.definition_override_vector_index).name;
							}
							if (selected_entry.definition_index == WEAPON_KNIFE ||
								selected_entry.definition_index == WEAPON_KNIFE_T)
							{
								selected_weapon_name = k_knife_names_preview.at(selected_entry.definition_override_vector_index).name;
							}
						}

						if (skin_tabxd == 0)
						{
							ImGui::Spacing();
							ImGui::Checkbox(_c("skin preview"), &g_Options.skin_preview);
							ImGui::Checkbox(_c(u8"Stattrak##2"), &selected_entry.stat_trak);
							ImGui::InputInt(_c(u8"Seed"), &selected_entry.seed);
							ImGui::InputInt(_c("stattrak"), &satatt.statrack_new.counter);
							ImGui::SliderFloat(_c(u8"Eskimişlik"), &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);

							if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T)
							{
								ImGui::ListBoxHeader(_c(u8"Bıçak##sdsdadsdadas"), ImVec2(0, 152));
								{
									for (int i = 0; i < k_knife_names.size(); i++)
									{
										if (ImGui::Selectable(k_knife_names[i].name, selected_entry.definition_override_vector_index == i))
										{
											selected_entry.definition_override_vector_index = i;

											selected_skin_name = "";
										}
									}

									selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
								}
								ImGui::ListBoxFooter();
							}
							else if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE)
							{
								ImGui::ListBoxHeader(_c(u8"Eldiven##sdsdadsdadas"), ImVec2(0, 152));
								{
									for (int i = 0; i < k_glove_names.size(); i++)
									{
										if (ImGui::Selectable(k_glove_names[i].name, selected_entry.definition_override_vector_index == i))
										{
											selected_entry.definition_override_vector_index = i;
										}
									}

									selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
								}
								ImGui::ListBoxFooter();
							}
							else {
								static auto unused_value = 0;
								selected_entry.definition_override_vector_index = 0;
							}
						}

						if (skin_tabxd == 1)
						{
							ImGui::Spacing();

							if (skins_parsed)
							{
								static char filter_name[32];
								std::string filter = filter_name;

								bool is_glove = selected_entry.definition_index == GLOVE_T_SIDE ||
									selected_entry.definition_index == GLOVE_CT_SIDE;

								bool is_knife = selected_entry.definition_index == WEAPON_KNIFE ||
									selected_entry.definition_index == WEAPON_KNIFE_T;

								int cur_weapidx = 0;
								if (!is_glove && !is_knife)
								{
									cur_weapidx = k_weapon_names[definition_vector_index].definition_index;
									//selected_weapon_name = k_weapon_names_preview[definition_vector_index].name;
								}
								else
								{
									if (selected_entry.definition_index == GLOVE_T_SIDE ||
										selected_entry.definition_index == GLOVE_CT_SIDE)
									{
										cur_weapidx = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
									}
									if (selected_entry.definition_index == WEAPON_KNIFE ||
										selected_entry.definition_index == WEAPON_KNIFE_T)
									{
										cur_weapidx = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;

									}
								}

								ImGui::InputText(_c(u8"İsim Filtresi"), filter_name, sizeof(filter_name));
								ImGui::Checkbox(_c(u8"Sadece Seçilen Silahın Skinlerini Göster"), &g_Options.show_cur);


								auto weaponName = weaponnames(cur_weapidx);


								ImGui::ListBoxHeader(_c(u8"Skinler##sdsdadsdadas"), ImVec2(0, 261));
								{
									if (selected_entry.definition_index != GLOVE_T_SIDE && selected_entry.definition_index != GLOVE_CT_SIDE)
									{
										if (ImGui::Selectable(" - ", selected_entry.paint_kit_index == -1))
										{
											selected_entry.paint_kit_vector_index = -1;
											selected_entry.paint_kit_index = -1;
											selected_skin_name = "";
										}

										int lastID = ImGui::GetItemID();
										for (size_t w = 0; w < k_skins.size(); w++)
										{
											for (auto names : k_skins[w].weaponName)
											{
												std::string name = k_skins[w].name;

												if (g_Options.show_cur)
												{
													if (names != weaponName)
														continue;
												}

												if (name.find(filter) != name.npos)
												{
													ImGui::PushID(lastID++);

													ImGui::PushStyleColor(ImGuiCol_Text, skins::get_color_ratiry(is_knife && g_Options.show_cur ? 6 : k_skins[w].rarity));
													{
														if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w))
														{
															selected_entry.paint_kit_vector_index = w;
															selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
															selected_skin_name = k_skins[w].name_short;
														}
													}
													ImGui::PopStyleColor();

													ImGui::PopID();
												}
											}
										}
									}
									else
									{
										int lastID = ImGui::GetItemID();

										if (ImGui::Selectable(" - ", selected_entry.paint_kit_index == -1))
										{
											selected_entry.paint_kit_vector_index = -1;
											selected_entry.paint_kit_index = -1;
											selected_skin_name = "";
										}

										for (size_t w = 0; w < k_gloves.size(); w++)
										{
											for (auto names : k_gloves[w].weaponName)
											{
												std::string name = k_gloves[w].name;
												//name += " | ";
												//name += names;

												if (g_Options.show_cur)
												{
													if (names != weaponName)
														continue;
												}

												if (name.find(filter) != name.npos)
												{
													ImGui::PushID(lastID++);

													ImGui::PushStyleColor(ImGuiCol_Text, skins::get_color_ratiry(6));
													{
														if (ImGui::Selectable(name.c_str(), selected_entry.paint_kit_vector_index == w))
														{
															selected_entry.paint_kit_vector_index = w;
															selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
															selected_skin_name = k_gloves[selected_entry.paint_kit_vector_index].name_short;
														}
													}
													ImGui::PopStyleColor();

													ImGui::PopID();
												}
											}
										}
									}
								}
								ImGui::ListBoxFooter();
							}
							else
							{
								ImGui::Text(_c(u8"skin dataları alınıyor, bekleyin..."));
							}
						}
					}
					ImGui::EndChild();
				}

				if (skin_tab == 1)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild(_c(u8"##CHANGER1"), ImVec2(233, 375), true);
					if (ImGui::Button(___(_c("Prime Ol"), _c("Update Prime"))))
					{

						static uint8_t* fakePrimeStatus = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "83 F8 05 5E 0F 94 C0 C3");
						DWORD old_protect;

						VirtualProtect(fakePrimeStatus, 3, PAGE_EXECUTE_READWRITE, &old_protect);
						char patch[] = { 0x83, 0xF8, 0x00 };
						memcpy(fakePrimeStatus, patch, 3);
						VirtualProtect(fakePrimeStatus, 3, old_protect, nullptr);
					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}

				if (skin_tab == 2)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild(_c(u8"##MODELC"), ImVec2(0, 375), true);
					{
						ImGui::DeadlySeparator(___(_c(u8"Model Changer"), _c(u8"Model Değiştirici")));
						ImGui::Separator();
						ImGui::Checkbox(_c(u8"Özel Modelleri Etkinleştir"), &g_Options.custommodelson);
						ImGui::Combo(_c("CT-MODELS"), &g_Options.customodelsct, customModelsct, ARRAYSIZE(customModelsct));
						ImGui::Combo(_c("T-MODELS"), &g_Options.customodelst, customModelst, ARRAYSIZE(customModelst));

						if (ImGui::Button(___(_c(u8"Devre Dışı Bırak"), _c("Disable")), ImVec2(240.0f, 20.0f)))
						{
							g_Options.customodelsct = 0;
							g_Options.customodelst = 0;
							g_ClientState->ForceFullUpdate();
						}

						ImGui::Combo(_c("Player T"), &g_Options.player_model_t, _c("Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0"));
						ImGui::Combo(_c("Player CT"), &g_Options.player_model_ct, _c("Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0"));
					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}


			}

			if (menus == 5)
			{
				ImGui::BeginChild(_c(u8"##CONFIG1"), ImVec2(0, 0), true);
				{
					static std::vector<std::string> configs;

					static auto load_configs = []() {
						std::vector<std::string> items = {};

						std::string path("C:\\xhanger");
						if (!fs::is_directory(path))
							fs::create_directories(path);

						for (auto& p : fs::directory_iterator(path))
							items.push_back(p.path().string().substr(path.length() + 1));

						return items;
					};

					static auto is_configs_loaded = false;
					if (!is_configs_loaded) {
						is_configs_loaded = true;
						configs = load_configs();
					}

					static std::string current_config;

					static char config_name[32];
					ImGui::Spacing();
					ImGui::InputText(_c(u8"##config_name"), config_name, sizeof(config_name));
					ImGui::SameLine();
					if (ImGui::Button(___(_c(u8"Oluştur"), _c("Create")))) {
						current_config = std::string(config_name);

						Config->Save(current_config + ".ini");
						is_configs_loaded = false;
						memset(config_name, 0, 32);
					}

					ImGui::BeginChild(_c("##configs"), ImVec2(300, 250), true);
					ImGui::ListBoxHeader(_c("##configs"));
					{
						for (auto& config : configs) {
							if (ImGui::Selectable(config.c_str(), config == current_config)) {
								current_config = config;
							}
						}
					}
					ImGui::ListBoxFooter();
					ImGui::EndChild();

					if (!current_config.empty()) {

						if (ImGui::Button(___(_c(u8"Yükle"), _c("Load"))))
						{
							if (current_config.empty())
								g_Notification.Push(_c("Hata"), _c(u8"Lütfen Config Seçiniz"));
							else
							{
								Config->Load(current_config);
								g_Notification.Push(_c(u8"Başarılı"), (current_config + _c(u8" Başarıyla Yüklendi")).c_str());

							}



						}
						ImGui::SameLine();

						if (ImGui::Button(___(_c(u8"Kaydet"), _c("Save"))))
						{
							if (current_config.empty())
								g_Notification.Push(_c("Hata"), _c(u8"Lütfen Config Seçiniz"));
							else
							{
								Config->Save(current_config);
								g_Notification.Push(_c(u8"Başarılı"), (current_config + _c(u8" Başarıyla Kaydedildi")).c_str());
							}

						}
						ImGui::SameLine();

						if (ImGui::Button(___(_c(u8"Sil"), _c("Delete"))) && fs::remove("C:\\xhanger\\" + current_config)) {
							if (current_config.empty())
								g_Notification.Push(_c("Hata"), _c(u8"Lütfen Config Seçiniz"));
							else
							{
								g_Notification.Push(_c(u8"Başarılı"), (current_config + _c(u8" Başarıyla Silindi")).c_str());
								current_config.clear();
								is_configs_loaded = false;
							}
						}

						ImGui::SameLine();
					}

					if (ImGui::Button(___(_c(u8"Yenile"), _c("Refresh"))))
						is_configs_loaded = false;

					ImGui::DeadlySeparator(_c(u8"Kullanıcı Bilgileri"));
					ImGui::Separator();
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
					ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::BeginChild(_c("##kullaniciadi"), ImVec2(200, 18));
					ImGui::Text(_c(u8"Kullanıcı : %s"), kadi.c_str());
					ImGui::EndChild();
					ImGui::BeginChild(_c("##zaman"), ImVec2(200, 18));
					ImGui::Text(_c(u8"Kalan Süre : %s"), zamans.c_str());
					ImGui::EndChild();
					ImGui::PopItemFlag();
					ImGui::SameLine();
					if (ImGui::Button(_c(u8"Beyaz Tema")))
						g_Options.theme = false;
					ImGui::SameLine();
					if (ImGui::Button(_c(u8"Siyah Tema")))
						g_Options.theme = true;
					ImGui::PopStyleColor(5);

				}
				ImGui::EndChild();
			}
			static int anim = 80;
			static bool animaktif = false;
			static bool annen = 0;
			static bool kapali = true;
			static int score = 0;
			static int highscore = 0;
			static int hiz = 2;
			static int boyut = 30;
			static int level = 1;
			static bool oyunbaslat = true;
			static bool solyukari = false;
			static bool solasagi = false;
			static bool sagasagi = false;
			static bool sagyukari = false;
			static bool anan = false;
			static float sag = 24;
			static float yukari = 80;
			static bool oldun = false;
			static int g = 0;
			static int menumod = 0;
			static bool skinfiltre = false;
			static bool backtrack = false;
			static int trackback = 0;
			if (menus == 6) {
				if (score == 200)
				{
					level = 2;
					boyut -= 2;
					score += 1;
				}
				if (score == 500)
				{
					level = 3;
					boyut -= 2;
				}
				if (score == 800)
				{
					level = 4;
					boyut -= 2;
				}
				if (score == 1100)
				{
					level = 5;
					boyut -= 2;
				}
				if (score == 1400)
				{
					level = 6;
					boyut -= 2;
				}
				if (score == 1700)
				{
					level = 7;
					boyut -= 2;
				}
				if (score == 2000)
				{
					level = 8;
					boyut -= 2;
				}
				if (score == 2300)
				{
					level = 9;
					boyut -= 2;
				}
				if (score == 2600)
				{
					level = 10;
					boyut -= 2;
				}

				if (oyunbaslat == true)
				{
					if (yukari == 80 && sag == 24)
					{
						solyukari = true;
						solasagi = false;
						sagasagi = false;
						sagyukari = false;
					}
					if (yukari == 540 && sag == 24)
					{
						solyukari = false;
						solasagi = true;
						sagasagi = false;
						sagyukari = false;
					}
					if (yukari == 540 && sag == 660)
					{
						solyukari = false;
						solasagi = false;
						sagasagi = true;
						sagyukari = false;
					}
					if (yukari == 80 && sag == 660)
					{
						solyukari = false;
						solasagi = false;
						sagasagi = false;
						sagyukari = true;
					}
					if (solyukari)
					{
						yukari += hiz;
					}
					else if (solasagi)
					{
						sag += hiz;
					}
					else if (sagasagi)
					{
						yukari -= hiz;
					}
					else if (sagyukari)
					{
						sag -= hiz;
					}
				}
				/*
				if (GetKeyState(VK_UP) & 0x8000)
				{
					yukari -= 5;
				}
				if (GetKeyState(VK_DOWN) & 0x8000)
				{
					yukari += 5;
				}
				if (GetKeyState(VK_RIGHT) & 0x8000)
				{
					sag += 5;
				}
				if (GetKeyState(VK_LEFT) & 0x8000)
				{
					sag -= 5;
				}
				*/
				ImGui::Spacing();
				ImGui::BeginChild("##Butonlar", ImVec2(0, 0), ImGuiSetCond_FirstUseEver, false);
				ImGui::Text(u8"Skor :");
				ImGui::SameLine();

				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.20f, 0.24f, 0.80f, 1.00f), "%d", score);
				ImGui::SameLine();
				ImGui::Text(u8"Yüksek Skor :");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), "%d", highscore);
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.20f, 0.24f, 0.52f, 1.00f), "Seviye :  %d", level);
				ImGui::SameLine();
				ImGui::Text(u8"Kullanıcı :");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
				ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
				ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
				ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
				ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.05f, 0.00f, 0.11f, 0.00f));
				ImGui::BeginChild("Kullanici Adi2", ImVec2(0, 25), ImGuiWindowFlags_NoScrollbar);
				ImGui::TextColored(ImColor(5, 213, 255), "%s", kadi.c_str());
				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Separator();
				ImGui::SetCursorPos(ImVec2(sag, yukari));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.49f, 0.83f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 0.00f, 0.00f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.00f, 0.49f, 0.83f, 1.00f));
				if (ImGui::Button("", ImVec2(boyut, boyut)))
				{
					oyunbaslat = true;

				}
				if (oyunbaslat == true)
				{
					if (ImGui::IsItemHovered())
					{
						score += 1;
					}
					else
					{
						if (score > highscore)
						{
							highscore = score;
						}
						solyukari = false;
						solasagi = false;
						sagasagi = false;
						sagyukari = false;
						oyunbaslat = false;
						boyut = 30;
						level = 1;
						hiz = 2;
						sag = 24;
						yukari = 80;
						score = 0;

					}
				}



				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::EndChild();




			}
		}
		ImGui::EndChild();

		ImGui::End();

	}

	if (g_Options.skin_preview && menus == 4 && skin_tab == 0 && g_Options.show_cur)
	{
		if ((selected_skin_name != old_name_skin) || (selected_weapon_name != old_name_weap))
		{
			std::string filename = selected_skin_name == "" ? "resource/flash/econ/weapons/base_weapons/" + std::string(selected_weapon_name) + ".png" : "resource/flash/econ/default_generated/" + std::string(selected_weapon_name) + "_" + std::string(selected_skin_name) + "_light_large.png";

			const auto handle = g_BaseFileSys->open(filename.c_str(), "r", "GAME");
			if (handle)
			{
				int file_len = g_BaseFileSys->size(handle);
				char* image = new char[file_len];

				g_BaseFileSys->read(image, file_len, handle);
				g_BaseFileSys->close(handle);

				D3DXCreateTextureFromFileInMemory(g_D3DDevice9, image, file_len, &m_skin_texture);

				delete[] image;

				old_name_skin = selected_skin_name;
				old_name_weap = selected_weapon_name;
			}
		}

		ImGui::SetNextWindowSize(ImVec2{ 300, 250 });

		if (ImGui::Begin(_c("skin preview##window"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			if (m_skin_texture)
			{
				ImGui::GetWindowDrawList()->AddImage(m_skin_texture, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 256, ImGui::GetWindowPos().y + 192), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 250));
			}

			ImGui::End();
		}
	}
	if (menus == 2 && g_Options.esp_preview)
	{
		ImVec2 esp_pos;
		ImColor col;
		ImGui::SetNextWindowSize(ImVec2(253, 395));
		ImGui::Begin(_c("##esp-preview"), &g_Options.esp_preview, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		ImGui::PushFont(g_pDefaultFont);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() / 2) - (ImGui::CalcTextSize("ESP PREVIEW").x / 2) - 20.f);
		ImGui::Text(_c("ESP PREVIEW"));
		ImGui::PopFont();
		//-------------------------------------------------------------
		esp_pos = ImGui::GetCursorScreenPos();
		//-------------------------------------------------------------
		if (g_Options.esp_player_health)
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(esp_pos.x, esp_pos.y + 15.f), ImVec2(esp_pos.x + 5.f, esp_pos.y + 300.f), ImColor(0, 255, 0, 255));//Health
			//-------------------------------------------------------------
		if (g_Options.esp_player_box == 1)
			ImGui::GetWindowDrawList()->AddRect(ImVec2(esp_pos.x + 10.f, esp_pos.y + 15.f), ImVec2(esp_pos.x + 185.f, esp_pos.y + 300.f), ImColor(255, 255, 255));//Box
			 //-------------------------------------------------------------
		if (g_Options.esp_player_armour)
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(esp_pos.x + 195.f, esp_pos.y + 15.f), ImVec2(esp_pos.x + 190.f, esp_pos.y + 300.f), ImColor(0, 0, 255, 255));//Armour
			//-------------------------------------------------------------
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (250 / 2) - (ImGui::CalcTextSize(_c("Name")).x / 2) - 25.f);
		if (g_Options.esp_player_names)
			ImGui::Text(_c("Name"));
		else
			ImGui::NewLine();
		//-------------------------------------------------------------
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 280.f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (250 / 2) - (ImGui::CalcTextSize(_c("Weapon")).x / 2) - 20.f);
		if (g_Options.esp_player_weapons)
			ImGui::Text(_c("Weapon"));
		else
			ImGui::NewLine();
		//-------------------------------------------------------------
		ImGui::End();
	}
}

void Visuals::RenderSpectatorList() {
	int specs = 0;
	std::string spect = "";

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
		int localIndex = g_EngineClient->GetLocalPlayer();
		C_BasePlayer* pLocalEntity = C_BasePlayer::GetPlayerByIndex(localIndex);
		if (pLocalEntity) {
			for (int i = 1; i < g_EngineClient->GetMaxClients(); i++) {
				C_BasePlayer* pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);
				if (!pBaseEntity)										     continue;
				if (pBaseEntity->m_iHealth() > 0)							 continue;
				if (pBaseEntity == pLocalEntity)							 continue;
				if (pBaseEntity->IsDormant())								 continue;
				if (!pLocalEntity->IsAlive()) {
					if (pBaseEntity->m_hObserverTarget() != pLocalEntity->m_hObserverTarget())
						continue;
				}
				else {
					if (pBaseEntity->m_hObserverTarget() != pLocalEntity)
						continue;
				}

				player_info_t pInfo;
				g_EngineClient->GetPlayerInfo(pBaseEntity->EntIndex(), &pInfo);
				if (pInfo.ishltv) continue;

				spect += pInfo.szName;
				spect += "\n";
				specs++;
			}
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin(_c(u8"İzleyici Listesi"), nullptr, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (specs > 0) spect += "\n";

		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(200, 25 + size.y));
		ImGui::Text(spect.c_str());
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void Menu::Toggle()
{
	if (!g_Unload)
		_visible = !_visible;
}

void Menu::CreateStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle& style = ImGui::GetStyle();


	style.Alpha = 0.8f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowRounding = 9.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.FramePadding = ImVec2(4, 1);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 3.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
	ImGui::GetStyle() = _style;
}