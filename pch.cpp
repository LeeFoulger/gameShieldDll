// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

#if _DEBUG
int console_print(const char* format, ...)
{
	va_list va_args;
	va_start(va_args, format);

	int result = vprintf_s(format, va_args);

	va_end(va_args);

	return result;
}
int console_print(const wchar_t* format, ...)
{
	va_list va_args;
	va_start(va_args, format);

	int result = vwprintf_s(format, va_args);

	va_end(va_args);

	return result;
}
#else
int console_print(const char* format, ...)
{
	char* buffer = new char[4096]{};

	va_list va_args;
	va_start(va_args, format);

	int result = vsprintf_s(buffer, 4096, format, va_args);
	OutputDebugStringA(buffer);

	va_end(va_args);

	delete[] buffer;
	buffer = nullptr;

	return result;
}
int console_print(const wchar_t* format, ...)
{
	wchar_t* buffer = new wchar_t[4096]{};

	va_list va_args;
	va_start(va_args, format);

	int result = vswprintf_s(buffer, 4096, format, va_args);
	OutputDebugStringW(buffer);

	va_end(va_args);

	delete[] buffer;
	buffer = nullptr;

	return result;
}
#endif // _DEBUG