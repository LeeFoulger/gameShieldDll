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
