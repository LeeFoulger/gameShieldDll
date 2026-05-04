#pragma once

class c_runtime_patch_manager
{
public:
	c_runtime_patch_manager() :
		patch_count(0),
		patches()
	{
	}

	~c_runtime_patch_manager()
	{
		dispose();
	}

	void add(s_module_patch* module_patch)
	{
		patches[patch_count] = module_patch;
	}

	void dispose()
	{
		for (long patch_index = 0; patch_index < patch_count; patch_index++)
		{
			s_module_patch* patch = patches[patch_index];
			delete patch;
			patch = nullptr;
		}
	}

private:
	long patch_count;
	s_module_patch* patches[1024];
};

static c_runtime_patch_manager g_runtime_patch_manager;

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

void simple_patch_setup(char* patch, unsigned long patch_size, short file_version, short file_type)
{
	assert(patch != nullptr);
	assert(patch_size >= sizeof(s_simple_patch_file_header));

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	header->header_signature = 'head';
	header->file_version = file_version;
	header->file_type = file_type;
	header->module_offset = NONE;
	header->footer_signature = 'foot';
}

void simple_patch_set_name(char* patch, const char* name)
{
	assert(patch != nullptr);
	assert(name != nullptr);

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	csstrnzcpy(header->name, name, sizeof(header->name));
}

void simple_patch_set_description(char* patch, const char* description)
{
	assert(patch != nullptr);
	assert(description != nullptr);

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	csstrnzcpy(header->description, description, sizeof(header->description));
}

void simple_patch_set_pattern(char* patch, unsigned long& file_size, const char* pattern, unsigned long pattern_size)
{
	assert(patch != nullptr);
	assert(pattern != nullptr);
	assert(pattern_size > 0);

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	header->pattern_offset = file_size;
	file_size += ALIGN(pattern_size, simple_patch_file_alignment_bits);
	csstrnzcpy(patch + header->pattern_offset, pattern, file_size - header->pattern_offset);
}

void simple_patch_set_mask(char* patch, unsigned long& file_size, const char* mask, unsigned long mask_size)
{
	assert(patch != nullptr);
	assert(mask != nullptr);
	assert(mask_size > 0);

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	header->mask_offset = file_size;
	file_size += ALIGN(mask_size, simple_patch_file_alignment_bits);
	csstrnzcpy(patch + header->mask_offset, mask, file_size - header->mask_offset);
}

void simple_patch_set_data(char* patch, unsigned long& file_size, unsigned char* data, unsigned long data_size)
{
	assert(patch != nullptr);
	assert(data != nullptr);
	assert(data_size > 0);

	s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);

	header->data_offset = file_size;
	header->data_size = data_size;
	file_size += ALIGN(header->data_size, simple_patch_file_alignment_bits);
	memcpy(patch + header->data_offset, data, header->data_size);
	header->file_size = file_size;
}

void simple_patch_write_file(char* file_data, unsigned long file_size, const char* filename)
{
	assert(file_data != nullptr);
	assert(file_size > 0);
	assert(filename != nullptr);

	c_path filepath, dll_dir;
	GetModuleFileNameA(GetModuleHandleA(DLL_NAME), dll_dir, sizeof(dll_dir));
	PathRemoveFileSpecA(dll_dir);
	sprintf_s(filepath, "%s\\patches\\", dll_dir);
	if (PathFileExistsA(filepath) == FALSE)
	{
		CreateDirectoryA(filepath, NULL);
	}
	strcat_s(filepath, filename);

	write_data_to_file(file_data, file_size, filepath);
}

void simple_patch_read_files_from_folder(const char* folder_name)
{
	assert(folder_name != nullptr);

	c_path find_str{};
	sprintf_s(find_str, "%s\\*.patch", folder_name);

	c_vector<char*> filenames;
	find_files(find_str, filenames);

	for (unsigned long filename_index = 0; filename_index < filenames.size(); filename_index++)
	{
		char* filename = filenames[filename_index];

		char* patch = nullptr;
		unsigned long patch_size = 0;

		read_data_from_file(&patch, &patch_size, filename);
		if (patch != nullptr)
		{
			s_simple_patch_file_header* header = reinterpret_cast<decltype(header)>(patch);
			if (patch_size == header->file_size)
			{
				if (header->header_signature == 'head' && header->footer_signature == 'foot')
				{
					if (header->file_version == 1)
					{
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
							s_module_patch* module_patch = module_patch_create_memset(NULL, module_offset, *data, data_size);
							g_runtime_patch_manager.add(module_patch);
							break;
						}
						case _simple_patch_file_type_memcpy:
						{
							s_module_patch* module_patch = module_patch_create_memcpy(NULL, module_offset, data, data_size);
							g_runtime_patch_manager.add(module_patch);
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
			}
		}
	}
}

