#pragma once

template<typename t_enum, typename t_type>
using c_enum = t_type;

#define NONE -1

#define valid_index(name, index) index > NONE && index < k_##name##_count

#define define_enum_strings(name) const char* k_##name##_strings[k_##name##_count]

#define get_enum_string(name, index) k_##name##_strings[_##name##_##index]
#define get_enum_default(name) k_default_##name

enum e_language
{
	_language_none = NONE,

	_language_english,
	_language_japanese,
	_language_german,
	_language_french,
	_language_spanish,
	_language_mexican,
	_language_italian,
	_language_korean,
	_language_chinese_traditional,
	_language_chinese_simplified,
	_language_portuguese,
	_language_russian,

	k_language_count,
	k_default_language = _language_english
};
define_enum_strings(language)
{
	"english",
	"japanese",
	"german",
	"french",
	"spanish",
	"mexican",
	"italian",
	"korean",
	"chinese-traditional",
	"chinese-simplified",
	"portuguese",
	"russian"
};
const char* g_language = get_enum_string(language, english);

enum e_game_mode
{
	_game_mode_none = NONE,

	_game_mode_null,

	_game_mode_singleplayer,
	_game_mode_multiplayer,
	_game_mode_ui,
	_game_mode_saved_film,
	_game_mode_survival,

	k_game_mode_count
};
define_enum_strings(game_mode)
{
	"none",
	"singleplayer",
	"multiplayer",
	"ui",
	"saved film",
	"survival"
};

enum e_game_engine_variant
{
	_game_engine_none = NONE,
	
	_game_engine_base_variant,
	_game_engine_ctf_variant,
	_game_engine_slayer_variant,
	_game_engine_oddball_variant,
	_game_engine_king_variant,
	_game_engine_sandbox_variant,
	_game_engine_vip_variant,
	_game_engine_juggernaut_variant,
	_game_engine_territories_variant,
	_game_engine_assault_variant,
	_game_engine_infection_variant,

	k_game_engine_variant_count,
	k_default_game_engine_multiplayer_variant = _game_engine_slayer_variant
};
define_enum_strings(game_engine_variant)
{
	"base",
	"ctf",
	"slayer",
	"oddball",
	"king",
	"sandbox",
	"vip",
	"juggernaut",
	"territories",
	"assault",
	"infection"
};

enum e_game_engine_variant_general_setting
{
	_game_engine_variant_general_setting_none = NONE,

	_game_engine_variant_general_setting_teams_enabled,

	k_game_engine_variant_general_setting_count
};
define_enum_strings(game_engine_variant_general_setting)
{
	"teams_enabled"
};

enum e_cache_file_type
{
	_cache_file_type_none = NONE,

	_cache_file_type_singleplayer,
	_cache_file_type_multiplayer,
	_cache_file_type_ui,
	_cache_file_type_shared,
	//...

	k_cache_file_type_count = 7
};

enum e_cache_file_shared_type
{
	_cache_file_shared_type_none = NONE,

	_cache_file_shared_type_ui,
	_cache_file_shared_type_shared,
	_cache_file_shared_type_singleplayer,
	//...

	k_cache_file_shared_type_count = 6
};

enum e_shared_file_type
{
	_shared_file_type_none = NONE,

	_shared_file_type_strings,

	_shared_file_type_ui,
	_shared_file_type_resources,
	_shared_file_type_textures,
	_shared_file_type_textures_b,
	_shared_file_type_audio,
	_shared_file_type_video,

	_shared_file_type_lightmaps,
	_shared_file_type_render_models,

	k_shared_file_type_count
};
define_enum_strings(shared_file_type)
{
	"strings",

	"ui",
	"resources",
	"textures",
	"textures_b",
	"audio",
	"video",

	"lightmaps",
	"render_models"
};

c_enum<e_game_mode, unsigned long> cache_file_type_to_game_mode(c_enum<e_cache_file_type, unsigned long> cache_file_type);
c_enum<e_game_engine_variant, unsigned long> cache_file_type_to_game_engine_variant(c_enum<e_cache_file_type, unsigned long> cache_file_type);

enum e_director_mode
{
	_director_mode_none = NONE,

	_director_mode_game,
	_director_mode_saved_film,
	_director_mode_observer,
	_director_mode_debug,
	_director_mode_unknown,
	_director_mode_editor,

	k_director_mode_count
};
define_enum_strings(director_mode)
{
	"game",
	"saved film",
	"observer",
	"debug",
	"<unknown>",
	"editor"
};

enum e_camera_mode
{
	_camera_mode_none = NONE,

	_camera_mode_following,
	_camera_mode_orbiting,
	_camera_mode_flying,
	_camera_mode_first_person,
	_camera_mode_dead,
	_camera_mode_static,
	_camera_mode_scripted,
	_camera_mode_authored,

	k_camera_mode_count
};
define_enum_strings(camera_mode)
{
	"following",
	"orbiting",
	"flying",
	"first person",
	"dead",
	"static",
	"scripted",
	"authored"
};