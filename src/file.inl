#pragma once

template<typename t_type>
bool read_structure_from_file(t_type** structure, const char* filename)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filename, "rb"), file != nullptr)
	{
		*structure = new t_type();
		fread(*structure, 1, sizeof(t_type), file);
		fclose(file);

		return true;
	}

	return false;
}

template<typename t_type>
bool write_structure_to_file(t_type& structure, const char* filename)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filename, "wb"), file != nullptr)
	{
		fwrite(&structure, 1, sizeof(t_type), file);
		fclose(file);

		return true;
	}

	return false;
}

bool read_data_from_file(char** out_data, long* out_size, const char* filename)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filename, "rb"), file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		*out_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		*out_data = new char[*out_size];
		fread(*out_data, 1, *out_size, file);
		fclose(file);

		return true;
	}

	return false;
}

bool write_data_to_file(char* data, unsigned long size, const char* filename)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filename, "wb"), file != nullptr)
	{
		fwrite(data, 1, size, file);
		fclose(file);

		return true;
	}

	return false;
}

void find_files(const char* find_str, c_vector<char*>* files, bool absolute_path = true)
{
	c_path find_path, dll_dir;
	GetModuleFileNameA(GetModuleHandleA(DLL_NAME), dll_dir, _countof(dll_dir));
	PathRemoveFileSpecA(dll_dir);
	sprintf_s(find_path, "%s\\%s", dll_dir, find_str);

	WIN32_FIND_DATAA find_file_data;
	HANDLE find_handle = FindFirstFileA(find_path, &find_file_data);
	PathRemoveFileSpecA(find_path);

	if (find_handle != INVALID_HANDLE_VALUE)
	{
		c_path filepath;
		do
		{
			if (!strcmp(find_file_data.cFileName, "..") || !strcmp(find_file_data.cFileName, ".") ||
				find_file_data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM || find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			sprintf_s(filepath, MAX_PATH - 1, "%s\\%s", find_path, find_file_data.cFileName);
			//printf_s("%s\n", filepath);

			char* tmp = new char[MAX_PATH];
			csstrnzcpy(tmp, absolute_path ? filepath : find_file_data.cFileName, MAX_PATH);
			(*files).push(tmp);

		} while (FindNextFileA(find_handle, &find_file_data) != 0);
	}

	FindClose(find_handle);
}

void find_files(const wchar_t* find_str, c_vector<wchar_t*>* files, bool absolute_path = true)
{
	c_path_wide find_path, dll_dir;
	GetModuleFileNameW(GetModuleHandleA(DLL_NAME), dll_dir, _countof(dll_dir));
	PathRemoveFileSpecW(dll_dir);
	swprintf_s(find_path, L"%s\\%s", dll_dir, find_str);

	WIN32_FIND_DATAW find_file_data;
	HANDLE find_handle = FindFirstFileW(find_path, &find_file_data);
	if (find_handle != INVALID_HANDLE_VALUE)
	{
		c_path_wide filepath;
		do
		{
			if (!wcscmp(find_file_data.cFileName, L"..") || !wcscmp(find_file_data.cFileName, L".") ||
				find_file_data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM || find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			swprintf_s(filepath, MAX_PATH - 1, L"%s\\%s", dll_dir, find_file_data.cFileName);
			//wprintf_s(L"%s\n", filepath);

			wchar_t* tmp = new wchar_t[MAX_PATH];
			cswcsnzcpy(tmp, absolute_path ? filepath : find_file_data.cFileName, MAX_PATH);
			(*files).push(tmp);

		} while (FindNextFileW(find_handle, &find_file_data) != 0);
	}

	FindClose(find_handle);
}
