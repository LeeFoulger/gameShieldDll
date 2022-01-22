#pragma once

template<typename t_type, unsigned long size>
using c_string = t_type[size];

using c_path = c_string<char, MAX_PATH>;
using c_path_wide = c_string<wchar_t, MAX_PATH>;

char* csstrnzcpy(char* s1, const char* s2, unsigned long size);
wchar_t* cswcsnzcpy(wchar_t* s1, const wchar_t* s2, unsigned long size);
