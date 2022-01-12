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
#include "key_handler.inl"

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

DWORD WINAPI main_loop(LPVOID)
{
	char* command_line = get_command_line();

	backend_session_bypass();
	halo_game::populate_game_load_map_offsets();


	return 0;
}

namespace key_handler
{
	LRESULT WINAPI window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_KEYDOWN)
		{
			on_key_press(wParam, VK_F1, game_reload_map);
			on_key_press(wParam, VK_F2, game_load_map_dialog);
		}

		return CallWindowProcW(reinterpret_cast<WNDPROC>(original_window_proc), hWnd, uMsg, wParam, lParam);
	}
}

HANDLE create_thread(DWORD(WINAPI* thread_func)(LPVOID), LPDWORD thread_id, LPVOID param = NULL)
{
	return CreateThread(NULL, 0, thread_func, param, 0, thread_id);
}

void on_dll_process_attach()
{
	static s_module_info module_info = {};
	module_info_get(module_info);

	{
		PIMAGE_NT_HEADERS nt_header;
		module_get_nt_header(NULL, &nt_header);

		char datetime_str[80]{};
		time_to_str(&nt_header->FileHeader.TimeDateStamp, &datetime_str, "%Y-%m-%d");

		char product_version_str[80]{};
		product_version_to_str(module_info.product_version, &product_version_str);

		printf("Halo Online (%s %s)\n", product_version_str, datetime_str);
	}

	unsigned long key_handler_thread_id;
	create_thread(key_handler::thread_func, &key_handler_thread_id, &module_info);

	unsigned long thread_id;
	create_thread(main_loop, &thread_id);
}
