#pragma once

namespace patches
{
	/*
	namespace bink
	{
		s_module_patch* bink_format_string_patch = patch_memset(NULL, module_offset_from_pattern(NULL, "bink\\%s.bik", "xxxxxxxxxxx"), '_', sizeof('_'));

		void toggle_all()
		{
			if (bink_format_string_patch)
				bink_format_string_patch->toggle();
		}
	}
	*/

	namespace contrails
	{
		unsigned long get_contrail_render_patch_module_offset()
		{
			printf("%s\n", __FUNCTION__);

			char pattern[] = "\x8A\x86\x2C\x01\x00\x00\xC0\xE8\x07\x89\x7D\xFC\xA8\x01";
			c_vector<unsigned long> references = find_all_references_with_length(pattern, _countof(pattern));

			if (references.size())
			{
				unsigned long reference0 = references[0];
				char* data0 = module_pointer<char>(NULL, reference0);

				while (data0[0] != '\xCC')
				{
					if (data0[1] != '\x55')
						data0--;

					data0--;
				}
				data0 += 2;

				unsigned long memory_size = 0;
				char* memory = module_memory(NULL, &memory_size);

				c_vector<unsigned long> calls;

				char* start = memory;
				char* end = memory + reference0;
				for (char* result = nullptr; start < end; start = result)
				{
					result = find_pattern(start, end - start, "\xE8\x00\x00\x00\x00", "x????");
					if (!result)
						break;

					calls.push(result - memory);
					result += 5;
				}

				char* call_addr = nullptr;

				for (unsigned long call_index = 0; call_index < calls.size(); call_index++)
				{
					unsigned long call_offset = calls[call_index];
					char* call = module_pointer<char>(NULL, reference0);

					unsigned long function_offset = call_to_function_offset(call_offset);

					if (function_offset == data0 - memory)
					{
						call_addr = module_pointer<char>(NULL, call_offset);
					}
				}

				if (call_addr)
				{
					char* function_addr = call_addr;
					while (function_addr[0] != '\xCC')
					{
						if (function_addr[1] != '\x55')
							function_addr--;

						function_addr--;
					}
					function_addr += 2;

					return module_address_to_offset(function_addr);
				}
			}

			return 0;
		}

		// contrail_render: return
		s_module_patch* contrail_render_patch = patch_memset(NULL, get_contrail_render_patch_module_offset(), 0xC3, 1);

		void toggle_all()
		{
			if (contrail_render_patch)
				contrail_render_patch->toggle();
		}
	}

	namespace director
	{
		unsigned long get_director_render_patch_module_offset()
		{
			printf("%s\n", __FUNCTION__);

			c_vector<unsigned long> references = get_all_strings_startswith("pan-cam");

			if (!references.size())
				return 0;

			unsigned long reference0 = references[0];
			char* data0 = module_pointer<char>(NULL, reference0);
			unsigned long data_addr0 = reinterpret_cast<unsigned long>(data0);

			c_vector<unsigned long> references1 = find_all_references(data_addr0);
			if (!references1.size())
				return 0;

			unsigned long reference1 = references1[0];
			char* data1 = module_pointer<char>(NULL, reference1);
			unsigned long data_addr1 = reinterpret_cast<unsigned long>(data1);

			c_vector<unsigned long> call_test_jz;
			{
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

				char* start = function_start;
				char* end = function_end;
				for (char* result = nullptr; start < end; start = result)
				{
					result = find_pattern(start, end - start, "\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00", "x????xxxx????");
					if (!result)
						break;

					call_test_jz.push(module_address_to_offset(result));
					result += sizeof("x????xxxx????");
				}
			}

			if (!call_test_jz.size())
				return 0;

			char* data2 = data1;
			while (data2[0] != '\x0F')
			{
				if (data2[1] != '\x84')
					data2++;

				data2++;
			}

			char* jump_addr = module_pointer<char>(NULL, call_test_jz[0]);
			if (*jump_addr == '\xE8')
			{
				jump_addr += 5; // call    player_control_get_machinima_camera_debug
				jump_addr += 2; // test    al, al
			}

			return module_address_to_offset(jump_addr);
		}

		// director_render: if (player_control_get_machinima_camera_debug()) return;
		s_module_patch* director_render_patch = patch_memset(NULL, get_director_render_patch_module_offset(), 0x90, 6, false);

		void toggle_all()
		{
			if (director_render_patch)
				director_render_patch->toggle();
		}
	}

	namespace language
	{
		void get_language_patch_offsets(unsigned long* out_patch0_offset, unsigned long* out_patch1_offset)
		{
			printf("%s\n", __FUNCTION__);

			static c_vector<unsigned long> patch_offsets;

			if (!patch_offsets.size())
			{
				c_vector<unsigned long> references0 = get_all_wstrings_startswith(L"LADEN");
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

				while (*data1 != '\xE8')
				{
					data1--;
				}

				unsigned long function_offset = call_to_function_offset(data1);
				unsigned long*& data2 = module_reference<unsigned long*>(NULL, function_offset + 1);

				c_vector<unsigned long> references2 = find_all_references(data2);
				for (unsigned long reference2_index = 0; reference2_index < references2.size(); reference2_index++)
				{
					unsigned long reference2 = references2[reference2_index];
					char* data3 = module_pointer<char>(NULL, reference2);

					if (data3[4] == '\x0B')
					{
						patch_offsets.push(module_address_to_offset(data3 + 4));
					}
				}
			}

			if (patch_offsets.size() == 2)
			{
				if (out_patch0_offset)
					*out_patch0_offset = patch_offsets[0];
				if (out_patch1_offset)
					*out_patch1_offset = patch_offsets[1];
			}
		}

		unsigned long get_language_patch0_module_offset()
		{
			printf("%s\n", __FUNCTION__);

			unsigned long result = 0;
			get_language_patch_offsets(&result, nullptr);

			return result;
		}

		unsigned long get_language_patch1_module_offset()
		{
			printf("%s\n", __FUNCTION__);

			unsigned long result = 0;
			get_language_patch_offsets(nullptr, &result);

			return result;
		}

		s_module_patch* language_patch0 = patch_memset(NULL, get_language_patch0_module_offset(), k_default_language, 1ul);
		s_module_patch* language_patch1 = patch_memset(NULL, get_language_patch1_module_offset(), k_default_language, 1ul);

		void toggle_all()
		{
			if (language_patch0)
				language_patch0->toggle();
			if (language_patch1)
				language_patch1->toggle();
		}
	}

	namespace watermark
	{
		unsigned long get_game_engine_render_watermarks_patch_module_offset()
		{
			printf("%s\n", __FUNCTION__);

			c_vector<unsigned long> references = get_all_wstrings_startswith(L"%s|nBuild %s %s %s|n%s");
			if (!references.size())
				get_all_wstrings_startswith(L"%s|n%s|n%s %s %s|n%s", &references);

			if (references.size())
			{
				unsigned long reference0 = references[0];
				char* data0 = module_pointer<char>(NULL, reference0);
				unsigned long data_addr0 = reinterpret_cast<unsigned long>(data0);

				c_vector<unsigned long> references1 = find_all_references(data_addr0);
				if (!references1.size())
					return 0;

				unsigned long reference1 = references1[0];
				char* data1 = module_pointer<char>(NULL, reference1);
				unsigned long data_addr1 = reinterpret_cast<unsigned long>(data1);

				while (data1[0] != '\xCC')
				{
					if (data1[1] != '\x55')
						data1--;

					data1--;
				}
				data1 += 2;

				return module_address_to_offset(data1);
			}

			return 0;
		}

		// game_engine_render_watermarks: return;
		s_module_patch* game_engine_render_watermarks_patch = patch_memset(NULL, get_game_engine_render_watermarks_patch_module_offset(), 0xC3, 1, false);

		void toggle_all()
		{
			if (game_engine_render_watermarks_patch)
				game_engine_render_watermarks_patch->toggle();
		}
	}
}

void game_set_language(c_enum<e_language, unsigned char> selected_language = k_default_language)
{
	if (patches::language::language_patch0)
		vmemset(patches::language::language_patch0->address, selected_language, sizeof(selected_language));
	if (patches::language::language_patch1)
		vmemset(patches::language::language_patch1->address, selected_language, sizeof(selected_language));
}

void backend_session_bypass()
{
	printf("%s\n", __FUNCTION__);

	const char* backend_session_offline_find = "   var BACKEND_SESSION_OFFLINE                  : int    = 0";
	const char* backend_session_online_find  = "   var BACKEND_SESSION_ONLINE                   : int    = 3";

	unsigned long backend_session_offline_size = static_cast<unsigned long>(strlen(backend_session_offline_find));
	unsigned long backend_session_online_size  = static_cast<unsigned long>(strlen(backend_session_online_find));

	if (backend_session_offline_size != backend_session_online_size)
		return;

	while (halo_game::virtual_files_buffer)
	{
		if (*halo_game::virtual_files_buffer)
		{
			for (long i = 0; i < 512; i++)
			{
				s_vfile_info& vfile = halo_game::virtual_files[i];
				if (*vfile.name && *vfile.folder && vfile.size)
				{
					char* vfile_buffer = halo_game::virtual_files_buffer + vfile.offset;

					if (strcmp(vfile.name, "ui_globals.ssl") != 0)
						continue;

					char* backend_session_offline = strstr(vfile_buffer, backend_session_offline_find);
					char* backend_session_online = strstr(vfile_buffer, backend_session_online_find);

					if (!backend_session_offline || !backend_session_online)
						return;

					char backend_session_offline_value = backend_session_offline[(backend_session_offline_size - 1)];
					char backend_session_online_value = backend_session_online[(backend_session_online_size - 1)];

					unsigned long backend_session_offline_value_offset = (backend_session_offline + (backend_session_offline_size - 1)) - module_memory(NULL);
					unsigned long backend_session_online_value_offset = (backend_session_online + (backend_session_online_size - 1)) - module_memory(NULL);

					patches::backend_session::patch_offline_value = patch_memset(NULL, backend_session_offline_value_offset, backend_session_online_value, sizeof(backend_session_online_value));
					patches::backend_session::patch_online_value = patch_memset(NULL, backend_session_online_value_offset, backend_session_offline_value, sizeof(backend_session_offline_value));
				}
			}

			break;
		}

		Sleep(200);
	}
}
