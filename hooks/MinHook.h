#pragma once

#include <cassert>
#include <cstddef>
#include <unordered_map>

class MinHook {
public:
    void setup(void* base) noexcept;
    void restore() noexcept {}
    void hook_index(std::size_t index, void* fun) noexcept;

    template <typename T, std::size_t Idx, typename ...Args>
    auto callOriginal(Args... args) const noexcept
    {
        const auto it = originals.find(Idx);
        if (it != originals.cend())
            return reinterpret_cast<T(__thiscall*)(void*, Args...)>(it->second)(base, args...);
        assert(false);
        std::exit(1);
    }
    template<typename T>
    T get_original(int index)
    {
        return (T)originals[index];
    }
    template <typename T, typename ...Args>
    auto getOriginal(std::size_t index, Args... args) const noexcept
    {
        const auto it = originals.find(index);
        if (it != originals.cend())
            return reinterpret_cast<T(__thiscall*)(void*, Args...)>(it->second);
        assert(false);
        std::exit(1);
    }
    template<class Type>
    static Type HookManual(uintptr_t* vftable, uint32_t index, Type fnNew) {
        DWORD Dummy;
        Type fnOld = (Type)vftable[index];
        VirtualProtect((void*)(vftable + index * 0x4), 0x4, PAGE_EXECUTE_READWRITE, &Dummy);
        vftable[index] = (uintptr_t)fnNew;
        VirtualProtect((void*)(vftable + index * 0x4), 0x4, Dummy, &Dummy);
        return fnOld;
    }
private:
    void* base;
    std::unordered_map<std::size_t, uintptr_t> originals;
};
