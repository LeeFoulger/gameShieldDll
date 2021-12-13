
#define SSL_BINDER_HOOK(module_name, call_func, print_call, func_name, ...) \
long __cdecl func_name(long, long, long, long, volatile long**); \
s_module_patch* func_name##_hook = patch_memaddr(module_name, get_ssl_binder_function_module_offset(###func_name), func_name, sizeof(void*)); \
long __cdecl func_name(long a1, long a2, long a3, long a4, volatile long** a5) \
{ \
	if (print_call) \
		printf("%s called\n", __FUNCTION__); \
	long(__cdecl* func)(long, long, long, long, volatile long**) = *reinterpret_cast<decltype(func)*>(func_name##_hook->old_data); \
	long result = 1; \
	if (call_func) \
		result = func(a1, a2, a3, a4, a5); \
	sslBinder_handler(__FUNCTION__, a1, a2, a3, a4, a5); \
	__VA_ARGS__ \
	return result; \
}

#define SSL_BINDER_UNHOOK(func_name) if (func_name##_hook) func_name##_hook->revert()

struct s_command
{
	c_string<wchar_t, 32> name;
	void(*callback)(const wchar_t*, const wchar_t*);
};

s_command commands[]
{
	{
		L"exit",
		[](const wchar_t* command, const wchar_t* message)
		{
			exit(EXIT_SUCCESS);
		}
	},
	{
		L"reload",
		[](const wchar_t* command, const wchar_t* message)
		{
			game_reload_map();
		}
	},
	{
		L"spload",
		[](const wchar_t* command, const wchar_t* message)
		{
			char scenario_path[256]{};
			snprintf(scenario_path, 256, "%ls", &message[wcslen(command) + 1]);
			game_load_map(scenario_path, _game_mode_singleplayer);
		}
	},
	{
		L"mpload",
		[](const wchar_t* command, const wchar_t* message)
		{
			char scenario_path[256]{};
			snprintf(scenario_path, 256, "%ls", &message[wcslen(command) + 1]);
			game_load_map(scenario_path, _game_mode_multiplayer);
		}
	},
	{
		L"uiload",
		[](const wchar_t* command, const wchar_t* message)
		{
			char scenario_path[256]{};
			snprintf(scenario_path, 256, "%ls", &message[wcslen(command) + 1]);
			game_load_map(scenario_path, _game_mode_ui);
		}
	},
	{
		L"toggle",
		[](const wchar_t* command, const wchar_t* message)
		{
			if (wcscmp(&message[wcslen(command) + 1], L"watermark") == 0)
			{
				patches::watermark::toggle_all();
			}
			else if (wcscmp(&message[wcslen(command) + 1], L"director") == 0)
			{
				patches::director::toggle_all();
			}
		}
	},
	{
		L"dump_virtual_files",
		[](const wchar_t* command, const wchar_t* message)
		{
			dump_virtual_files();
		}
	},
	{
		L"rtti_search",
		[](const wchar_t* command, const wchar_t* message)
		{
			char* rtti_name = new char[4096]{};
			snprintf(rtti_name, 4096, "%ls", &message[wcslen(command) + 1]);
			rtti::find_demangled(rtti_name);
			delete[] rtti_name;
		}
	}
};

struct s_wstring
{
	unsigned long __unknown0;
	unsigned long __str_len4;
	unsigned long __str_len8;

#pragma warning(push)
#pragma warning(disable: 4200)
	// warning C4200: nonstandard extension used: zero-sized array in struct/union
	// message : This member will be ignored by a defaulted constructor or copy/move assignment operator
	wchar_t str[];
#pragma warning(pop)
};

void SSL_HQ_Chat_handler(const wchar_t* message = L"")
{
	if (message)
	{
		wprintf(L"%s\n", message);

		for (s_command& command : commands)
		{
			if (wcscmp(message, command.name) == 0 || wcsstr(message, command.name) == message)
				command.callback(command.name, message);
		}
	}
}

void sslBinder_handler(const char* function_name, long a1, long a2, long a3, long a4, volatile long** a5)
{
	if (strstr(function_name, "SSL_HQ::Chat") != 0)
	{
		s_wstring* wstr = *reinterpret_cast<s_wstring**>(a3);
		SSL_HQ_Chat_handler(wstr->str);
	}
	//__debugbreak();
}

// this function is extremely fucking slow
unsigned long get_ssl_binder_function_module_offset(const char* ssl_function_name)
{
	c_vector<unsigned long> references0 = get_all_strings_startswith(ssl_function_name);
	if (!references0.size())
		return 0;

	printf("%s\n", ssl_function_name);

	unsigned long reference0 = references0[0];
	char* data0 = module_pointer<char>(NULL, reference0);

	c_vector<unsigned long> references1 = find_all_references(data0);
	if (!references1.size())
		return 0;

	unsigned long reference1 = references1[0];
	char* data1 = module_pointer<char>(NULL, reference1);

	c_vector<unsigned long> moves;
	{
		char* start = nullptr;
		char* end = nullptr;
		{
			char* function_start = data1;
			char* function_end = data1;
			while (function_start[0] != '\xCC')
			{
				if (function_start[1] != '\x55')
					function_start--;

				function_start--;
			}
			function_start++;

			while (function_end[0] != '\xC3' && function_end[1] != '\xCC')
			{
				function_end++;
			}
			function_end++;

			start = function_start;
			end = function_end;
		}

		for (char* result = nullptr; start < end; start += sizeof("x????x"))
		{
			result = find_pattern(start, end - start, "\xBA\x00\x00\x00\x00\x8D", "x????x");
			if (!result)
				break;

			moves.push(module_address_to_offset(result));
		}
	}

	if (!moves.size())
		return 0;

	for (unsigned long move_index = 0; move_index < moves.size(); move_index++)
	{
		unsigned long move_offset = moves[move_index];
		char* move_addr = module_pointer<char>(NULL, move_offset);

		unsigned long function_addr = *reinterpret_cast<unsigned long*>(move_addr + 1);
		char* function_start = reinterpret_cast<char*>(function_addr);

		char* function_end = function_start;
		{
			while (function_end[0] != '\xC3' && function_end[1] != '\xCC')
			{
				function_end++;
			}
			function_end++;
		}

		c_vector<unsigned long> calls;
		char* start = function_start;
		char* end = function_end;
		for (char* result = nullptr; start < end; start = result)
		{
			result = find_pattern(start, end - start, "\xFF\x15\x00\x00\x00\x00", "xx????");
			if (!result)
				break;

			calls.push(module_address_to_offset(result));
			result += 5;
		}

		if (!calls.size())
			return 0;

		unsigned long result = module_address_to_offset(module_reference<unsigned long>(NULL, calls[0] + 2));
		return result;
	}

	return 0;
}

//namespace SSL_HQ
//{
//	SSL_BINDER_HOOK(NULL, 0, 1, ShowroomBegin); // prevent crash on button click
//
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendPrivate);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendGeneral);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendParty);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendGameAll);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendGameTeam);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatSendCommand);
//	SSL_BINDER_HOOK(NULL, 0, 1, ChatExtractCommand);
//
//	SSL_BINDER_HOOK(NULL, 0, 1, TextureRenderBipedProfile); // prevent crash when switching genders
//	SSL_BINDER_HOOK(NULL, 0, 1, TextureRenderBipedUserCard2); // prevent crash when switching genders
//	SSL_BINDER_HOOK(NULL, 0, 1, TextureRenderBipedPostmatch); // prevent crash when reloading the mainmenu
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(ShowroomBegin);
//
//		SSL_BINDER_UNHOOK(ChatSendPrivate);
//		SSL_BINDER_UNHOOK(ChatSendGeneral);
//		SSL_BINDER_UNHOOK(ChatSendParty);
//		SSL_BINDER_UNHOOK(ChatSendGameAll);
//		SSL_BINDER_UNHOOK(ChatSendGameTeam);
//		SSL_BINDER_UNHOOK(ChatSendCommand);
//		SSL_BINDER_UNHOOK(ChatExtractCommand);
//
//		SSL_BINDER_UNHOOK(TextureRenderBipedProfile);
//		SSL_BINDER_UNHOOK(TextureRenderBipedUserCard2);
//		SSL_BINDER_UNHOOK(TextureRenderBipedPostmatch);
//	}
//}

//namespace GAME_PREFERENCES_SSL
//{
//	SSL_BINDER_HOOK(NULL, /*0x00F94FD4,*/ 1, 0, GetToggleCrouch);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E8C,*/ 1, 0, SetToggleCrouch);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FAC,*/ 1, 0, GetHUDShake);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F08,*/ 1, 0, SetHUDShake);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E5C,*/ 1, 0, GetSwearFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F38,*/ 1, 0, SetSwearFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F50,*/ 1, 0, GetAltCrosshair);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EF0,*/ 1, 0, SetAltCrosshair);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F00,*/ 1, 0, GetGooeyCrosshair);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FD0,*/ 1, 0, SetGooeyCrosshair);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F20,*/ 1, 0, GetDisplayHints);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F74,*/ 1, 0, SetDisplayHints);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E64,*/ 1, 0, GetTeamColor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94ED0,*/ 1, 0, SetTeamColor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F78,*/ 1, 0, GetCameraFOV);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EA0,*/ 1, 0, SetCameraFOV);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F1C,*/ 1, 0, ResetTutorial);
//
//	// multiplayer settings
//	SSL_BINDER_HOOK(NULL, /*0x00F94EE4,*/ 1, 0, GetMatchmakingRegion);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FE8,*/ 1, 0, SetMatchmakingRegion);
//
//	// video settings
//	SSL_BINDER_HOOK(NULL, /*0x00F94EEC,*/ 1, 0, GetScreenResolutionList);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E9C,*/ 1, 0, GetScreenResolution);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F44,*/ 1, 0, GetDefaultVideoSettings);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FA8,*/ 1, 0, GetAdvancedVideoPresetParams);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EFC,*/ 1, 0, CheckVideoSettingsForApply);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F54,*/ 1, 0, GetDefaultControlsSettings);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F34,*/ 1, 0, GetDefaultSoundSettings);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EA8,*/ 1, 0, GetDefaultGameplaySettings);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E58,*/ 1, 0, SetScreenResolution);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FC4,*/ 1, 0, GetQualityPresetOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F0C,*/ 1, 0, GetQualityPreset);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EF4,*/ 1, 0, GetBrightness);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FB0,*/ 1, 0, SetBrightness);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F70,*/ 1, 0, GetBrighntessScaleAndShift);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FA0,*/ 1, 0, GetFullscreen);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FC8,*/ 1, 0, SetFullscreen);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E74,*/ 1, 0, GetVSync);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E60,*/ 1, 0, SetVSync);
//	SSL_BINDER_HOOK(NULL, /*0x00F94ED8,*/ 1, 0, GetAntialiasing);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F04,*/ 1, 0, SetAntialiasing);
//
//	// video settings advanced
//	SSL_BINDER_HOOK(NULL, /*0x00F94F7C,*/ 1, 0, GetTextureResolutionOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FE4,*/ 1, 0, GetTextureResolution);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F94,*/ 1, 0, SetTextureResolution);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FE0,*/ 1, 0, GetTextureFilteringQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E78,*/ 1, 0, GetTextureFilteringQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FC0,*/ 1, 0, SetTextureFilteringQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FCC,*/ 1, 0, GetLightingQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FA4,*/ 1, 0, GetLightingQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F5C,*/ 1, 0, SetLightingQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E68,*/ 1, 0, GetEffectsQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FB4,*/ 1, 0, GetEffectsQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EDC,*/ 1, 0, SetEffectsQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F64,*/ 1, 0, GetShadowQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E80,*/ 1, 0, GetShadowQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FB8,*/ 1, 0, SetShadowQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F58,*/ 1, 0, GetDetailsQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E7C,*/ 1, 0, GetDetailsQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F2C,*/ 1, 0, SetDetailsQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FD8,*/ 1, 0, GetPostprocessQualityOptions);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F40,*/ 1, 0, GetPostprocessQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FEC,*/ 1, 0, SetPostprocessQuality);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EF8,*/ 1, 0, GetMotionBlur);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E88,*/ 1, 0, SetMotionBlur);
//
//	// audio settings
//	SSL_BINDER_HOOK(NULL, /*0x00F94EC0,*/ 1, 0, GetMasterVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94ECC,*/ 1, 0, SetMasterVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F90,*/ 1, 0, GetSFXVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EB0,*/ 1, 0, SetSFXVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F60,*/ 1, 0, GetMusicVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F80,*/ 1, 0, SetMusicVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EC4,*/ 1, 0, GetVoiceVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E6C,*/ 1, 0, SetVoiceVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EC8,*/ 1, 0, GetVoiceChatControl);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F30,*/ 1, 0, SetVoiceChatControl);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E84,*/ 1, 0, GetVoiceChatVolume);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F88,*/ 1, 0, SetVoiceChatVolume);
//
//	// controls settings
//	SSL_BINDER_HOOK(NULL, /*0x00F94F10,*/ 1, 0, GetControlsMethod);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EE0,*/ 1, 0, SetControlsMethod);
//	SSL_BINDER_HOOK(NULL, /*0x00F94ED4,*/ 1, 0, GetMouseSensitivityVert);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F6C,*/ 1, 0, GetMouseSensitivityHor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E94,*/ 1, 0, SetMouseSensitivityVert);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F68,*/ 1, 0, SetMouseSensitivityHor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F14,*/ 1, 0, GetMouseSensitivityVehicleVert);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EAC,*/ 1, 0, GetMouseSensitivityVehicleHor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FDC,*/ 1, 0, SetMouseSensitivityVehicleVert);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EBC,*/ 1, 0, SetMouseSensitivityVehicleHor);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E54,*/ 1, 0, GetMouseAcceleration);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F3C,*/ 1, 0, SetMouseAcceleration);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F4C,*/ 1, 0, GetMouseFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F8C,*/ 1, 0, SetMouseFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EB8,*/ 1, 0, GetInvertMouse);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E90,*/ 1, 0, SetInvertMouse);
//
//	// controller settings
//	SSL_BINDER_HOOK(NULL, /*0x00F94F98,*/ 1, 0, GetLookSensitivity);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F28,*/ 1, 0, SetLookSensitivity);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F24,*/ 1, 0, GetInvertLook);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E98,*/ 1, 0, SetInvertLook);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EB4,*/ 1, 0, GetAutoCentering);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EE8,*/ 1, 0, SetAutoCentering);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F48,*/ 1, 0, GetButtonLayout);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F84,*/ 1, 0, SetButtonLayout);
//
//	// rebind keys
//	SSL_BINDER_HOOK(NULL, /*0x00F94F9C,*/ 1, 0, GetKeyLabels);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E70,*/ 1, 0, GetActionLabels);
//	SSL_BINDER_HOOK(NULL, /*0x00F94EA4,*/ 1, 0, GetDefaultKeyBinds);
//	SSL_BINDER_HOOK(NULL, /*0x00F94FBC,*/ 1, 0, GetKeyBinds);
//	SSL_BINDER_HOOK(NULL, /*0x00F94F18,*/ 1, 0, SetKeyBinds);
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(GetToggleCrouch);
//		SSL_BINDER_UNHOOK(SetToggleCrouch);
//		SSL_BINDER_UNHOOK(GetHUDShake);
//		SSL_BINDER_UNHOOK(SetHUDShake);
//		SSL_BINDER_UNHOOK(GetSwearFilter);
//		SSL_BINDER_UNHOOK(SetSwearFilter);
//		SSL_BINDER_UNHOOK(GetAltCrosshair);
//		SSL_BINDER_UNHOOK(SetAltCrosshair);
//		SSL_BINDER_UNHOOK(GetGooeyCrosshair);
//		SSL_BINDER_UNHOOK(SetGooeyCrosshair);
//		SSL_BINDER_UNHOOK(GetDisplayHints);
//		SSL_BINDER_UNHOOK(SetDisplayHints);
//		SSL_BINDER_UNHOOK(GetTeamColor);
//		SSL_BINDER_UNHOOK(SetTeamColor);
//		SSL_BINDER_UNHOOK(GetCameraFOV);
//		SSL_BINDER_UNHOOK(SetCameraFOV);
//		SSL_BINDER_UNHOOK(ResetTutorial);
//		SSL_BINDER_UNHOOK(GetMatchmakingRegion);
//		SSL_BINDER_UNHOOK(SetMatchmakingRegion);
//		SSL_BINDER_UNHOOK(GetScreenResolutionList);
//		SSL_BINDER_UNHOOK(GetScreenResolution);
//		SSL_BINDER_UNHOOK(GetDefaultVideoSettings);
//		SSL_BINDER_UNHOOK(GetAdvancedVideoPresetParams);
//		SSL_BINDER_UNHOOK(CheckVideoSettingsForApply);
//		SSL_BINDER_UNHOOK(GetDefaultControlsSettings);
//		SSL_BINDER_UNHOOK(GetDefaultSoundSettings);
//		SSL_BINDER_UNHOOK(GetDefaultGameplaySettings);
//		SSL_BINDER_UNHOOK(SetScreenResolution);
//		SSL_BINDER_UNHOOK(GetQualityPresetOptions);
//		SSL_BINDER_UNHOOK(GetQualityPreset);
//		SSL_BINDER_UNHOOK(GetBrightness);
//		SSL_BINDER_UNHOOK(SetBrightness);
//		SSL_BINDER_UNHOOK(GetBrighntessScaleAndShift);
//		SSL_BINDER_UNHOOK(GetFullscreen);
//		SSL_BINDER_UNHOOK(SetFullscreen);
//		SSL_BINDER_UNHOOK(GetVSync);
//		SSL_BINDER_UNHOOK(SetVSync);
//		SSL_BINDER_UNHOOK(GetAntialiasing);
//		SSL_BINDER_UNHOOK(SetAntialiasing);
//		SSL_BINDER_UNHOOK(GetTextureResolutionOptions);
//		SSL_BINDER_UNHOOK(GetTextureResolution);
//		SSL_BINDER_UNHOOK(SetTextureResolution);
//		SSL_BINDER_UNHOOK(GetTextureFilteringQualityOptions);
//		SSL_BINDER_UNHOOK(GetTextureFilteringQuality);
//		SSL_BINDER_UNHOOK(SetTextureFilteringQuality);
//		SSL_BINDER_UNHOOK(GetLightingQualityOptions);
//		SSL_BINDER_UNHOOK(GetLightingQuality);
//		SSL_BINDER_UNHOOK(SetLightingQuality);
//		SSL_BINDER_UNHOOK(GetEffectsQualityOptions);
//		SSL_BINDER_UNHOOK(GetEffectsQuality);
//		SSL_BINDER_UNHOOK(SetEffectsQuality);
//		SSL_BINDER_UNHOOK(GetShadowQualityOptions);
//		SSL_BINDER_UNHOOK(GetShadowQuality);
//		SSL_BINDER_UNHOOK(SetShadowQuality);
//		SSL_BINDER_UNHOOK(GetDetailsQualityOptions);
//		SSL_BINDER_UNHOOK(GetDetailsQuality);
//		SSL_BINDER_UNHOOK(SetDetailsQuality);
//		SSL_BINDER_UNHOOK(GetPostprocessQualityOptions);
//		SSL_BINDER_UNHOOK(GetPostprocessQuality);
//		SSL_BINDER_UNHOOK(SetPostprocessQuality);
//		SSL_BINDER_UNHOOK(GetMotionBlur);
//		SSL_BINDER_UNHOOK(SetMotionBlur);
//		SSL_BINDER_UNHOOK(GetMasterVolume);
//		SSL_BINDER_UNHOOK(SetMasterVolume);
//		SSL_BINDER_UNHOOK(GetSFXVolume);
//		SSL_BINDER_UNHOOK(SetSFXVolume);
//		SSL_BINDER_UNHOOK(GetMusicVolume);
//		SSL_BINDER_UNHOOK(SetMusicVolume);
//		SSL_BINDER_UNHOOK(GetVoiceVolume);
//		SSL_BINDER_UNHOOK(SetVoiceVolume);
//		SSL_BINDER_UNHOOK(GetVoiceChatControl);
//		SSL_BINDER_UNHOOK(SetVoiceChatControl);
//		SSL_BINDER_UNHOOK(GetVoiceChatVolume);
//		SSL_BINDER_UNHOOK(SetVoiceChatVolume);
//		SSL_BINDER_UNHOOK(GetControlsMethod);
//		SSL_BINDER_UNHOOK(SetControlsMethod);
//		SSL_BINDER_UNHOOK(GetMouseSensitivityVert);
//		SSL_BINDER_UNHOOK(GetMouseSensitivityHor);
//		SSL_BINDER_UNHOOK(SetMouseSensitivityVert);
//		SSL_BINDER_UNHOOK(SetMouseSensitivityHor);
//		SSL_BINDER_UNHOOK(GetMouseSensitivityVehicleVert);
//		SSL_BINDER_UNHOOK(GetMouseSensitivityVehicleHor);
//		SSL_BINDER_UNHOOK(SetMouseSensitivityVehicleVert);
//		SSL_BINDER_UNHOOK(SetMouseSensitivityVehicleHor);
//		SSL_BINDER_UNHOOK(GetMouseAcceleration);
//		SSL_BINDER_UNHOOK(SetMouseAcceleration);
//		SSL_BINDER_UNHOOK(GetMouseFilter);
//		SSL_BINDER_UNHOOK(SetMouseFilter);
//		SSL_BINDER_UNHOOK(GetInvertMouse);
//		SSL_BINDER_UNHOOK(SetInvertMouse);
//		SSL_BINDER_UNHOOK(GetLookSensitivity);
//		SSL_BINDER_UNHOOK(SetLookSensitivity);
//		SSL_BINDER_UNHOOK(GetInvertLook);
//		SSL_BINDER_UNHOOK(SetInvertLook);
//		SSL_BINDER_UNHOOK(GetAutoCentering);
//		SSL_BINDER_UNHOOK(SetAutoCentering);
//		SSL_BINDER_UNHOOK(GetButtonLayout);
//		SSL_BINDER_UNHOOK(SetButtonLayout);
//		SSL_BINDER_UNHOOK(GetKeyLabels);
//		SSL_BINDER_UNHOOK(GetActionLabels);
//		SSL_BINDER_UNHOOK(GetDefaultKeyBinds);
//		SSL_BINDER_UNHOOK(GetKeyBinds);
//		SSL_BINDER_UNHOOK(SetKeyBinds);
//	}
//}

namespace HF2P_ENGINE_CTRL
{
//	SSL_BINDER_HOOK(NULL, /*0x00F94E28,*/ 1, 1, ExitGame);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E2C,*/ 1, 0, PlaySound);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E1C,*/ 1, 0, StopSound);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DE0,*/ 1, 0, InitUIScreenSounds);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E08,*/ 1, 0, TermUIScreenSounds);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DF0,*/ 1, 0, PlaySoundHaloUI);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E00,*/ 1, 1, SetCredentials);
	SSL_BINDER_HOOK(NULL, /*0x00F94E24,*/ 0, 1, PerformLogin);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DEC,*/ 1, 1, PerformRelogin);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E18,*/ 1, 0, SetUIMusicId);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E38,*/ 1, 0, SuppressIngameInput);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DF8,*/ 1, 0, ShowFinishedPostpone);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DE8,*/ 1, 0, SetUIPlayerModelId);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E30,*/ 1, 0, SetGamePlayerModelId);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E20,*/ 1, 0, GetUIPlayerModelId);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E3C,*/ 1, 0, SetJaysSpecialCamera);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DF4,*/ 1, 0, SetRotatePlayerModel);
//	SSL_BINDER_HOOK(NULL, /*0x00F94DE4,*/ 1, 0, GetRotatePlayerModel);
//
//	// target_id - weapon target enum, 0,1,2...
//	SSL_BINDER_HOOK(NULL, /*0x00F94E10,*/ 1, 0, OnWeaponScreenKeyDown);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E34,*/ 1, 0, OnWeaponScreenDragStart);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E04,*/ 1, 0, OnWeaponScreenDragUpdate);
//	SSL_BINDER_HOOK(NULL, /*0x00F94E0C,*/ 1, 0, OnWeaponScreenDragEnd);
//
	void revert_all()
	{
//		SSL_BINDER_UNHOOK(ExitGame);
//		SSL_BINDER_UNHOOK(PlaySound);
//		SSL_BINDER_UNHOOK(StopSound);
//		SSL_BINDER_UNHOOK(InitUIScreenSounds);
//		SSL_BINDER_UNHOOK(TermUIScreenSounds);
//		SSL_BINDER_UNHOOK(PlaySoundHaloUI);
//		SSL_BINDER_UNHOOK(SetCredentials);
		SSL_BINDER_UNHOOK(PerformLogin);
//		SSL_BINDER_UNHOOK(PerformRelogin);
//		SSL_BINDER_UNHOOK(SetUIMusicId);
//		SSL_BINDER_UNHOOK(SuppressIngameInput);
//		SSL_BINDER_UNHOOK(ShowFinishedPostpone);
//		SSL_BINDER_UNHOOK(SetUIPlayerModelId);
//		SSL_BINDER_UNHOOK(SetGamePlayerModelId);
//		SSL_BINDER_UNHOOK(GetUIPlayerModelId);
//		SSL_BINDER_UNHOOK(SetJaysSpecialCamera);
//		SSL_BINDER_UNHOOK(SetRotatePlayerModel);
//		SSL_BINDER_UNHOOK(GetRotatePlayerModel);
//		SSL_BINDER_UNHOOK(OnWeaponScreenKeyDown);
//		SSL_BINDER_UNHOOK(OnWeaponScreenDragStart);
//		SSL_BINDER_UNHOOK(OnWeaponScreenDragUpdate);
//		SSL_BINDER_UNHOOK(OnWeaponScreenDragEnd);
	}
}

namespace UI_INGAME
{
	SSL_BINDER_HOOK(NULL, /*0x00F951E0,*/ 1, 1, ScoringEventNext);
	SSL_BINDER_HOOK(NULL, /*0x00F951E8,*/ 1, 1, MedalNext);

	SSL_BINDER_HOOK(NULL, /*0x00F951DC,*/ 1, 1, EarlyRespawnRequest);
	//SSL_BINDER_HOOK(NULL, /*0x00F951E4,*/ 1, 1, IsTutorialLevel);

	void revert_all()
	{
		SSL_BINDER_UNHOOK(ScoringEventNext);
		SSL_BINDER_UNHOOK(MedalNext);
		SSL_BINDER_UNHOOK(EarlyRespawnRequest);
		//SSL_BINDER_UNHOOK(IsTutorialLevel);
	}
}

//namespace UI_GLOBALS
//{
//	SSL_BINDER_HOOK(NULL, /*0x00F951A8,*/ 1, 0, PassesAsWeaponFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F95194,*/ 1, 0, PassesAsPrimaryWeaponFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F951C8,*/ 1, 0, PassesAsSecondaryWeaponFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F951AC,*/ 1, 0, PassesAsGrenadeWeaponFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F9519C,*/ 1, 0, PassesAsArmorFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F9517C,*/ 1, 0, PassesAsTacticalPackageFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F95188,*/ 1, 0, PassesAsSupportPackageFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F951B0,*/ 1, 0, PassesAsEmblemFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F95190,*/ 1, 0, PassesAsAccountLabelFilter);
//	SSL_BINDER_HOOK(NULL, /*0x00F9518C,*/ 1, 0, GetArmorPartType);
//
//	SSL_BINDER_HOOK(NULL, /*0x00F951D0,*/ 1, 0, String2WString);
//	SSL_BINDER_HOOK(NULL, /*0x00F951B4,*/ 1, 0, Int64ToInt);
//	SSL_BINDER_HOOK(NULL, /*0x00F95174,*/ 1, 0, IntToInt64);
//	SSL_BINDER_HOOK(NULL, /*0x00F951D4,*/ 1, 0, String2Int64);
//	SSL_BINDER_HOOK(NULL, /*0x00F951C0,*/ 1, 0, StripHtmlTags);
//	SSL_BINDER_HOOK(NULL, /*0x00F95180,*/ 1, 0, TranslateText);
//	SSL_BINDER_HOOK(NULL, /*0x00F951A4,*/ 1, 0, TranslateWText);
//	SSL_BINDER_HOOK(NULL, /*0x00F951BC,*/ 1, 0, GetUrlComponents);
//	SSL_BINDER_HOOK(NULL, /*0x00F951C4,*/ 1, 0, GetInvalidUIDVal);
//	SSL_BINDER_HOOK(NULL, /*0x00F951CC,*/ 1, 0, CmpStrings);
//	SSL_BINDER_HOOK(NULL, /*0x00F951D8,*/ 1, 0, CmpWStrings);
//	SSL_BINDER_HOOK(NULL, /*0x00F95184,*/ 1, 0, CmpInt64);
//	SSL_BINDER_HOOK(NULL, /*0x00F95178,*/ 1, 0, GetCurrentTimeStamp);
//	SSL_BINDER_HOOK(NULL, /*0x00F951B8,*/ 1, 0, Timestamp2Time);
//	SSL_BINDER_HOOK(NULL, /*0x00F951A0,*/ 1, 0, Timestamp2Date);
//	SSL_BINDER_HOOK(NULL, /*0x00F95198,*/ 1, 0, EscapeString);
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(PassesAsWeaponFilter);
//		SSL_BINDER_UNHOOK(PassesAsPrimaryWeaponFilter);
//		SSL_BINDER_UNHOOK(PassesAsSecondaryWeaponFilter);
//		SSL_BINDER_UNHOOK(PassesAsGrenadeWeaponFilter);
//		SSL_BINDER_UNHOOK(PassesAsArmorFilter);
//		SSL_BINDER_UNHOOK(PassesAsTacticalPackageFilter);
//		SSL_BINDER_UNHOOK(PassesAsSupportPackageFilter);
//		SSL_BINDER_UNHOOK(PassesAsEmblemFilter);
//		SSL_BINDER_UNHOOK(PassesAsAccountLabelFilter);
//		SSL_BINDER_UNHOOK(GetArmorPartType);
//		SSL_BINDER_UNHOOK(String2WString);
//		SSL_BINDER_UNHOOK(Int64ToInt);
//		SSL_BINDER_UNHOOK(IntToInt64);
//		SSL_BINDER_UNHOOK(String2Int64);
//		SSL_BINDER_UNHOOK(StripHtmlTags);
//		SSL_BINDER_UNHOOK(TranslateText);
//		SSL_BINDER_UNHOOK(TranslateWText);
//		SSL_BINDER_UNHOOK(GetUrlComponents);
//		SSL_BINDER_UNHOOK(GetInvalidUIDVal);
//		SSL_BINDER_UNHOOK(CmpStrings);
//		SSL_BINDER_UNHOOK(CmpWStrings);
//		SSL_BINDER_UNHOOK(CmpInt64);
//		SSL_BINDER_UNHOOK(GetCurrentTimeStamp);
//		SSL_BINDER_UNHOOK(Timestamp2Time);
//		SSL_BINDER_UNHOOK(Timestamp2Date);
//		SSL_BINDER_UNHOOK(EscapeString);
//	}
//}

//namespace UI_SSL_BINDABLE_MAP
//{
//	SSL_BINDER_HOOK(NULL, /*0x00F94FFC,*/ 1, 0, Get);
//	SSL_BINDER_HOOK(NULL, /*0x00F95110,*/ 1, 0, Exist);
//	SSL_BINDER_HOOK(NULL, /*0x00F950BC,*/ 1, 0, GetKeys);
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(Get);
//		SSL_BINDER_UNHOOK(Exist);
//		SSL_BINDER_UNHOOK(GetKeys);
//	}
//}

//namespace UI_UID
//{
//	SSL_BINDER_HOOK(NULL, /*0x00F95104,*/ 1, 0, Equals);
//	SSL_BINDER_HOOK(NULL, /*0x00F950B0,*/ 1, 0, IsValid);
//	SSL_BINDER_HOOK(NULL, /*0x00F950CC,*/ 1, 0, ToStr);
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(Equals);
//		SSL_BINDER_UNHOOK(IsValid);
//		SSL_BINDER_UNHOOK(ToStr);
//	}
//}

//namespace HF2P_EMBLEM_EDITOR
//{
//	//SSL_BINDER_HOOK(NULL, 0x00000000, 1, 1, Stub);
//
//	void revert_all()
//	{
//		//SSL_BINDER_UNHOOK(Stub);
//	}
//}

namespace SSL_HQ
{
	//SSL_BINDER_HOOK(NULL, /*0x00F952E4,*/ 0, 1, OpenURL);

	//SSL_BINDER_HOOK(NULL, /*0x00F95358,*/ 0, 1, BackendEnvironmentSet);

	//SSL_BINDER_HOOK(NULL, /*0x00F952C4,*/ 0, 1, NewsMarkAsRead);

	//SSL_BINDER_HOOK(NULL, /*0x00F952F8,*/ 0, 1, ShopVideoBegin);
	//SSL_BINDER_HOOK(NULL, /*0x00F952CC,*/ 0, 1, ShopVideoEnd);
	//SSL_BINDER_HOOK(NULL, /*0x00F95314,*/ 0, 1, ShopNewClear);

	//SSL_BINDER_HOOK(NULL, /*0x00F952FC,*/ 0, 1, TutorialStart);

	SSL_BINDER_HOOK(NULL, /*0x00F9530C,*/ 0, 1, ShowroomBegin); // prevent crash on button click
	//SSL_BINDER_HOOK(NULL, /*0x00F95370,*/ 0, 1, ShowroomTryOn);
	//SSL_BINDER_HOOK(NULL, /*0x00F9537C,*/ 0, 1, ShowroomTryOnArmor);
	//SSL_BINDER_HOOK(NULL, /*0x00F95300,*/ 0, 1, ShowroomTryOnColor);
	//SSL_BINDER_HOOK(NULL, /*0x00F95360,*/ 0, 1, ShowroomEnd);

	SSL_BINDER_HOOK(NULL, /*0x00F95318,*/ 0, 1, ChatSendPrivate);
	SSL_BINDER_HOOK(NULL, /*0x00F9536C,*/ 0, 1, ChatSendGeneral);
	SSL_BINDER_HOOK(NULL, /*0x00F952C8,*/ 0, 1, ChatSendParty);
	SSL_BINDER_HOOK(NULL, /*0x00F95354,*/ 0, 1, ChatSendGameAll);
	SSL_BINDER_HOOK(NULL, /*0x00F952D0,*/ 0, 1, ChatSendGameTeam);
	SSL_BINDER_HOOK(NULL, /*0x00F95364,*/ 0, 1, ChatSendCommand);
	SSL_BINDER_HOOK(NULL, /*0x00F95374,*/ 0, 1, ChatExtractCommand);

	//SSL_BINDER_HOOK(NULL, /*0x00F9534C,*/ 0, 1, PartyCreate);
	//SSL_BINDER_HOOK(NULL, /*0x00F952DC,*/ 0, 1, PartyLeave);
	//SSL_BINDER_HOOK(NULL, /*0x00F95308,*/ 0, 1, PartyInviteSend);
	//SSL_BINDER_HOOK(NULL, /*0x00F952F4,*/ 0, 1, PartyInviteAccept);
	//SSL_BINDER_HOOK(NULL, /*0x00F952EC,*/ 0, 1, PartyInviteRefuse);
	//SSL_BINDER_HOOK(NULL, /*0x00F9533C,*/ 0, 1, PartyKick);

	//SSL_BINDER_HOOK(NULL, /*0x00F95324,*/ 0, 1, MatchmakingSetQueue);
	//SSL_BINDER_HOOK(NULL, /*0x00F95330,*/ 0, 1, MatchmakingEnterQueue);
	//SSL_BINDER_HOOK(NULL, /*0x00F952D4,*/ 0, 1, MatchmakingLeaveQueue);

	//SSL_BINDER_HOOK(NULL, /*0x00F95350,*/ 0, 1, MatchmakingSetPlaylist);

	//SSL_BINDER_HOOK(NULL, /*0x00F95338,*/ 0, 1, CustomGameSetMap);
	//SSL_BINDER_HOOK(NULL, /*0x00F95340,*/ 0, 1, CustomGameSetMode);

	//SSL_BINDER_HOOK(NULL, /*0x00F9535C,*/ 0, 1, LobbyVote);
	//SSL_BINDER_HOOK(NULL, /*0x00F952F0,*/ 0, 1, LobbyVoteReset);

	SSL_BINDER_HOOK(NULL, /*0x00F9531C,*/ 0, 1, TextureRenderBipedProfile); // prevent crash when switching genders
	//SSL_BINDER_HOOK(NULL, /*0x00F95328,*/ 0, 1, TextureRenderBipedGenderSelection);
	//SSL_BINDER_HOOK(NULL, /*0x00F95320,*/ 0, 1, TextureRenderBipedClassSelection);
	//SSL_BINDER_HOOK(NULL, /*0x00F95310,*/ 0, 1, TextureRenderBipedUserCard0);
	//SSL_BINDER_HOOK(NULL, /*0x00F952E0,*/ 0, 1, TextureRenderBipedUserCard1);
	SSL_BINDER_HOOK(NULL, /*0x00F95334,*/ 0, 1, TextureRenderBipedUserCard2); // prevent crash when switching genders
	//SSL_BINDER_HOOK(NULL, /*0x00F952E8,*/ 0, 1, TextureRenderBipedKiller);
	//SSL_BINDER_HOOK(NULL, /*0x00F95304,*/ 0, 1, TextureRenderBipedScoreboard);
	SSL_BINDER_HOOK(NULL, /*0x00F95348,*/ 0, 1, TextureRenderBipedPostmatch); // prevent crash when reloading the mainmenu
	//SSL_BINDER_HOOK(NULL, /*0x00F95344,*/ 0, 1, TextureRenderBipedLoadouts);

	//SSL_BINDER_HOOK(NULL, /*0x00F9532C,*/ 0, 1, TextureRenderWeaponShopStart);
	//SSL_BINDER_HOOK(NULL, /*0x00F95368,*/ 0, 1, TextureRenderWeaponShopStop);
	//SSL_BINDER_HOOK(NULL, /*0x00F95378,*/ 0, 1, TextureRenderWeaponInventoryStart);
	//SSL_BINDER_HOOK(NULL, /*0x00F952D8,*/ 0, 1, TextureRenderWeaponInventoryStop);

	void revert_all()
	{
		//SSL_BINDER_UNHOOK(OpenURL);
		//SSL_BINDER_UNHOOK(BackendEnvironmentSet);
		//SSL_BINDER_UNHOOK(NewsMarkAsRead);
		//SSL_BINDER_UNHOOK(ShopVideoBegin);
		//SSL_BINDER_UNHOOK(ShopVideoEnd);
		//SSL_BINDER_UNHOOK(ShopNewClear);
		//SSL_BINDER_UNHOOK(TutorialStart);
		SSL_BINDER_UNHOOK(ShowroomBegin);
		//SSL_BINDER_UNHOOK(ShowroomTryOn);
		//SSL_BINDER_UNHOOK(ShowroomTryOnArmor);
		//SSL_BINDER_UNHOOK(ShowroomTryOnColor);
		//SSL_BINDER_UNHOOK(ShowroomEnd);
		SSL_BINDER_UNHOOK(ChatSendPrivate);
		SSL_BINDER_UNHOOK(ChatSendGeneral);
		SSL_BINDER_UNHOOK(ChatSendParty);
		SSL_BINDER_UNHOOK(ChatSendGameAll);
		SSL_BINDER_UNHOOK(ChatSendGameTeam);
		SSL_BINDER_UNHOOK(ChatSendCommand);
		SSL_BINDER_UNHOOK(ChatExtractCommand);
		//SSL_BINDER_UNHOOK(PartyCreate);
		//SSL_BINDER_UNHOOK(PartyLeave);
		//SSL_BINDER_UNHOOK(PartyInviteSend);
		//SSL_BINDER_UNHOOK(PartyInviteAccept);
		//SSL_BINDER_UNHOOK(PartyInviteRefuse);
		//SSL_BINDER_UNHOOK(PartyKick);
		//SSL_BINDER_UNHOOK(MatchmakingSetQueue);
		//SSL_BINDER_UNHOOK(MatchmakingEnterQueue);
		//SSL_BINDER_UNHOOK(MatchmakingLeaveQueue);
		//SSL_BINDER_UNHOOK(MatchmakingSetPlaylist);
		//SSL_BINDER_UNHOOK(CustomGameSetMap);
		//SSL_BINDER_UNHOOK(CustomGameSetMode);
		//SSL_BINDER_UNHOOK(LobbyVote);
		//SSL_BINDER_UNHOOK(LobbyVoteReset);
		SSL_BINDER_UNHOOK(TextureRenderBipedProfile);
		//SSL_BINDER_UNHOOK(TextureRenderBipedGenderSelection);
		//SSL_BINDER_UNHOOK(TextureRenderBipedClassSelection);
		//SSL_BINDER_UNHOOK(TextureRenderBipedUserCard0);
		//SSL_BINDER_UNHOOK(TextureRenderBipedUserCard1);
		SSL_BINDER_UNHOOK(TextureRenderBipedUserCard2);
		//SSL_BINDER_UNHOOK(TextureRenderBipedKiller);
		//SSL_BINDER_UNHOOK(TextureRenderBipedScoreboard);
		SSL_BINDER_UNHOOK(TextureRenderBipedPostmatch);
		//SSL_BINDER_UNHOOK(TextureRenderBipedLoadouts);
		//SSL_BINDER_UNHOOK(TextureRenderWeaponShopStart);
		//SSL_BINDER_UNHOOK(TextureRenderWeaponShopStop);
		//SSL_BINDER_UNHOOK(TextureRenderWeaponInventoryStart);
		//SSL_BINDER_UNHOOK(TextureRenderWeaponInventoryStop);
	}
}

//namespace SSL_CLAN_SERVICE
//{
//	SSL_BINDER_HOOK(NULL, /*0x00F9529C,*/ 1, 1, IsValidClanName);
//	SSL_BINDER_HOOK(NULL, /*0x00F952B8,*/ 1, 1, IsValidClanTag);
//	SSL_BINDER_HOOK(NULL, /*0x00F952BC,*/ 1, 1, IsValidClanDescription);
//
//	SSL_BINDER_HOOK(NULL, /*0x00F952AC,*/ 1, 1, AsyncFindClansByName);
//
//	//SSL_BINDER_HOOK(NULL, /*0x00F952B0,*/ 1, 1, RequestData);
//
//	SSL_BINDER_HOOK(NULL, /*0x00F952C0,*/ 1, 1, ClanCreate);
//	SSL_BINDER_HOOK(NULL, /*0x00F952A4,*/ 1, 1, ClanLeave);
//	SSL_BINDER_HOOK(NULL, /*0x00F952A0,*/ 1, 1, ClanInviteSend);
//	SSL_BINDER_HOOK(NULL, /*0x00F952B4,*/ 1, 1, ClanInviteAccept);
//	SSL_BINDER_HOOK(NULL, /*0x00F952A8,*/ 1, 1, ClanKick);
//
//	void revert_all()
//	{
//		SSL_BINDER_UNHOOK(IsValidClanName);
//		SSL_BINDER_UNHOOK(IsValidClanTag);
//		SSL_BINDER_UNHOOK(IsValidClanDescription);
//		SSL_BINDER_UNHOOK(AsyncFindClansByName);
//		//SSL_BINDER_UNHOOK(RequestData);
//		SSL_BINDER_UNHOOK(ClanCreate);
//		SSL_BINDER_UNHOOK(ClanLeave);
//		SSL_BINDER_UNHOOK(ClanInviteSend);
//		SSL_BINDER_UNHOOK(ClanInviteAccept);
//		SSL_BINDER_UNHOOK(ClanKick);
//	}
//}

namespace SSL_USER_SERVICE
{
	//SSL_BINDER_HOOK(NULL, /*0x00F953C8,*/ 1, 1, IsValidNickname);

	//SSL_BINDER_HOOK(NULL, /*0x00F953D4,*/ 1, 1, AsyncFindUsers);
	// 
	////SSL_BINDER_HOOK(NULL, /*0x00F953D8,*/ 1, 1, RequestData);
	//SSL_BINDER_HOOK(NULL, /*0x00F953E0,*/ 1, 1, RequestApplyOffer);
	//SSL_BINDER_HOOK(NULL, /*0x00F953E8,*/ 1, 1, RequestRealMoneyPurchase);
	//SSL_BINDER_HOOK(NULL, /*0x00F95384,*/ 1, 1, RequestFollow);
	//SSL_BINDER_HOOK(NULL, /*0x00F95380,*/ 1, 1, RequestUnfollow);
	//SSL_BINDER_HOOK(NULL, /*0x00F9538C,*/ 1, 1, RequestNicknameChange);
	//SSL_BINDER_HOOK(NULL, /*0x00F953CC,*/ 1, 1, RequestUITutorialComplete);

	SSL_BINDER_HOOK(NULL, /*0x00F953AC,*/ 0, 1, SetAccountLabel);
	SSL_BINDER_HOOK(NULL, /*0x00F95390,*/ 0, 1, SetEmblem);
	SSL_BINDER_HOOK(NULL, /*0x00F95388,*/ 0, 1, SetRace);
	SSL_BINDER_HOOK(NULL, /*0x00F953A4,*/ 0, 1, SetLoadout);

	SSL_BINDER_HOOK(NULL, /*0x00F953A8,*/ 0, 1, SetGender);

	SSL_BINDER_HOOK(NULL, /*0x00F953B0,*/ 0, 1, SetArmorLoadoutActive);
	SSL_BINDER_HOOK(NULL, /*0x00F953B4,*/ 0, 1, SetArmorLoadoutItem);
	SSL_BINDER_HOOK(NULL, /*0x00F953A0,*/ 0, 1, SetArmorLoadoutColors);
	SSL_BINDER_HOOK(NULL, /*0x00F953DC,*/ 0, 1, SetWeaponLoadoutActive);
	SSL_BINDER_HOOK(NULL, /*0x00F95394,*/ 0, 1, SetWeaponLoadoutWeapon);
	SSL_BINDER_HOOK(NULL, /*0x00F953BC,*/ 0, 1, SetWeaponLoadoutGrenade);
	SSL_BINDER_HOOK(NULL, /*0x00F953C4,*/ 0, 1, SetWeaponLoadoutBooster);
	SSL_BINDER_HOOK(NULL, /*0x00F953B8,*/ 0, 1, SetWeaponLoadoutConsumable);

	SSL_BINDER_HOOK(NULL, /*0x00F953C0,*/ 0, 1, SetCharacterActive);
	SSL_BINDER_HOOK(NULL, /*0x00F953D0,*/ 0, 1, SwitchCharacterGender);
	SSL_BINDER_HOOK(NULL, /*0x00F953E4,*/ 0, 1, EquipCharacter);
	SSL_BINDER_HOOK(NULL, /*0x00F9539C,*/ 0, 1, EquipCharacterWithSlot);
	SSL_BINDER_HOOK(NULL, /*0x00F95398,*/ 0, 1, SetCharacterColor);

	void revert_all()
	{
		//SSL_BINDER_UNHOOK(IsValidNickname);
		//SSL_BINDER_UNHOOK(AsyncFindUsers);
		////SSL_BINDER_UNHOOK(RequestData);
		//SSL_BINDER_UNHOOK(RequestApplyOffer);
		//SSL_BINDER_UNHOOK(RequestRealMoneyPurchase);
		//SSL_BINDER_UNHOOK(RequestFollow);
		//SSL_BINDER_UNHOOK(RequestUnfollow);
		//SSL_BINDER_UNHOOK(RequestNicknameChange);
		//SSL_BINDER_UNHOOK(RequestUITutorialComplete);
		SSL_BINDER_UNHOOK(SetAccountLabel);
		SSL_BINDER_UNHOOK(SetEmblem);
		SSL_BINDER_UNHOOK(SetRace);
		SSL_BINDER_UNHOOK(SetLoadout);
		SSL_BINDER_UNHOOK(SetGender);
		SSL_BINDER_UNHOOK(SetArmorLoadoutActive);
		SSL_BINDER_UNHOOK(SetArmorLoadoutItem);
		SSL_BINDER_UNHOOK(SetArmorLoadoutColors);
		SSL_BINDER_UNHOOK(SetWeaponLoadoutActive);
		SSL_BINDER_UNHOOK(SetWeaponLoadoutWeapon);
		SSL_BINDER_UNHOOK(SetWeaponLoadoutGrenade);
		SSL_BINDER_UNHOOK(SetWeaponLoadoutBooster);
		SSL_BINDER_UNHOOK(SetWeaponLoadoutConsumable);
		SSL_BINDER_UNHOOK(SetCharacterActive);
		SSL_BINDER_UNHOOK(SwitchCharacterGender);
		SSL_BINDER_UNHOOK(EquipCharacter);
		SSL_BINDER_UNHOOK(EquipCharacterWithSlot);
		SSL_BINDER_UNHOOK(SetCharacterColor);
	}
}
