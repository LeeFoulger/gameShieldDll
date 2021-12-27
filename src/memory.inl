#pragma once

char* csstrnzcpy(char* s1, const char* s2, unsigned long size)
{
	unsigned long s2_size = static_cast<unsigned long>(strlen(s2));
	strncpy_s(s1, size, s2, 0xFFFFFFFF);
	if (s2_size >= size)
		s1[size - 1] = 0;
	else
		memset(&s1[s2_size], 0, (size - s2_size) * sizeof(char));
	return s1;
}

wchar_t* cswcsnzcpy(wchar_t* s1, const wchar_t* s2, unsigned long size)
{
	unsigned long s2_size = static_cast<unsigned long>(wcslen(s2));
	wcsncpy_s(s1, size, s2, 0xFFFFFFFF);
	if (s2_size >= size)
		s1[size - 1] = 0;
	else
		memset(&s1[s2_size], 0, (size - s2_size) * sizeof(wchar_t));
	return s1;
}

// based off https://github.com/B1nGh0/FindPattern/blob/709154d5bf0d6d31944c5df7f3d5c79cc87a7d2f/findpattern.c
char* find_pattern(char* start, unsigned long size, const char* pattern, const char* mask)
{
	unsigned long i, j;
	unsigned long len;
	unsigned char fnd = 0;

	if (!size || !pattern || !mask)
		return 0;

	if (!(len = strlen(mask)) || len > size)
		return 0;

	for (i = 0; !fnd && i <= size - len; i++)
	{
		for (fnd = 1, j = 0; fnd && j < len; j++)
		{
			fnd &= mask[j] == '?' || pattern[j] == (start)[i + j];
		}
	}

	return fnd ? start + i - 1 : nullptr;
}

void* vmemcpy(void* dst, const void* src, unsigned long size)
{
	DWORD protect;
	VirtualProtect(dst, size, PAGE_READWRITE, &protect);
	void* result = ::memcpy(dst, src, size);
	VirtualProtect(dst, size, protect, &protect);
	return result;
}

void* vmemmove(void* dst, const void* src, unsigned long size)
{
	DWORD protect;
	VirtualProtect(dst, size, PAGE_READWRITE, &protect);
	void* result = ::memmove(dst, src, size);
	VirtualProtect(dst, size, protect, &protect);
	return result;
}

void* vmemset(void* dst, long val, unsigned long size)
{
	DWORD protect;
	VirtualProtect(dst, size, PAGE_READWRITE, &protect);
	void* result = ::memset(dst, val, size);
	VirtualProtect(dst, size, protect, &protect);
	return result;
}

template<typename t_type>
t_type* typed_vmemcpy(t_type* dst, const t_type* src)
{
	return static_cast<t_type*>(::vmemcpy(dst, src, sizeof(t_type)));
}

template<typename t_type>
t_type* typed_vmemmove(t_type* dst, const t_type* src)
{
	return static_cast<t_type*>(::vmemmove(dst, src, sizeof(t_type)));
}

void module_get_nt_header(const wchar_t* module_name, PIMAGE_NT_HEADERS* out_nt_header)
{
	char* memory = reinterpret_cast<char*>(GetModuleHandle(module_name));

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(memory);
	if (dos_header && out_nt_header)
	{
		*out_nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>(memory + dos_header->e_lfanew);
	}
}

char* module_memory(const wchar_t* module_name, unsigned long* memory_size = nullptr)
{
	char* memory = reinterpret_cast<char*>(GetModuleHandle(module_name));

	if (memory_size)
	{
		PIMAGE_NT_HEADERS nt_header = nullptr;
		module_get_nt_header(module_name, &nt_header);

		if (nt_header)
			*memory_size = nt_header->OptionalHeader.SizeOfImage;
	}

	return memory;
}

template<typename t_type>
t_type* module_pointer(const wchar_t* module_name, unsigned long offset)
{
	char* module_address = module_memory(module_name) + offset;
	if (offset == 0 || module_address - offset == 0)
		return nullptr;

	return reinterpret_cast<t_type*>(module_address);
}

template<typename t_type>
t_type& module_reference(const wchar_t* module_name, unsigned long offset)
{
	return *reinterpret_cast<t_type*>(module_memory(module_name) + offset);
}

void* module_memcpy(const wchar_t* module_name, unsigned long offset, const void* src, unsigned long size)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address - offset == 0)
		return nullptr;

	return ::vmemcpy(module_address, src, size);
}

void* module_memmove(const wchar_t* module_name, unsigned long offset, const void* src, unsigned long size)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address - offset == 0)
		return nullptr;

	return ::vmemmove(module_address, src, size);
}

void* module_memset(const wchar_t* module_name, unsigned long offset, long val, unsigned long size)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address - offset == 0)
		return nullptr;

	return ::vmemset(module_address, val, size);
}

unsigned long module_offset_from_pattern(const wchar_t* module_name, const char* pattern, const char* mask)
{
	unsigned long memory_size = 0;
	char* memory = module_memory(module_name, &memory_size);
	char* result = find_pattern(memory, memory_size, pattern, mask);

	return result ? result - memory : 0;
}

struct s_module_patch
{
	void* address;

	void* old_data;
	void* new_data;
	unsigned long data_size;

	bool enabled;

	void* revert()
	{
		enabled = false;
		return ::vmemcpy(address, old_data, data_size);
	}
	void* enable()
	{
		enabled = true;
		return ::vmemcpy(address, new_data, data_size);
	}
	void toggle()
	{
		if (enabled)
			revert();
		else
			enable();
	}
};

s_module_patch* patch_memset(const wchar_t* module_name, unsigned long offset, long val, unsigned long size, bool enabled = true)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address == module_memory(module_name) || module_address - offset == 0)
		return nullptr;

	s_module_patch* module_patch = new s_module_patch();
	module_patch->address = module_address;
	module_patch->data_size = size;

	module_patch->old_data = new char[module_patch->data_size]{};
	::memcpy(module_patch->old_data, module_patch->address, module_patch->data_size);

	module_patch->new_data = new char[module_patch->data_size]{};
	::memset(module_patch->new_data, val, size);

	module_patch->enabled = enabled;
	if (module_patch->enabled)
		module_patch->enable();

	return module_patch;
}


unsigned long call_to_function_offset(char* call_addr);
unsigned long call_to_function_offset(unsigned long call_offset);

char* patch_call(const wchar_t* module_name, unsigned long offset, const void* src, bool enabled = true)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address == module_memory(module_name) || module_address - offset == 0)
		return nullptr;

	char* function_address = module_memory(module_name) + call_to_function_offset(offset);

	unsigned char temp_jump[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 };
	unsigned long jump_size = ((unsigned long)src - (unsigned long)module_address - 5);

	vmemcpy(&temp_jump[1], &jump_size, 4);
	vmemcpy(module_address, temp_jump, 5);

	return function_address;
}

s_module_patch* patch_memcpy(const wchar_t* module_name, unsigned long offset, const void* src, unsigned long size, bool enabled = true)
{
	char* module_address = module_memory(module_name) + offset;
	if (module_address == module_memory(module_name) || module_address - offset == 0)
		return nullptr;

	s_module_patch* module_patch = new s_module_patch();
	module_patch->address = module_address;
	module_patch->data_size = size;

	module_patch->old_data = new char[module_patch->data_size]{};
	::memcpy(module_patch->old_data, module_patch->address, module_patch->data_size);

	module_patch->new_data = new char[module_patch->data_size]{};
	::memcpy(module_patch->new_data, src, size);

	module_patch->enabled = enabled;
	if (module_patch->enabled)
		module_patch->enable();

	return module_patch;
}

s_module_patch* patch_memaddr(const wchar_t* module_name, unsigned long offset, const void* src, unsigned long size)
{
	return patch_memcpy(module_name, offset, &src, size);
}

unsigned long module_address_to_offset(char* address)
{
	char* memory = module_memory(NULL);
	return address - memory;
}

unsigned long module_address_to_offset(unsigned long address)
{
	char* memory = module_memory(NULL);
	return reinterpret_cast<char*>(address) - memory;
}

unsigned long call_to_function_offset(char* call_addr)
{
	unsigned long function_offset = 0;

	if (*call_addr == '\xE8')
	{
		function_offset = *reinterpret_cast<unsigned long*>(call_addr + 1);
		function_offset += module_address_to_offset(call_addr + 5);
	}
	//else if (*call_addr == '\xEB')
	//{
	//	function_offset = module_reference<unsigned long>(NULL, call_offset + 1);
	//	function_offset += (call_offset + 5);
	//}

	return function_offset;
}
unsigned long call_to_function_offset(unsigned long call_offset)
{
	unsigned long function_offset = 0;

	char* call_addr = module_pointer<char>(NULL, call_offset);
	if (*call_addr == '\xE8')
	{
		function_offset = module_reference<unsigned long>(NULL, call_offset + 1);
		function_offset += (call_offset + 5);
	}
	//else if (*call_addr == '\xEB')
	//{
	//	function_offset = module_reference<unsigned long>(NULL, call_offset + 1);
	//	function_offset += (call_offset + 5);
	//}

	return function_offset;
}

unsigned long push_to_data_offset(unsigned long push_offset)
{
	unsigned long data_offset = 0;

	char* push_addr = module_pointer<char>(NULL, push_offset);
	if (*push_addr == '\x68')
	{
		data_offset = *reinterpret_cast<unsigned long*>(push_addr + 1);
		data_offset -= reinterpret_cast<unsigned long>(module_memory(NULL));
	}

	return data_offset;
}

unsigned long jump_to_function_offset(char* jump_addr)
{
	unsigned long function_offset = 0;

	if (jump_addr[0] == '\x0F' && jump_addr[1] == '\x84')
	{
		function_offset = *reinterpret_cast<unsigned long*>(jump_addr + 2);
		function_offset += module_address_to_offset(jump_addr + 6);
	}
	//else if (*call_addr == '\xEB')
	//{
	//	function_offset = module_reference<unsigned long>(NULL, call_offset + 1);
	//	function_offset += (call_offset + 5);
	//}

	return function_offset;
}

template<typename t_type>
c_vector<unsigned long> find_all_references(const t_type to_find, c_vector<unsigned long>* out_references = nullptr)
{
	c_vector<unsigned long> references;

	unsigned long memory_size = 0;
	char* memory = module_memory(NULL, &memory_size);

	char* pattern = new char[sizeof(to_find)]{};
	::memcpy(pattern, &to_find, sizeof(to_find));
	char* mask = new char[sizeof(to_find) + 1]{};
	::memset(mask, 'x', sizeof(to_find));

	char* start = memory;
	char* end = memory + memory_size;
	for (char* result = nullptr; start < end; start = result)
	{
		result = find_pattern(start, end - start, pattern, mask);
		if (!result)
			break;

		references.push(result - memory);
		result += sizeof(unsigned long);
	}

	if (out_references)
		*out_references = references;
	return references;
}

c_vector<unsigned long> find_all_references_with_length(const char* to_find, unsigned long count, c_vector<unsigned long>* out_references = nullptr)
{
	c_vector<unsigned long> references;

	unsigned long memory_size = 0;
	char* memory = module_memory(NULL, &memory_size);

	unsigned long char_size = sizeof(*to_find);
	unsigned long pattern_len = ((count - 1) * char_size);

	char* pattern = new char[pattern_len + char_size]{};
	::memcpy(pattern, to_find, pattern_len);
	char* mask = new char[pattern_len + char_size]{};
	::memset(mask, 'x', pattern_len);

	char* start = memory;
	char* end = memory + memory_size;
	for (char* result = nullptr; start < end; start = result)
	{
		result = find_pattern(start, end - start, pattern, mask);
		if (!result)
			break;

		references.push(result - memory);
		result += (pattern_len + char_size);
	}

	if (out_references)
		*out_references = references;
	return references;
}

c_vector<unsigned long> get_all_strings_startswith(const char* to_find, c_vector<unsigned long>* out_references = nullptr)
{
	c_vector<unsigned long> references;

	unsigned long memory_size = 0;
	char* memory = module_memory(NULL, &memory_size);

	unsigned long char_size = sizeof(*to_find);
	unsigned long pattern_len = (strlen(to_find) * char_size);

	char* pattern = new char[pattern_len + char_size]{};
	::memcpy(pattern, to_find, pattern_len);
	char* mask = new char[pattern_len + char_size]{};
	::memset(mask, 'x', pattern_len);

	char* start = memory;
	char* end = memory + memory_size;
	for (char* result = nullptr; start < end; start = result)
	{
		result = find_pattern(start, end - start, pattern, mask);
		if (!result)
			break;

		references.push(result - memory);
		result += (pattern_len + char_size);
	}

	if (out_references)
		*out_references = references;
	return references;
}

c_vector<unsigned long> get_all_wstrings_startswith(const wchar_t* to_find, c_vector<unsigned long>* out_references = nullptr)
{
	c_vector<unsigned long> references;

	unsigned long memory_size = 0;
	char* memory = module_memory(NULL, &memory_size);

	unsigned long char_size = sizeof(*to_find);
	unsigned long pattern_len = (wcslen(to_find) * char_size);

	char* pattern = new char[pattern_len + char_size]{};
	::memcpy(pattern, to_find, pattern_len);
	char* mask = new char[pattern_len + char_size]{};
	::memset(mask, 'x', pattern_len);

	char* start = memory;
	char* end = memory + memory_size;
	for (char* result = nullptr; start < end; start = result)
	{
		result = find_pattern(start, end - start, pattern, mask);
		if (!result)
			break;

		references.push(result - memory);
		result += (pattern_len + char_size);
	}

	if (out_references)
		*out_references = references;
	return references;
}

namespace rtti
{
	c_vector<unsigned long> references = get_all_strings_startswith(".?AV");

	void find(const char* to_find)
	{
		if (references.is_valid())
		{
			static char buff[4096]{};

			for (unsigned long reference_index = 0; reference_index < references.size(); reference_index++)
			{
				char* address = module_pointer<char>(NULL, references[reference_index]);

				if (strstr(address, to_find) != 0)
				{
					printf("0x%08lX, %s\n", BASE_ADDRESS_32BIT + references[reference_index], address);
				}

				printf("");
			}
		}
	}

	void find_demangled(const char* to_find)
	{
		if (references.is_valid())
		{
			static char buff[4096]{};

			for (unsigned long reference_index = 0; reference_index < references.size(); reference_index++)
			{
				char* address = module_pointer<char>(NULL, references[reference_index]);

				unsigned long len = static_cast<unsigned long>(strlen(address));
				char* str = new char[len + 4]{};

				::memcpy(str, address, len);
				::memcpy(str, "??_7", 4);
				::memcpy(str + strlen(address), "6B@", 3);

				if ((UnDecorateSymbolName(str, buff, sizeof(buff), UNDNAME_COMPLETE)) != 0)
				{
					if (strstr(buff, to_find) != 0)
					{
						printf("0x%08lX, %s\n", BASE_ADDRESS_32BIT + references[reference_index], buff);
					}
				}
				else
				{
					printf("error %x\n", GetLastError());
				}
				delete[] str;

				printf("");
			}
		}
	}
}
