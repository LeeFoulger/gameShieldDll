#pragma once

inline void on_key_press(WPARAM wParam, int virtual_key, void(*callback)(void*), void* userdata = nullptr)
{
	if (!callback)
		return;

	if (wParam == virtual_key)
		callback(userdata);
}

namespace key_handler
{
	LONG_PTR original_window_proc = NULL;

	LRESULT WINAPI window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	DWORD WINAPI thread_func(LPVOID param)
	{
		if (param)
			original_window_proc = SetWindowLongPtr(FindWindow(NULL, static_cast<s_module_info*>(param)->class_name), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_proc));

		return S_OK;
	}
}
