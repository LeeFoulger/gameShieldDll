#pragma once

namespace console
{
	FILE* dummy_file = nullptr;

	void on_console_create();

#if _DEBUG
	bool console_created = []() -> bool
	{
		if (AllocConsole())
		{
			freopen_s(&dummy_file, "CONOUT$", "w", stdout);
			freopen_s(&dummy_file, "CONOUT$", "w", stderr);
			freopen_s(&dummy_file, "CONIN$", "r", stdin);

			console_print("console created\n");

			on_console_create();

			return true;
		}

		return false;
	}();
#endif // _DEBUG
}
