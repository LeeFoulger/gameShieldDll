#pragma once

namespace console
{
	FILE* dummy_file = nullptr;

#if _DEBUG
	bool console_create = []() -> bool
	{
		if (AllocConsole())
		{
			freopen_s(&dummy_file, "CONOUT$", "w", stdout);
			freopen_s(&dummy_file, "CONOUT$", "w", stderr);
			freopen_s(&dummy_file, "CONIN$", "r", stdin);

			printf("console created\n");

			return true;
		}

		return false;
	}();
#endif // _DEBUG
}
