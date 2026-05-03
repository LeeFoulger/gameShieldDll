#pragma once

namespace runtime_patch_manager
{
	const long simple_patch_file_alignment_bits = 2;

	enum e_simple_patch_file_type
	{
		_simple_patch_file_type_none = 0,
		_simple_patch_file_type_memset,
		_simple_patch_file_type_memcpy,

		k_simple_patch_file_type_count
	};

	struct s_simple_patch_file_header
	{
		unsigned long header_signature;

		unsigned long file_size;
		short file_version;
		short file_type;
		long : 32;

		unsigned long pattern_offset;
		unsigned long mask_offset;

		unsigned long data_offset;
		unsigned long data_size;

		c_string<char, 32> name;
		c_string<char, 48> description;

		unsigned long module_offset;
		long : 32;
		long : 32;

		unsigned long footer_signature;
	};


	long patch_count;
	s_module_patch* patches[1024];

	void add(s_module_patch* module_patch)
	{
		patches[patch_count] = module_patch;
	}

	void simple_patch_setup(s_simple_patch_file_header* header, short file_version, short file_type)
	{
		header->header_signature = 'head';
		header->file_version = file_version;
		header->file_type = file_type;
		header->module_offset = NONE;
		header->footer_signature = 'foot';
	}

	void simple_patch_set_name(s_simple_patch_file_header* header, const char* name)
	{
		csstrnzcpy(header->name, name, sizeof(header->name));
	}

	void simple_patch_set_description(s_simple_patch_file_header* header, const char* description)
	{
		csstrnzcpy(header->description, description, sizeof(header->description));
	}

	void simple_patch_set_pattern(char* patch, s_simple_patch_file_header* header, unsigned long& file_size, const char* pattern)
	{
		header->pattern_offset = file_size;
		file_size += ALIGN(strlen(pattern), simple_patch_file_alignment_bits);
		csstrnzcpy(patch + header->pattern_offset, pattern, file_size - header->pattern_offset);
	}

	void simple_patch_set_mask(char* patch, s_simple_patch_file_header* header, unsigned long& file_size, const char* mask)
	{
		header->mask_offset = file_size;
		file_size += ALIGN(strlen(mask), simple_patch_file_alignment_bits);
		csstrnzcpy(patch + header->mask_offset, "xxxxxxxxxxx", file_size - header->mask_offset);
	}

	void simple_patch_set_data(char* patch, s_simple_patch_file_header* header, unsigned long& file_size, unsigned char* data, unsigned long data_size)
	{
		header->data_offset = file_size;
		header->data_size = data_size;
		file_size += ALIGN(header->data_size, simple_patch_file_alignment_bits);
		memcpy(patch + header->data_offset, data, header->data_size);
		header->file_size = file_size;
	}

	// TODO: make a separate generic patch generator
	void write_patch_file(const char* filename)
	{
		char patch[sizeof(s_simple_patch_file_header) + 0x400]{};
		s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

		unsigned long file_size = sizeof(*header);

		unsigned char data[] = "_";
		simple_patch_setup(header, 1, _simple_patch_file_type_memset);
		simple_patch_set_name(header, "bink format string");
		simple_patch_set_description(header, "skip the intro video files");
		simple_patch_set_pattern(patch, header, file_size, "bink\\%s.bik");
		simple_patch_set_mask(patch, header, file_size, "xxxxxxxxxxx");
		simple_patch_set_data(patch, header, file_size, data, sizeof(data) - 1);

		c_path filepath, dll_dir;
		GetModuleFileNameA(GetModuleHandleA(DLL_NAME), dll_dir, sizeof(dll_dir));
		PathRemoveFileSpecA(dll_dir);
		sprintf_s(filepath, "%s\\patches\\", dll_dir);
		if (PathFileExistsA(filepath) == FALSE)
		{
			CreateDirectoryA(filepath, NULL);
		}
		strcat_s(filepath, filename);

		write_data_to_file(patch, file_size, filepath);
	}

	void read_files_from_folder(const char* folder_name)
	{
		c_path find_str{};
		sprintf_s(find_str, "%s\\*.patch", folder_name);

		c_vector<char*> files;
		find_files(find_str, &files);

		for (unsigned long i = 0; i < files.size(); i++)
		{
			char* filepath = files[i];

			char* patch = nullptr;
			long patch_size = 0;

			read_data_from_file(&patch, &patch_size, filepath);
			if (!patch)
			{
				continue;
			}

			s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);
			if (patch_size != header->file_size)
			{
				continue;
			}

			if (header->header_signature != 'head' || header->footer_signature != 'foot')
			{
				continue;
			}

			if (header->file_version != 1)
			{
				continue;
			}

			if (*header->name)
			{
				console_print("[Runtime Patch Manager] %s", header->name);

				if (*header->description)
				{
					console_print("(%s)", header->description);
				}

				console_print("\n");
			}

			const char* pattern = patch + header->pattern_offset;
			const char* mask = patch + header->mask_offset;

			const char* data = patch + header->data_offset;
			long data_size = header->data_size;

			unsigned long module_offset = header->module_offset;
			if (module_offset == NONE)
			{
				module_offset = module_offset_from_pattern(NULL, pattern, mask);
			}

			switch (header->file_type)
			{
			case _simple_patch_file_type_memset:
			{
				runtime_patch_manager::add(patch_memset(NULL, module_offset, *data, data_size));
				break;
			}
			case _simple_patch_file_type_memcpy:
			{
				runtime_patch_manager::add(patch_memcpy(NULL, module_offset, data, data_size));
				break;
			}
			default:
			{
				console_print("unknown patch type: %hi\n", header->file_type);
				break;
			}
			}
		}

		
	}

};

