#include "pch.h"

c_enum<e_game_mode, unsigned long> cache_file_type_to_game_mode(c_enum<e_cache_file_type, unsigned long> value)
{
	//assert(valid_index(cache_file_type, value));

	switch (value)
	{
	case _cache_file_type_singleplayer: return _game_mode_singleplayer;
	case _cache_file_type_multiplayer: return _game_mode_multiplayer;
	case _cache_file_type_ui: return _game_mode_ui;
	}

	return _game_mode_none;
}

c_enum<e_game_engine_variant, unsigned long> cache_file_type_to_game_engine_variant(c_enum<e_cache_file_type, unsigned long> value)
{
	//assert(valid_index(cache_file_type, value));

	switch (value)
	{
	case _cache_file_type_multiplayer: return k_default_game_engine_multiplayer_variant;
	}

	return _game_engine_base_variant;
}
