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
#include "simple_patch.inl"

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
		console_print("frostSetUserName(%s)\n", userName);
	}
	inline void on_frostSetCharName(const char* charName)
	{
		console_print("frostSetCharName(%s)\n", charName);
	}
	inline void on_frostInitialize(const char* moduleName)
	{
		console_print("frostInitialize(%s)\n", moduleName);
	}
	inline void on_frostFinalize()
	{
		console_print("frostFinalize()\n");
	}
	inline void on_frostSetUserNameW(const wchar_t* userName)
	{
		console_print("frostSetUserNameW(%ls)\n", userName);
	}
	inline void on_frostSetCharNameW(const wchar_t* charName)
	{
		console_print("frostSetCharNameW(%ls)\n", charName);
	}
}

DWORD WINAPI main_loop(LPVOID param)
{
	char* command_line = get_command_line();

	backend_session_bypass();
	halo_game::populate_game_load_map_offsets();

	//game_load_map("guardian", _game_mode_multiplayer, k_default_game_engine_multiplayer_variant);

	return S_OK;
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

template<unsigned long t_data_length, typename t_data_type>
unsigned long t_data_size(t_data_type(&data)[t_data_length])
{
	return t_data_length - 1;
}

void on_dll_process_attach()
{
	{
		// skip intros
		{
			char patch[sizeof(s_simple_patch_file_header) + 0x400]{};
			unsigned long total_patch_size = sizeof(s_simple_patch_file_header);

			unsigned char patch_data[] = "_";
			simple_patch_setup(patch, sizeof(patch), 1, _simple_patch_file_type_memset);
			simple_patch_set_name(patch, "bink format string");
			simple_patch_set_description(patch, "skip the intro video files");
			simple_patch_set_pattern(patch, total_patch_size, "bink\\%s.bik", t_data_size("bink\\%s.bik"));
			simple_patch_set_mask(patch, total_patch_size, "xxxxxxxxxxx", t_data_size("xxxxxxxxxxx"));
			simple_patch_set_data(patch, total_patch_size, patch_data, t_data_size(patch_data));
			simple_patch_write_file(patch, total_patch_size, "skip_intros.patch");
		}

		simple_patch_read_files_from_folder("patches");
	}

	static s_module_info module_info = {};
	module_info_get(module_info);

	{
		PIMAGE_NT_HEADERS nt_header;
		module_get_nt_header(NULL, &nt_header);

		char datetime_str[80]{};
		time_to_str(&nt_header->FileHeader.TimeDateStamp, &datetime_str, "%Y-%m-%d");

		char product_version_str[80]{};
		product_version_to_str(module_info.product_version, &product_version_str);

		console_print("Halo Online (%s %s)\n", product_version_str, datetime_str);
	}

	unsigned long key_handler_thread_id;
	create_thread(key_handler::thread_func, &key_handler_thread_id, &module_info);

	unsigned long thread_id;
	create_thread(main_loop, &thread_id);
}

#if _DEBUG
void console::on_console_create()
{
}
#endif // _DEBUG

