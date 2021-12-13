#pragma once

constexpr unsigned long long make_product_version(unsigned long long a, unsigned long long b, unsigned long long c)
{
	return ((a << 48) | (b << 32) | (c << 0));
}

struct s_module_version
{
	unsigned long long product_version;
	c_string<wchar_t, 64> class_name;
};

// executable product version not fucking cache version
enum e_product_version : unsigned long long
{
	_product_version_ms23_release_00_1_106708 = make_product_version(0, 1, 106708),
	_product_version_ms23_release_00_1_155080 = make_product_version(0, 1, 155080),
	_product_version_ms23_release_00_1_171227 = make_product_version(0, 1, 171227),
	_product_version_ms23_release_00_1_177150 = make_product_version(0, 1, 177150),
	_product_version_ms25_release_00_1_235640 = make_product_version(0, 1, 235640),
	_product_version_ms26_release_00_1_301003 = make_product_version(0, 1, 301003),
	_product_version_live_release_04_1_332089 = make_product_version(4, 1, 332089),
	_product_version_live_release_08_1_373869 = make_product_version(8, 1, 373869),
	_product_version_live_release_09_1_416138 = make_product_version(9, 1, 416138),
	_product_version_live_release_10_1_430653 = make_product_version(10, 1, 430653),
	_product_version_live_release_10_1_454665 = make_product_version(10, 1, 454665),
	_product_version_live_release_10_1_479394 = make_product_version(10, 1, 479394),
	_product_version_live_release_11_1_498295 = make_product_version(11, 1, 498295),
	_product_version_live_release_11_1_530945 = make_product_version(11, 1, 530945),
	_product_version_live_release_11_1_533032 = make_product_version(11, 1, 533032),
	_product_version_live_release_11_1_554482 = make_product_version(11, 1, 554482),
	_product_version_live_release_11_1_571698 = make_product_version(11, 1, 571698),
	_product_version_live_release_11_1_604673 = make_product_version(11, 1, 604673),
	_product_version_ms30_release_12_1_700255 = make_product_version(12, 1, 700255)
};

// module being executable
s_module_version module_versions[]
{
	{ _product_version_ms23_release_00_1_106708, L"HaloOnline cert_ms23_release_106708_0" }, // first appearance of 'eldorado.exe'
	{ _product_version_ms23_release_00_1_155080, L"HaloOnline cert_ms23_release_155080_0" },
	{ _product_version_ms23_release_00_1_171227, L"HaloOnline cert_ms23_release_171227_0" },
	{ _product_version_ms23_release_00_1_177150, L"HaloOnline cert_ms23_release_177150_0" },
	{ _product_version_ms25_release_00_1_235640, L"HaloOnline cert_ms25_release_235640_0" },
	{ _product_version_ms26_release_00_1_301003, L"Halo Online Game Client cert_MS26_new_release_0.0.1.301003" },
	{ _product_version_live_release_04_1_332089, L"Halo Online Game Client Live_release_0.4.1.332089" },
	{ _product_version_live_release_08_1_373869, L"Halo Online Game Client Live_release_8.1.373869" }, // first appearance of 'halo_online.exe'
	{ _product_version_live_release_09_1_416138, L"Halo Online Game Client Live_release_9.1.416138" },
	{ _product_version_live_release_10_1_430653, L"Halo Online Game Client Live_release_10.1.430653" },
	{ _product_version_live_release_10_1_454665, L"Halo Online Game Client Live_release_10.1.454665" },
	{ _product_version_live_release_10_1_479394, L"Halo Online Game Client Live_release_10.1.479394" },
	{ _product_version_live_release_11_1_498295, L"Halo Online Game Client Live_release_11.1.498295" },
	{ _product_version_live_release_11_1_530945, L"Halo Online Game Client Live_release_11.1.530945" },
	{ _product_version_live_release_11_1_533032, L"Halo Online Game Client Live_release_11.1.533032" },
	{ _product_version_live_release_11_1_554482, L"Halo Online Game Client Live_release_11.1.554482" },
	{ _product_version_live_release_11_1_571698, L"Halo Online Game Client Live_release_11.1.571698" },
	{ _product_version_live_release_11_1_604673, L"Halo Online Game Client Live_release_11.1.604673" },
	{ _product_version_ms30_release_12_1_700255, L"Halo Online Game Client cert_ms30_oct19_release_12.1.700255" }
};

unsigned long long& version_get(c_string<wchar_t, 64>* out_class_name = nullptr)
{
	static s_module_version module_version = { 0xFFFFFFFFFFFFFFFFull, NULL };

	if (module_version.product_version == 0xFFFFFFFFFFFFFFFFull || module_version.class_name == NULL)
	{
		WCHAR class_name[64]{};
		GetClassName(GetActiveWindow(), class_name, 64);

		for (s_module_version& version : module_versions)
		{
			if (version.class_name == NULL || wcscmp(class_name, version.class_name) != 0)
				continue;

			::memcpy(&module_version, &version, sizeof(s_module_version));
		}
	}

	if (out_class_name)
		cswcsnzcpy(*out_class_name, module_version.class_name, 64);
	return module_version.product_version;
}
