#pragma once

template<typename t_type>
struct c_vector
{
public:
	c_vector() :
		elements(nullptr),
		count(0)
	{
	}

	c_vector(t_type* elements, unsigned long count) :
		elements(elements),
		count(count)
	{
	}

	void push(t_type value)
	{
		t_type* tmp = new t_type[count + 1]{};

		if (elements)
		{
			::memcpy(tmp, elements, sizeof(t_type) * count);
			delete[] elements;
			elements = nullptr;
		}

		tmp[count] = value;

		elements = tmp;
		count++;
	}

	t_type*& get() { return elements; }
	unsigned long& size() { return count; }
	bool is_valid() { return elements && count; }

	t_type& operator[](unsigned long index) { return elements[index]; }

private:
	t_type* elements;
	unsigned long count;
};

template<typename t_enum, typename t_type>
using c_flags = t_type;

struct s_vfile_info
{
	c_string<char, 256> name;
	c_string<char, 256> folder;
	unsigned long offset;
	unsigned long size;
};

struct s_game_engine_variant_general_settings
{
	c_flags<e_game_engine_variant_general_setting, unsigned char> flags;
	unsigned char time_limit;
	unsigned char number_of_rounds;
	unsigned char early_victory_win_count;
};
struct s_game_engine_variant_respawn_settings
{
	unsigned char flags;
	unsigned char lives_per_round;
	unsigned char shared_team_lives;
	unsigned char respawn_time;
	unsigned char suicide_penalty;
	unsigned char betrayal_penalty;
	unsigned char unknown_penalty;
	unsigned char respawn_time_growth;
	unsigned char respawn_trait_duration;
};
struct s_game_engine_variant_social_settings
{
	unsigned short flags;
	unsigned short team_changing;
};

struct s_game_variant
{
	c_enum<e_game_engine_variant, unsigned long> game_engine_variant;
	char __data[0x260];

	s_game_engine_variant_general_settings& get_general_settings();
	s_game_engine_variant_respawn_settings& get_respawn_settings();
	s_game_engine_variant_social_settings& get_social_settings();
};

struct s_map_variant
{
	char __data[0xE090];

	unsigned long& get_map_id();
};

enum e_game_options
{
	_game_options_v0 = 0,
	_game_options_v1,
	_game_options_v2,
	_game_options_v3,

	k_game_options_count
};

constexpr unsigned long get_game_options_size(c_enum<e_game_options, unsigned long> version)
{
	switch (version)
	{
	// 106708 to 177150
	case _game_options_v0: return 0x24B48;

	// 235640 to 332089
	case _game_options_v1: return 0x245C8;

	// 498295 to 604673
	case _game_options_v2: return 0x1A048;

	// 700255, no more builds available
	case _game_options_v3: return 0x1A248;
	}

	return 0;
}

struct s_game_options
{
	c_enum<e_game_mode, unsigned long> game_mode;
	char __data[get_game_options_size(_game_options_v2) - 4];

	c_path& get_scenario_path();
	s_game_variant& get_game_variant();
	s_map_variant& get_map_variant();
};

struct s_cache_file_header
{
	char __data[0x3390];

	unsigned long& get_header_signature();
	c_string<char, 256>& get_source_file();
	c_string<char, 32>& get_build();
	c_enum<e_cache_file_type, unsigned short>& get_cache_file_type();
	c_enum<e_cache_file_shared_type, unsigned short>& get_cache_file_shared_type();
	c_flags<e_shared_file_type, unsigned long>& get_shared_file_type_flags();
	c_string<char, 32>& get_scenario_name();
	c_string<char, 128>& get_scenario_path();
	unsigned long& get_map_id();
	unsigned long& get_scenario_tag_index();
	unsigned long& get_footer_signature();
};