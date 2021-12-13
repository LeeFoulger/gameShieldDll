#pragma once

/*
--account test_account --sign-in-code test_code --environment test_environment

struct c_game_shield_interface
{
	HMODULE frostHandle;
	void(__cdecl* frostSetUserName)(const char*);
	void(__cdecl* frostSetCharName)(const char*);
	void(__cdecl* frostInitialize)(const char*);
	void(__cdecl* frostFinalize)();
	void(__cdecl* frostSetUserNameW)(const wchar_t*);
	void(__cdecl* frostSetCharNameW)(const wchar_t*);
};
*/

namespace frost
{
	void on_frostSetUserName(const char* userName);
	void on_frostSetCharName(const char* charName);
	void on_frostInitialize(const char* moduleName);
	void on_frostFinalize();
	void on_frostSetUserNameW(const wchar_t* userName);
	void on_frostSetCharNameW(const wchar_t* charName);
}

DLL_EXPORT void __cdecl frostSetUserName(const char* userName)
{
	frost::on_frostSetUserName(userName);
}
DLL_EXPORT void __cdecl frostSetCharName(const char* charName)
{
	frost::on_frostSetCharName(charName);
}
DLL_EXPORT void __cdecl frostInitialize(const char* moduleName)
{
	frost::on_frostInitialize(moduleName);
}
DLL_EXPORT void __cdecl frostFinalize()
{
	frost::on_frostFinalize();
}
DLL_EXPORT void __cdecl frostSetUserNameW(const wchar_t* userName)
{
	frost::on_frostSetUserNameW(userName);
}
DLL_EXPORT void __cdecl frostSetCharNameW(const wchar_t* charName)
{
	frost::on_frostSetCharNameW(charName);
}
