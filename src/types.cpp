#include "pch.h"

s_game_engine_variant_general_settings& s_game_variant::get_general_settings()
{
	return *reinterpret_cast<s_game_engine_variant_general_settings*>(__data + 0x120);
}

s_game_engine_variant_respawn_settings& s_game_variant::get_respawn_settings()
{
	return *reinterpret_cast<s_game_engine_variant_respawn_settings*>(__data + 0x124);
}

s_game_engine_variant_social_settings& s_game_variant::get_social_settings()
{
	return *reinterpret_cast<s_game_engine_variant_social_settings*>(__data + 0x14C);
}


unsigned long& s_map_variant::get_map_id()
{
	return *reinterpret_cast<unsigned long*>(__data + 0x100);
}


c_path& s_game_options::get_scenario_path()
{
	return *reinterpret_cast<c_path*>(__data + 0x20);
}

s_game_variant& s_game_options::get_game_variant()
{
	return *reinterpret_cast<s_game_variant*>(__data + 0x328);
}

s_map_variant& s_game_options::get_map_variant()
{
	return *reinterpret_cast<s_map_variant*>(__data + 0x58C);
}


unsigned long& s_cache_file_header::get_header_signature()
{
	return *reinterpret_cast<unsigned long*>(__data + 0x0);
}

c_string<char, 256>& s_cache_file_header::get_source_file()
{
	return *reinterpret_cast<c_string<char, 256>*>(__data + 0x1C);
}

c_string<char, 32>& s_cache_file_header::get_build()
{
	return *reinterpret_cast<c_string<char, 32>*>(__data + 0x11C);
}

c_enum<e_cache_file_type, unsigned short>& s_cache_file_header::get_cache_file_type()
{
	return *reinterpret_cast<c_enum<e_cache_file_type, unsigned short>*>(__data + 0x13C);
}

c_enum<e_cache_file_shared_type, unsigned short>& s_cache_file_header::get_cache_file_shared_type()
{
	return *reinterpret_cast<c_enum<e_cache_file_shared_type, unsigned short>*>(__data + 0x13E);
}

c_flags<e_shared_file_type, unsigned long>& s_cache_file_header::get_shared_file_type_flags()
{
	return *reinterpret_cast<c_flags<e_shared_file_type, unsigned long>*>(__data + 0x168);
}

long shared_file_count(char* __data)
{
	long count = 2;

	unsigned long flags = *reinterpret_cast<unsigned long*>(__data + 0x168);
	for (unsigned long bit_index = 0; bit_index < (sizeof(flags) * 8); bit_index++)
	{
		unsigned long flag = flags & (1 << bit_index);
		count += flag != 0;
	}

	return count;
}

c_string<char, 32>& s_cache_file_header::get_scenario_name()
{
	unsigned long offset = shared_file_count(__data) * 8;
	return *reinterpret_cast<c_string<char, 32>*>(__data + offset + 0x16C);
}

c_string<char, 128>& s_cache_file_header::get_scenario_path()
{
	unsigned long offset = shared_file_count(__data) * 8;
	return *reinterpret_cast<c_string<char, 128>*>(__data + 0x16C + offset + 0x24);
}

unsigned long& s_cache_file_header::get_map_id()
{
	unsigned long offset = shared_file_count(__data) * 8;
	return *reinterpret_cast<unsigned long*>(__data + 0x16C + offset + 0x2C48);
}

unsigned long& s_cache_file_header::get_scenario_tag_index()
{
	unsigned long offset = shared_file_count(__data) * 8;
	return *reinterpret_cast<unsigned long*>(__data + 0x16C + offset + 0x2C4C);
}

unsigned long& s_cache_file_header::get_footer_signature()
{
	return *reinterpret_cast<unsigned long*>(__data + (sizeof(s_cache_file_header) - 4));
}
