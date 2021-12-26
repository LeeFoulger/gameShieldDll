#include "pch.h"

c_config::c_config(const char* filename, bool fullpath)
{
	c_path path;
	GetModuleFileNameA(GetModuleHandleA(DLL_NAME), path, sizeof(path));
	PathRemoveFileSpecA(path);
	sprintf_s(m_filename, "%s\\%s", path, filename);
}

template<typename t_type>
void c_config::read_as_format(t_type* out_value, const char* format, const char* section, const char* key)
{
	if (!out_value)
		return;

	c_string<char, 4096> default_str;
	c_string<char, 4096> str;

	sprintf_s(default_str, format, *out_value);
	GetPrivateProfileStringA(section, key, default_str, str, sizeof(str), m_filename);

	sscanf_s(str, format, out_value);
}

template<typename t_type>
void c_config::write_as_format(t_type value, const char* format, const char* section, const char* key)
{
	c_string<char, 4096> str;

	sprintf_s(str, format, value);
	WritePrivateProfileStringA(section, key, str, m_filename);
}

void c_config::read_bool(bool* out_value, const char* section, const char* key)
{
	bool value;
	read_as_format(&value, "%d", section, key);

	*out_value = !!value;
}

void c_config::write_bool(bool value, const char* section, const char* key)
{
	write_as_format(value, "%d", section, key);
}

void c_config::read_long(long* out_value, const char* section, const char* key, bool as_hex)
{
	read_as_format(out_value, as_hex ? "0x%lX" : "%ld", section, key);
}

void c_config::write_long(long value, const char* section, const char* key, bool as_hex)
{
	write_as_format(value, as_hex ? "0x%lX" : "%ld", section, key);
}

void c_config::read_ulong(unsigned long* out_value, const char* section, const char* key, bool as_hex)
{
	read_as_format(out_value, as_hex ? "0x%lX" : "%lu", section, key);
}

void c_config::write_ulong(unsigned long value, const char* section, const char* key, bool as_hex)
{
	write_as_format(value, as_hex ? "0x%lX" : "%lu", section, key);
}

void c_config::read_long64(long long* out_value, const char* section, const char* key, bool as_hex)
{
	read_as_format(out_value, as_hex ? "0x%llX" : "%lld", section, key);
}

void c_config::write_long64(long long value, const char* section, const char* key, bool as_hex)
{
	write_as_format(value, as_hex ? "0x%llX" : "%lld", section, key);
}

void c_config::read_ulong64(unsigned long long* out_value, const char* section, const char* key, bool as_hex)
{
	read_as_format(out_value, as_hex ? "0x%llX" : "%llu", section, key);
}

void c_config::write_ulong64(unsigned long long value, const char* section, const char* key, bool as_hex)
{
	write_as_format(value, as_hex ? "0x%llX" : "%llu", section, key);
}