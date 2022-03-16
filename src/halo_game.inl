#pragma once

namespace halo_game
{
	unsigned long get_command_line_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x04D033FC;
			//case _product_version_ms26_332089: return 0x04D03370;
			//case _product_version_ms27_373869: return 0x04E8643C;
			//case _product_version_ms27_416138: return 0x04E864BC;
			//case _product_version_ms28_430653: return 0x04E919C8;
			//case _product_version_ms28_454665: return 0x04E91A48;
			//case _product_version_ms28_479394: return 0x04E9DA48;
			case _product_version_ms29_498295: return 0x04EB6DD4;
			case _product_version_ms29_530945: return 0x04EBE9BC;
			case _product_version_ms29_533032: return 0x04EBE93C;
			case _product_version_ms29_554482: return 0x04EBE9BC;
			case _product_version_ms29_571698: return 0x04EBE944;
			case _product_version_ms29_604673: return 0x04EBE944;
			//case _product_version_ms30_700255: return 0x03F3095C;
		}
		return 0;
	}
	char** command_line = module_pointer<char*>(NULL, get_command_line_module_offset());


	unsigned long get_global_preferences_set_last_language_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00097B80;
			//case _product_version_ms26_332089: return 0x00097B20;
			//case _product_version_ms27_373869: return 0x00097BC0;
			//case _product_version_ms27_416138: return 0x00097EB0;
			//case _product_version_ms28_430653: return 0x00099D90;
			//case _product_version_ms28_454665: return 0x00099D90;
			//case _product_version_ms28_479394: return 0x00099D90;
			case _product_version_ms29_498295: return 0x0009AB10;
			case _product_version_ms29_530945: return 0x0009AB10;
			case _product_version_ms29_533032: return 0x0009A860;
			case _product_version_ms29_554482: return 0x0009AD90;
			case _product_version_ms29_571698: return 0x0009AB10;
			case _product_version_ms29_604673: return 0x0009AC10;
			//case _product_version_ms30_700255: return 0x0009B630;
		}
		return 0;
	}
	using t_global_preferences_set_last_language = void __fastcall(unsigned long);
	t_global_preferences_set_last_language* global_preferences_set_last_language = module_pointer<t_global_preferences_set_last_language>(NULL, get_global_preferences_set_last_language_module_offset());

	unsigned long get_global_preferences_get_last_language_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00097C10;
			//case _product_version_ms26_332089: return 0x00097BB0;
			//case _product_version_ms27_373869: return 0x00097C50;
			//case _product_version_ms27_416138: return 0x00097F40;
			//case _product_version_ms28_430653: return 0x00099E20;
			//case _product_version_ms28_454665: return 0x00099E20;
			//case _product_version_ms28_479394: return 0x00099E20;
			case _product_version_ms29_498295: return 0x0009ABA0;
			case _product_version_ms29_530945: return 0x0009ABA0;
			case _product_version_ms29_533032: return 0x0009A8F0;
			case _product_version_ms29_554482: return 0x0009AE20;
			case _product_version_ms29_571698: return 0x0009ABA0;
			case _product_version_ms29_604673: return 0x0009ACA0;
			//case _product_version_ms30_700255: return 0x0009B6C0;
		}
		return 0;
	}
	using t_global_preferences_get_last_language = unsigned long __fastcall(void);
	t_global_preferences_get_last_language* global_preferences_get_last_language = module_pointer<t_global_preferences_get_last_language>(NULL, get_global_preferences_get_last_language_module_offset());

	unsigned long get_global_preferences_get_fullscreen_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00098A60;
			//case _product_version_ms26_332089: return 0x00098A20;
			//case _product_version_ms27_373869: return 0x00098AA0;
			//case _product_version_ms27_416138: return 0x00098D90;
			//case _product_version_ms28_430653: return 0x0009AC70;
			//case _product_version_ms28_454665: return 0x0009AC70;
			//case _product_version_ms28_479394: return 0x0009AC70;
			case _product_version_ms29_498295: return 0x0009BA70;
			case _product_version_ms29_530945: return 0x0009BA70;
			case _product_version_ms29_533032: return 0x0009B7C0;
			case _product_version_ms29_554482: return 0x0009BCF0;
			case _product_version_ms29_571698: return 0x0009BA70;
			case _product_version_ms29_604673: return 0x0009BB70;
			//case _product_version_ms30_700255: return 0x0009C580;
		}
		return 0;
	}
	using t_global_preferences_get_fullscreen = bool __fastcall(void);
	t_global_preferences_get_fullscreen* global_preferences_get_fullscreen = module_pointer<t_global_preferences_get_fullscreen>(NULL, get_global_preferences_get_fullscreen_module_offset());

	unsigned long get_global_preferences_set_fullscreen_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00098AF0;
			//case _product_version_ms26_332089: return 0x00098AB0;
			//case _product_version_ms27_373869: return 0x00098B30;
			//case _product_version_ms27_416138: return 0x00098E20;
			//case _product_version_ms28_430653: return 0x0009AD00;
			//case _product_version_ms28_454665: return 0x0009AD00;
			//case _product_version_ms28_479394: return 0x0009AD00;
			case _product_version_ms29_498295: return 0x0009BB00;
			case _product_version_ms29_530945: return 0x0009BB00;
			case _product_version_ms29_533032: return 0x0009B850;
			case _product_version_ms29_554482: return 0x0009BD80;
			case _product_version_ms29_571698: return 0x0009BB00;
			case _product_version_ms29_604673: return 0x0009BC00;
			//case _product_version_ms30_700255: return 0x0009C610;
		}
		return 0;
	}
	using t_global_preferences_set_fullscreen = void __fastcall(bool);
	t_global_preferences_set_fullscreen* global_preferences_set_fullscreen = module_pointer<t_global_preferences_set_fullscreen>(NULL, get_global_preferences_set_fullscreen_module_offset());

	unsigned long get_global_preferences_get_screen_resolution_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00098D80;
			//case _product_version_ms26_332089: return 0x00098D40;
			//case _product_version_ms27_373869: return 0x00098DC0;
			//case _product_version_ms27_416138: return 0x000990B0;
			//case _product_version_ms28_430653: return 0x0009AF90;
			//case _product_version_ms28_454665: return 0x0009AD90;
			//case _product_version_ms28_479394: return 0x0009AF90;
			case _product_version_ms29_498295: return 0x0009BE90;
			case _product_version_ms29_530945: return 0x0009BE90;
			case _product_version_ms29_533032: return 0x0009BBE0;
			case _product_version_ms29_554482: return 0x0009C110;
			case _product_version_ms29_571698: return 0x0009BE90;
			case _product_version_ms29_604673: return 0x0009BF90;
			//case _product_version_ms30_700255: return 0x0009C9A0;
		}
		return 0;
	}
	using t_global_preferences_get_screen_resolution = void __fastcall(unsigned long&, unsigned long&);
	t_global_preferences_get_screen_resolution* global_preferences_get_screen_resolution = module_pointer<t_global_preferences_get_screen_resolution>(NULL, get_global_preferences_get_screen_resolution_module_offset());

	unsigned long get_global_preferences_set_screen_resolution_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x00098E20;
			//case _product_version_ms26_332089: return 0x00098DE0;
			//case _product_version_ms27_373869: return 0x00098E60;
			//case _product_version_ms27_416138: return 0x00099150;
			//case _product_version_ms28_430653: return 0x0009B030;
			//case _product_version_ms28_454665: return 0x0009B030;
			//case _product_version_ms28_479394: return 0x0009B030;
			case _product_version_ms29_498295: return 0x0009BF30;
			case _product_version_ms29_530945: return 0x0009BF30;
			case _product_version_ms29_533032: return 0x0009BC80;
			case _product_version_ms29_554482: return 0x0009C1B0;
			case _product_version_ms29_571698: return 0x0009BF30;
			case _product_version_ms29_604673: return 0x0009C030;
			//case _product_version_ms30_700255: return 0x0009CA40;
		}
		return 0;
	}
	using t_global_preferences_set_screen_resolution = void __fastcall(unsigned long, unsigned long);
	t_global_preferences_set_screen_resolution* global_preferences_set_screen_resolution = module_pointer<t_global_preferences_set_screen_resolution>(NULL, get_global_preferences_set_screen_resolution_module_offset());

	unsigned long get_force_display_update_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x0021C750;
			//case _product_version_ms26_332089: return 0x0021C6B0;
			//case _product_version_ms27_373869: return 0x0021C900;
			//case _product_version_ms27_416138: return 0x0021CB70;
			//case _product_version_ms28_430653: return 0x0021F0B0;
			//case _product_version_ms28_454665: return 0x0021F0B0;
			//case _product_version_ms28_479394: return 0x0021F0B0;
			case _product_version_ms29_498295: return 0x002204F0;
			case _product_version_ms29_530945: return 0x002204F0;
			case _product_version_ms29_533032: return 0x002202A0;
			case _product_version_ms29_554482: return 0x002207C0;
			case _product_version_ms29_571698: return 0x002205C0;
			case _product_version_ms29_604673: return 0x002206A0;
			//case _product_version_ms30_700255: return 0x002287C0;
		}
		return 0;
	}
	using t_force_display_update = void __stdcall(unsigned long, unsigned long);
	t_force_display_update* force_display_update = module_pointer<t_force_display_update>(NULL, get_force_display_update_module_offset());

	unsigned long get_global_preferences_set_hide_watermark_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		s_module_info module_info = {};
		module_info_get(module_info);
		switch (module_info.product_version.value)
		{
			//case _product_version_ms26_301003: return 0x0009A630;
			//case _product_version_ms26_332089: return 0x0009A5D0;
			//case _product_version_ms27_373869: return 0x0009A5C0;
			//case _product_version_ms27_416138: return 0x0009A8B0;
			//case _product_version_ms28_430653: return 0x0009C790;
			//case _product_version_ms28_454665: return 0x0009C790;
			//case _product_version_ms28_479394: return 0x0009C790;
			case _product_version_ms29_498295: return 0x0009D6F0;
			case _product_version_ms29_530945: return 0x0009D6F0;
			case _product_version_ms29_533032: return 0x0009D440;
			case _product_version_ms29_554482: return 0x0009D970;
			case _product_version_ms29_571698: return 0x0009D6F0;
			case _product_version_ms29_604673: return 0x0009D800;
			//case _product_version_ms30_700255: return 0x0009E200;
		}
		return 0;
	}
	using t_global_preferences_set_hide_watermark = void __fastcall(bool);
	t_global_preferences_set_hide_watermark* global_preferences_set_hide_watermark = module_pointer<t_global_preferences_set_hide_watermark>(NULL, get_global_preferences_set_hide_watermark_module_offset());

	void get_virtual_files_offsets(unsigned long* out_vfiles_buffer_offset, unsigned long* out_vfiles_offset)
	{
		console_print("%s\n", __FUNCTION__);

		unsigned long vfiles_buffer_offset = 0;
		unsigned long vfiles_offset = 0;

		char* memory = module_memory(NULL);

		c_vector<unsigned long> references = find_all_references('vfsl');
		for (unsigned long reference_index = 0; reference_index < references.size(); reference_index++)
		{
			char* reference = module_pointer<char>(NULL, references[reference_index]);

			char* start = reference;
			while (*start != '\xE8')
			{
				start++;
			}

			char* end = start;
			bool found_end = false;
			while (!found_end)
			{
				if ((end[0] == '\xC2' && end[3] == '\xCC') || (end[0] == '\xC3' && end[1] == '\xCC'))
					found_end = true;
				end++;
			}

			c_vector<unsigned long> calls;

			end += *end == '\xC2' ? 3 : *end == '\xC3' ? 1 : 0;
			for (char* pos = start; start < end; start = pos++)
			{
				if (*pos == '\xE8')
					calls.push(pos - memory);
			}

			if (!calls.size() || calls.size() < 2)
				break;

			if (call_to_function_offset(calls[calls.size() - 1]) == call_to_function_offset(calls[calls.size() - 2]))
			{
				vfiles_buffer_offset = push_to_data_offset(calls[calls.size() - 2] - 5);
				vfiles_offset = push_to_data_offset(calls[calls.size() - 1] - 5);
				
				break;
			}
		}

		if (out_vfiles_buffer_offset)
			*out_vfiles_buffer_offset = vfiles_buffer_offset;
		if (out_vfiles_offset)
			*out_vfiles_offset = vfiles_offset;
	}

	unsigned long get_virtual_files_buffer_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		unsigned long result = 0;
		get_virtual_files_offsets(&result, nullptr);

		return result;
	}
	char* virtual_files_buffer = module_pointer<char>(NULL, get_virtual_files_buffer_module_offset());

	unsigned long get_virtual_files_module_offset()
	{
		console_print("%s\n", __FUNCTION__);

		unsigned long result = 0;
		get_virtual_files_offsets(nullptr, &result);

		return result;
	}
	s_vfile_info* virtual_files = module_pointer<s_vfile_info>(NULL, get_virtual_files_module_offset()); // 512 files


	using t_game_options_ctor = void __fastcall(s_game_options& options);
	t_game_options_ctor* game_options_ctor = nullptr;
	using t_game_options_new = void __fastcall(s_game_options& options);
	t_game_options_new* game_options_new = nullptr;
	using t_game_options_setup_default_players = void __fastcall(long player_count, s_game_options& options);
	t_game_options_setup_default_players* game_options_setup_default_players = nullptr;
	using t_game_variant_ctor = void __fastcall(s_game_variant& game_variant);
	t_game_variant_ctor* game_variant_ctor = nullptr;
	using t_gui_game_setup_storage_map_variant_valid = bool __fastcall(s_map_variant& map_variant);
	t_gui_game_setup_storage_map_variant_valid* gui_game_setup_storage_map_variant_valid = nullptr;
	using t_map_variant_create_default = void __fastcall(s_map_variant& map_variant, unsigned long map_id);
	t_map_variant_create_default* map_variant_create_default = nullptr;
	using t_main_game_change = void __fastcall(s_game_options& options);
	t_main_game_change* main_game_change = nullptr;
	s_game_options* main_game_pending_game_options = nullptr;

	void populate_game_load_map_offsets()
	{
		console_print("%s\n", __FUNCTION__);

		{
			c_vector<unsigned long> references0 = get_all_strings_startswith("levels\\solo\\s3d_tutorial\\s3d_tutorial");
			if (!references0.size())
				return;

			unsigned long reference0 = references0[0];
			char* data0 = module_pointer<char>(NULL, reference0);
			unsigned long data_addr0 = reinterpret_cast<unsigned long>(data0);

			c_vector<unsigned long> references1 = find_all_references(data_addr0);
			if (!references1.size())
				return;

			unsigned long reference1 = references1[0];
			char* data1 = module_pointer<char>(NULL, reference1);
			unsigned long data_addr1 = reinterpret_cast<unsigned long>(data1);

			char* function_start = data1;
			char* function_end = data1;

			while (true)
			{
				if (function_start[0] == '\xCC' && function_start[1] == '\x55')
					break;

				function_start--;
			}
			function_start++;

			while (true)
			{
				if (function_end[0] == '\xC3' && function_end[1] == '\xCC')
					break;

				function_end++;
			}
			function_end++;

			c_vector<unsigned long> calls;
			{
				char* start = function_start;
				char* end = function_end;
				for (char* result = nullptr; start < end; start = result)
				{
					result = find_pattern(start, end - start, "\xE8\x00\x00\x00\xFF", "x???x");
					if (!result)
						break;

					calls.push(module_address_to_offset(result));

					result++;
				}
			}

			if (calls.size() == 7)
			{
				game_options_ctor = module_pointer<t_game_options_ctor>(NULL, call_to_function_offset(calls[0]));
				game_options_new = module_pointer<t_game_options_new>(NULL, call_to_function_offset(calls[1]));
				game_options_setup_default_players = module_pointer<t_game_options_setup_default_players>(NULL, call_to_function_offset(calls[2]));
				game_variant_ctor = module_pointer<t_game_variant_ctor>(NULL, call_to_function_offset(calls[3]));
				gui_game_setup_storage_map_variant_valid = module_pointer<t_gui_game_setup_storage_map_variant_valid>(NULL, call_to_function_offset(calls[4]));
				map_variant_create_default = module_pointer<t_map_variant_create_default>(NULL, call_to_function_offset(calls[5]));
				main_game_change = module_pointer<t_main_game_change>(NULL, call_to_function_offset(calls[6]));

				main_game_pending_game_options = module_pointer<s_game_options>(NULL, push_to_data_offset(call_to_function_offset(calls[7]) + 0xD));
				return;
			}
			else if (calls.size() == 8)
			{
				game_options_ctor = module_pointer<t_game_options_ctor>(NULL, call_to_function_offset(calls[0]));
				game_options_new = module_pointer<t_game_options_new>(NULL, call_to_function_offset(calls[1]));
				game_options_setup_default_players = module_pointer<t_game_options_setup_default_players>(NULL, call_to_function_offset(calls[3]));
				game_variant_ctor = module_pointer<t_game_variant_ctor>(NULL, call_to_function_offset(calls[4]));
				gui_game_setup_storage_map_variant_valid = module_pointer<t_gui_game_setup_storage_map_variant_valid>(NULL, call_to_function_offset(calls[5]));
				map_variant_create_default = module_pointer<t_map_variant_create_default>(NULL, call_to_function_offset(calls[6]));
				main_game_change = module_pointer<t_main_game_change>(NULL, call_to_function_offset(calls[7]));

				main_game_pending_game_options = module_pointer<s_game_options>(NULL, push_to_data_offset(call_to_function_offset(calls[7]) + 0xD));
				return;
			}
			console_print("");
		}

		char tutorial_scenario_path[] = "levels\\solo\\s3d_tutorial\\s3d_tutorial";
		unsigned long tutorial_scenario_path_size = _countof(tutorial_scenario_path);

		char* mask = new char[tutorial_scenario_path_size + 1]{};
		::memset(mask, 'x', tutorial_scenario_path_size);

		char* memory = module_memory(NULL);

		unsigned long tutorial_scenario_path_offset = module_offset_from_pattern(NULL, tutorial_scenario_path, mask);
		tutorial_scenario_path_offset += reinterpret_cast<unsigned long>(memory);

		char* launch_tutorial_pattern = new char[5]{};
		*launch_tutorial_pattern = '\xB8'; // push offset tutorial_scenario_path
		::memcpy(launch_tutorial_pattern + 1, &tutorial_scenario_path_offset, 4);

		unsigned long launch_tutorial_offset = module_offset_from_pattern(NULL, launch_tutorial_pattern, "xxxxx") - 0x4C;
		//launch_tutorial_offset += reinterpret_cast<unsigned long>(memory);

		game_options_ctor = module_pointer<t_game_options_ctor>(NULL, call_to_function_offset(launch_tutorial_offset + 0x2B));
		game_options_new = module_pointer<t_game_options_new>(NULL, call_to_function_offset(launch_tutorial_offset + 0x3F));
		game_options_setup_default_players = module_pointer<t_game_options_setup_default_players>(NULL, call_to_function_offset(launch_tutorial_offset + 0xE6));
		game_variant_ctor = module_pointer<t_game_variant_ctor>(NULL, call_to_function_offset(launch_tutorial_offset + 0xF2));
		gui_game_setup_storage_map_variant_valid = module_pointer<t_gui_game_setup_storage_map_variant_valid>(NULL, call_to_function_offset(launch_tutorial_offset + 0xFE));
		map_variant_create_default = module_pointer<t_map_variant_create_default>(NULL, call_to_function_offset(launch_tutorial_offset + 0x115));
		main_game_change = module_pointer<t_main_game_change>(NULL, call_to_function_offset(launch_tutorial_offset + 0x11E));
		main_game_pending_game_options = module_pointer<s_game_options>(NULL, push_to_data_offset((reinterpret_cast<unsigned long>(main_game_change) - (reinterpret_cast<unsigned long>(memory)) + 0xD)));
	}
}

char* get_command_line()
{
	char* command_line = nullptr;
	if (halo_game::command_line)
		command_line = *halo_game::command_line;

	return command_line;
}

void game_update_window(unsigned long width, unsigned long height, bool windowed)
{
	halo_game::global_preferences_set_fullscreen(windowed == false);
	halo_game::global_preferences_set_screen_resolution(width, height);
	halo_game::force_display_update(width, height);
}

namespace patches
{
	namespace backend_session
	{
		s_module_patch* patch_offline_value;
		s_module_patch* patch_online_value;

		void toggle_all()
		{
			if (patches::backend_session::patch_offline_value)
				patches::backend_session::patch_offline_value->toggle();
			if (patches::backend_session::patch_online_value)
				patches::backend_session::patch_online_value->toggle();
		}
	}
}

void dump_virtual_files()
{
	patches::backend_session::toggle_all();

	c_path path{};
	c_path relative_path{};
	for (long i = 0; i < 512; i++)
	{
		::memset(path, 0, sizeof(path));
		::memset(relative_path, 0, sizeof(relative_path));

		s_vfile_info& vfile = halo_game::virtual_files[i];
		if (*vfile.name && *vfile.folder && vfile.size)
		{
			char* vfile_buffer = halo_game::virtual_files_buffer + vfile.offset;

			snprintf(relative_path, MAX_PATH, "virtual_files\\%s", vfile.folder);
			char* _fullpath_result = _fullpath(path, relative_path, _countof(path));

			int SHCreateDirectoryExA_result = SHCreateDirectoryExA(NULL, path, NULL);
			if (SHCreateDirectoryExA_result == ERROR_SUCCESS || SHCreateDirectoryExA_result == ERROR_ALREADY_EXISTS)
			{
				strcat_s(path, vfile.name);
				write_data_to_file(vfile_buffer, vfile.size, path);
			}
		}
	}

	patches::backend_session::toggle_all();
}
void game_load_map(const char* scenario_path, c_enum<e_game_mode, unsigned long> game_mode, c_enum<e_game_engine_variant, unsigned long> game_engine_variant = _game_engine_base_variant)
{
	static char* data = new char[1 << 20]{};
	s_game_options& options = *reinterpret_cast<s_game_options*>(data);

	halo_game::game_options_ctor(options);
	halo_game::game_options_new(options);
	options.game_mode = game_mode;
	csstrnzcpy(options.get_scenario_path(), scenario_path, MAX_PATH);
	halo_game::game_options_setup_default_players(1, options);

	s_game_variant& game_variant = options.get_game_variant();

	if (game_mode == _game_mode_multiplayer)
	{
		// if 'game_engine_variant' is set to `_game_engine_base_variant` the game will crash
		if (game_engine_variant == _game_engine_base_variant)
			game_engine_variant = _game_engine_slayer_variant;

		game_variant.game_engine_variant = game_engine_variant;

		s_game_engine_variant_general_settings& general_settings = game_variant.get_general_settings();
		general_settings.flags |= (1 << _game_engine_variant_general_setting_teams_enabled);
		general_settings.time_limit = 0;

		s_game_engine_variant_respawn_settings& respawn_settings = game_variant.get_respawn_settings();
		respawn_settings.respawn_time = 0;
		respawn_settings.suicide_penalty = 0;

		s_game_engine_variant_social_settings& social_settings = game_variant.get_social_settings();
		social_settings.team_changing = 1;
	}

	halo_game::game_variant_ctor(game_variant);

	s_map_variant& map_variant = options.get_map_variant();
	if (!halo_game::gui_game_setup_storage_map_variant_valid(map_variant))
		halo_game::map_variant_create_default(map_variant, map_variant.get_map_id());

	halo_game::main_game_change(options);
}

void game_reload_map(void* userdata = nullptr)
{
	if (halo_game::main_game_pending_game_options)
		halo_game::main_game_change(*halo_game::main_game_pending_game_options);
}

void game_load_map_dialog(void* userdata = nullptr)
{
	c_path scenario_path{};
	unsigned long cache_file_type = -1;

	COMDLG_FILTERSPEC file_filter[] = { { L"Map File", L"*.map" } };
	s_file_open_dialog_parameters parameters = { L"maps", L"mainmenu.map", file_filter, _countof(file_filter), L"Load Map" };

	ShowCursor(TRUE);
	file_open_dialog(scenario_path, parameters);
	ShowCursor(FALSE);

	s_cache_file_header* cache_file_header = nullptr;
	if (read_structure_from_file(&cache_file_header, scenario_path))
	{
		csstrnzcpy(scenario_path, cache_file_header->get_scenario_path(), MAX_PATH);

		cache_file_type = cache_file_header->get_cache_file_type();
		if (strcmp(cache_file_header->get_scenario_name(), "mainmenu") == 0)
			cache_file_type = _cache_file_type_ui;

		delete cache_file_header;
	}

	unsigned long game_mode = cache_file_type_to_game_mode(cache_file_type);
	unsigned long game_engine_variant = cache_file_type_to_game_engine_variant(cache_file_type);

	game_load_map(scenario_path, game_mode, game_engine_variant);
}
