#pragma once

struct s_file_open_dialog_parameters
{
	const wchar_t* default_folder;
	const wchar_t* default_filename;
	COMDLG_FILTERSPEC* file_filter;
	unsigned long file_filter_count;
	const wchar_t* ok_button_label;
};

#pragma warning(push)
#pragma warning(disable: 28183)
unsigned long file_open_dialog(c_path& out_file, s_file_open_dialog_parameters& parameters)
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return E_FAIL;

	IFileOpenDialog* file_dialog = nullptr;
	if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<LPVOID*>(&file_dialog))))
		return E_FAIL;

	if (!file_dialog)
		return E_FAIL;

	if (FAILED(file_dialog->SetFileTypes(parameters.file_filter_count, parameters.file_filter)))
		return E_FAIL;

	c_path_wide path{};
	wchar_t* _wfullpath_result = _wfullpath(path, parameters.default_folder, _countof(path));

	IShellItem* dir_shell_item;
	if (FAILED(SHCreateItemFromParsingName(path, NULL, IID_PPV_ARGS(&dir_shell_item))))
		return E_FAIL;

	if (FAILED(file_dialog->SetDefaultFolder(dir_shell_item)))
		return E_FAIL;

	if (FAILED(file_dialog->SetFileName(parameters.default_filename)))
		return E_FAIL;

	if (FAILED(file_dialog->SetOptions(FOS_NOCHANGEDIR | FOS_FILEMUSTEXIST)))
		return E_FAIL;

	if (FAILED(file_dialog->SetOkButtonLabel(parameters.ok_button_label)))
		return E_FAIL;

	if (FAILED(file_dialog->Show(NULL)))
	{
		snprintf(out_file, MAX_PATH, "%ls\\%ls", path, parameters.default_filename);
		return E_FAIL;
	}

	IShellItem* shell_item{};
	if (FAILED(file_dialog->GetResult(&shell_item)))
		return E_FAIL;

	LPWCH display_name_wide{};
	if (FAILED(shell_item->GetDisplayName(SIGDN_FILESYSPATH, &display_name_wide)) || display_name_wide == nullptr)
		return E_FAIL;

	char* display_name = static_cast<char*>(CoTaskMemAlloc(MAX_PATH));
	snprintf(display_name, MAX_PATH, "%ls", display_name_wide);
	csstrnzcpy(out_file, display_name, MAX_PATH);

	CoTaskMemFree(display_name);
	CoTaskMemFree(display_name_wide);
	shell_item->Release();
	file_dialog->Release();
	CoUninitialize();

	return S_OK;
}
#pragma warning(pop)
