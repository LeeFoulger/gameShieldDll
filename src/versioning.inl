#pragma once



union u_product_version
{
	unsigned long long value;

	struct
	{
		unsigned long long major_version : 16;
		unsigned long long minor_version : 16;
		unsigned long long build_number : 32;
	} split;
};

constexpr unsigned long long make_product_version(unsigned long long a, unsigned long long b, unsigned long long c)
{
	return ((a << 0) | (b << 16) | (c << 32));
}

struct s_module_info
{
	u_product_version product_version;
	time_t timestamp;
	c_string<wchar_t, 64> class_name;
};

enum e_product_version : unsigned long long
{
	_product_version_ms23_106708 = make_product_version(0, 1, 106708),
	_product_version_ms23_155080 = make_product_version(0, 1, 155080),
	_product_version_ms23_171227 = make_product_version(0, 1, 171227),
	_product_version_ms23_177150 = make_product_version(0, 1, 177150),
	_product_version_ms25_235640 = make_product_version(0, 1, 235640),
	_product_version_ms26_301003 = make_product_version(0, 1, 301003),
	_product_version_ms26_332089 = make_product_version(4, 1, 332089),
	_product_version_ms27_373869 = make_product_version(8, 1, 373869),
	_product_version_ms27_416138 = make_product_version(9, 1, 416138),
	_product_version_ms28_430653 = make_product_version(10, 1, 430653),
	_product_version_ms28_454665 = make_product_version(10, 1, 454665),
	_product_version_ms28_479394 = make_product_version(10, 1, 479394),
	_product_version_ms29_498295 = make_product_version(11, 1, 498295),
	_product_version_ms29_530945 = make_product_version(11, 1, 530945),
	_product_version_ms29_533032 = make_product_version(11, 1, 533032),
	_product_version_ms29_554482 = make_product_version(11, 1, 554482),
	_product_version_ms29_571698 = make_product_version(11, 1, 571698),
	_product_version_ms29_604673 = make_product_version(11, 1, 604673),
	_product_version_ms30_700255 = make_product_version(12, 1, 700255)
};

//	module being executable, strings taken from the executable
//		HaloOnline %s_%d
//		Halo Online Game Client %s
//	cert_ms23_release_106708, first appearance of 'eldorado.exe'
//	Live_release_8.1.373869, first appearance of 'halo_online.exe'
s_module_info module_info_array[]
{
	// D:\Projects\eldorado_cert_ms23_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms23_106708, 0x550C2DC5, L"HaloOnline cert_ms23_release_106708_0" },
	{ _product_version_ms23_155080, 0x5527AC33, L"HaloOnline cert_ms23_release_155080_0" },

	// D:\Projects\depot\Eldorado\branches\cert_ms23\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms23_171227, 0x5530F668, L"HaloOnline cert_ms23_release_171227_0" },

	// D:\Projects\eldorado_cert_ms23_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms23_177150, 0x553645B4, L"HaloOnline cert_ms23_release_177150_0" },

	// D:\Projects\eldorado_cert_ms25_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms25_235640, 0x555B600B, L"HaloOnline cert_ms25_release_235640_0" },

	// D:\Projects\eldorado_cert_ms26new_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms26_301003, 0x557BFFE2, L"Halo Online Game Client cert_MS26_new_release_0.0.1.301003" },
	{ _product_version_ms26_332089, 0x55912489, L"Halo Online Game Client Live_release_0.4.1.332089" },

	// D:\Projects\eldorado_cert_ms27new_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms27_373869, 0x55A62757, L"Halo Online Game Client Live_release_8.1.373869" },
	{ _product_version_ms27_416138, 0x55BCD41A, L"Halo Online Game Client Live_release_9.1.416138" },

	// D:\Projects\eldorado_cert_ms28_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms28_430653, 0x55C4BA0C, L"Halo Online Game Client Live_release_10.1.430653" },
	{ _product_version_ms28_454665, 0x55D4534D, L"Halo Online Game Client Live_release_10.1.454665" },
	{ _product_version_ms28_479394, 0x55DF314B, L"Halo Online Game Client Live_release_10.1.479394" },

	// D:\Projects\eldorado_cert_ms29_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms29_498295, 0x55E87D63, L"Halo Online Game Client Live_release_11.1.498295" },
	{ _product_version_ms29_530945, 0x55F981A0, L"Halo Online Game Client Live_release_11.1.530945" },
	{ _product_version_ms29_533032, 0x55FAA70D, L"Halo Online Game Client Live_release_11.1.533032" },
	{ _product_version_ms29_554482, 0x560525DE, L"Halo Online Game Client Live_release_11.1.554482" },
	{ _product_version_ms29_571698, 0x560D4BA1, L"Halo Online Game Client Live_release_11.1.571698" },
	{ _product_version_ms29_604673, 0x561F8159, L"Halo Online Game Client Live_release_11.1.604673" },

	// D:\Projects\eldorado_cert_ms30_code\Halo3\bin\hf2p_game_client_cache_release.pdb
	{ _product_version_ms30_700255, 0x565493D6, L"Halo Online Game Client cert_ms30_oct19_release_12.1.700255" }
};

void module_info_get(s_module_info& out_module_info)
{
	static s_module_info module_info = { 0xFFFFFFFFFFFFFFFFull, 0, NULL };

	if (module_info.product_version.value == 0xFFFFFFFFFFFFFFFFull || module_info.class_name == NULL)
	{
		WCHAR class_name[64]{};
		GetClassName(GetActiveWindow(), class_name, 64);

		for (s_module_info& info : module_info_array)
		{
			if (info.class_name == NULL || wcscmp(class_name, info.class_name) != 0)
				continue;

			::memcpy(&module_info, &info, sizeof(s_module_info));
		}
	}

	::memcpy(&out_module_info, &module_info, sizeof(s_module_info));
}
