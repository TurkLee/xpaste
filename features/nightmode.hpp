#pragma once
#pragma once

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "../helpers/singleton.hpp"
#include "../helpers/utils.hpp"
#include "../render.hpp"
#include "../hooks.hpp"
#include "../valve_sdk/sdk.hpp"
#include "../helpers/config.hpp"

class NightMode : public Singleton<NightMode> {
public:
	void run() noexcept;
	void apply() noexcept;
	void remove() noexcept;
};