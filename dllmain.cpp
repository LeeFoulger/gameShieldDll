#include "pch.h"

static c_config g_config("config.ini");

#include "gameShieldDll.inl"

#if _DEBUG
#include "console.inl"
#endif // _DEBUG

#include "file.inl"
#include "memory.inl"
#include "dialogs.inl"
#include "versioning.inl"
#include "halo_game.inl"
#include "patches.inl"
#include "hooks.inl"

void on_dll_process_attach();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	SetProcessDPIAware();

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		on_dll_process_attach();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

namespace frost
{
	inline void on_frostSetUserName(const char* userName)
	{
		printf("frostSetUserName(%s)\n", userName);
	}
	inline void on_frostSetCharName(const char* charName)
	{
		printf("frostSetCharName(%s)\n", charName);
	}
	inline void on_frostInitialize(const char* moduleName)
	{
		printf("frostInitialize(%s)\n", moduleName);
	}
	inline void on_frostFinalize()
	{
		printf("frostFinalize()\n");
	}
	inline void on_frostSetUserNameW(const wchar_t* userName)
	{
		printf("frostSetUserNameW(%ls)\n", userName);
	}
	inline void on_frostSetCharNameW(const wchar_t* charName)
	{
		printf("frostSetCharNameW(%ls)\n", charName);
	}
}

DWORD WINAPI main_loop(void* param)
{
	char* command_line = get_command_line();

	backend_session_bypass();
	halo_game::populate_game_load_map_offsets();


	bool running = true;
	while (running)
	{
		if (GetAsyncKeyState(VK_F1) & 0x8000)
		{
			game_reload_map();
		}
		if (GetAsyncKeyState(VK_F2) & 0x8000)
		{
			game_load_map_dialog();
		}

		Sleep(50);
	}

	return 0;
}

void on_dll_process_attach()
{
	static s_module_info module_info = {};
	module_info_get(module_info);

	DWORD thread_id;
	CreateThread(NULL, 0, main_loop, NULL, 0, &thread_id);
}
