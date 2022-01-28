#pragma once
#include "IAppSystem.hpp"
using LocalizeStringIndex_t = unsigned;
class ILocalizeTextQuery
{
public:
	virtual int ComputeTextWidth(const wchar_t* pString) = 0;
};
class ILocalizationChangeCallback
{
public:
	virtual void OnLocalizationChanged() = 0;
};
class ILocalize : public IAppSystem
{
public:
	virtual bool AddFile(const char* fileName, const char* pPathID = nullptr, bool bIncludeFallbackSearchPaths = false) = 0;
	virtual void RemoveAll() = 0;
	virtual wchar_t* Find(const char* tokenName) = 0;
	virtual const wchar_t* FindSafe(const char* tokenName) = 0;
	virtual int ConvertANSIToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes) = 0;
	virtual int ConvertUnicodeToANSI(const wchar_t* unicode, char* ansi, int ansiBufferSize) = 0;
	virtual LocalizeStringIndex_t FindIndex(const char* tokenName) = 0;
	virtual void ConstructString(wchar_t* unicodeOuput, int unicodeBufferSizeInBytes, const wchar_t* formatString, int numFormatParameters, ...) = 0;
	virtual const char* GetNameByIndex(LocalizeStringIndex_t index) = 0;
	virtual wchar_t* GetValueByIndex(LocalizeStringIndex_t index) = 0;
};

#include <fstream>
#include <string>
#include <optional>
#include <map>

class i_basefilesystem
{
public:

    int read(void* output, int size, void* file)
    {
        return CallVFunction<int(__thiscall*)(void*, void*, int, void*)>(this, 0)(this, output, size, file);
    }

    void* open(const char* file_name, const char* options, const char* path_id)
    {
        return CallVFunction<void* (__thiscall*)(void*, const char*, const char*, const char*)>(this, 2)(this, file_name, options, path_id);
    }

    void close(void* file)
    {
        return CallVFunction<void(__thiscall*)(void*, void*)>(this, 3)(this, file);
    }

    unsigned int size(void* file)
    {
        return CallVFunction<unsigned int(__thiscall*)(void*, void*)>(this, 7)(this, file);
    }
};