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

		long file_size;
		short file_version;
		short file_type;
		long : 32;

		long pattern_offset;
		long mask_offset;

		long data_offset;
		long data_size;

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

	// TODO: make a separate generic patch generator
	void write_patch_file(const char* filename)
	{
		char patch[sizeof(s_simple_patch_file_header) + 0x400]{};
		s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

		long file_size = sizeof(*header);

		header->header_signature = 'head';

		header->file_version = 1;
		header->file_type = _simple_patch_file_type_memset;

		header->module_offset = 0xFFFFFFFF;

		csstrnzcpy(header->name, "bink format string", sizeof(header->name));
		csstrnzcpy(header->description, "skip the intro video files", sizeof(header->description));

		header->pattern_offset = file_size;
		file_size += ALIGN(sizeof("bink\\%s.bik"), simple_patch_file_alignment_bits);
		csstrnzcpy(patch + header->pattern_offset, "bink\\%s.bik", file_size - header->pattern_offset);

		header->mask_offset = file_size;
		file_size += ALIGN(sizeof("xxxxxxxxxxx"), simple_patch_file_alignment_bits);
		csstrnzcpy(patch + header->mask_offset, "xxxxxxxxxxx", file_size - header->mask_offset);

		header->data_offset = file_size;

		unsigned char data[] = "_";

		header->data_size = sizeof(data) - 1;
		file_size += ALIGN(header->data_size, simple_patch_file_alignment_bits);
		memcpy(patch + header->data_offset, &data, header->data_size);

		header->file_size = file_size;
		header->footer_signature = 'foot';

		c_path filepath, dll_dir;
		GetModuleFileNameA(GetModuleHandleA(DLL_NAME), dll_dir, sizeof(dll_dir));
		PathRemoveFileSpecA(dll_dir);
		sprintf_s(filepath, "%s\\patches\\%s", dll_dir, filename);

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
				continue;

			s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);
			if (patch_size != header->file_size)
				continue;

			if (header->header_signature != 'head' || header->footer_signature != 'foot')
			if (*header->name)
			{
				console_print("[Runtime Patch Manager] %s", header->name);

				if (*header->description)
					console_print("(%s)", header->description);

				console_print("\n");
			}

			const char* pattern = patch + header->pattern_offset;
			const char* mask = patch + header->mask_offset;

			const char* data = patch + header->data_offset;
			long data_size = header->data_size;

			unsigned long module_offset = header->module_offset;
			if (module_offset == 0xFFFFFFFF)
				module_offset = module_offset_from_pattern(NULL, pattern, mask);

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

		console_print("");
	}

};

