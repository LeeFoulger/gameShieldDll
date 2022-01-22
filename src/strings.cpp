#include "pch.h"

char* csstrnzcpy(char* s1, const char* s2, unsigned long size)
{
	unsigned long s2_size = static_cast<unsigned long>(strlen(s2));
	strncpy_s(s1, size, s2, 0xFFFFFFFF);
	if (s2_size >= size)
		s1[size - 1] = 0;
	else
		memset(&s1[s2_size], 0, (size - s2_size) * sizeof(char));
	return s1;
}

wchar_t* cswcsnzcpy(wchar_t* s1, const wchar_t* s2, unsigned long size)
{
	unsigned long s2_size = static_cast<unsigned long>(wcslen(s2));
	wcsncpy_s(s1, size, s2, 0xFFFFFFFF);
	if (s2_size >= size)
		s1[size - 1] = 0;
	else
		memset(&s1[s2_size], 0, (size - s2_size) * sizeof(wchar_t));
	return s1;
}
