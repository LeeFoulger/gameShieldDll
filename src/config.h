#pragma once

class c_config
{
public:
	c_config(const char* filename, bool fullpath = false);

	void read_bool(bool* out_value, const char* section, const char* key);
	void write_bool(bool value, const char* section, const char* key);

	void read_long(long* out_value, const char* section, const char* key, bool as_hex = false);
	void write_long(long value, const char* section, const char* key, bool as_hex = false);

	void read_ulong(unsigned long* out_value, const char* section, const char* key, bool as_hex = false);
	void write_ulong(unsigned long value, const char* section, const char* key, bool as_hex = false);

	void read_long64(long long* out_value, const char* section, const char* key, bool as_hex = false);
	void write_long64(long long value, const char* section, const char* key, bool as_hex = false);

	void read_ulong64(unsigned long long* out_value, const char* section, const char* key, bool as_hex = false);
	void write_ulong64(unsigned long long value, const char* section, const char* key, bool as_hex = false);

private:
	c_path m_filename;

	template<typename t_type>
	void read_as_format(t_type* out_value, const char* format, const char* section, const char* key);

	template<typename t_type>
	void write_as_format(t_type value, const char* format, const char* section, const char* key);

};
