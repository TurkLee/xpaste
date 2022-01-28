#include "MinHook.h"
#include "../helpers/MinHook.h"

void MinHook::setup(void* base) noexcept
{
    this->base = base;
}

void MinHook::hook_index(std::size_t index, void* fun) noexcept
{
    void* orig;
    MH_CreateHook((*reinterpret_cast<void***>(base))[index], fun, &orig);
    originals[index] = uintptr_t(orig);
}