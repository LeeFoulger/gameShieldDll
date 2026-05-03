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
	HRESULT hs = S_OK;

	if (SUCCEEDED(hs = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		IFileOpenDialog* file_dialog = nullptr;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<LPVOID*>(&file_dialog))))
		{
			if (file_dialog != nullptr)
			{
				if (SUCCEEDED(file_dialog->SetFileTypes(parameters.file_filter_count, parameters.file_filter)))
				{
					c_path_wide path{};
					wchar_t* _wfullpath_result = _wfullpath(path, parameters.default_folder, _countof(path));

					IShellItem* dir_shell_item;
					if (SUCCEEDED(SHCreateItemFromParsingName(path, NULL, IID_PPV_ARGS(&dir_shell_item))))
					{
						if (SUCCEEDED(file_dialog->SetDefaultFolder(dir_shell_item)))
						{
							if (SUCCEEDED(file_dialog->SetFileName(parameters.default_filename)))
							{
								if (SUCCEEDED(file_dialog->SetOptions(FOS_NOCHANGEDIR | FOS_FILEMUSTEXIST)))
								{
									if (SUCCEEDED(file_dialog->SetOkButtonLabel(parameters.ok_button_label)))
									{
										if (SUCCEEDED(file_dialog->Show(NULL)))
										{
											IShellItem* shell_item = nullptr;
											if (SUCCEEDED(file_dialog->GetResult(&shell_item)))
											{
												LPWCH display_name_wide = nullptr;
												if (SUCCEEDED(shell_item->GetDisplayName(SIGDN_FILESYSPATH, &display_name_wide)) || display_name_wide == nullptr)
												{
													char* display_name = static_cast<char*>(CoTaskMemAlloc(MAX_PATH));
													snprintf(display_name, MAX_PATH, "%ls", display_name_wide);
													csstrnzcpy(out_file, display_name, MAX_PATH);

													CoTaskMemFree(display_name);
													CoTaskMemFree(display_name_wide);
													shell_item->Release();
													file_dialog->Release();
													CoUninitialize();
												}
											}
										}
										else
										{
											snprintf(out_file, MAX_PATH, "%ls\\%ls", path, parameters.default_filename);
											hs = E_FAIL;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return hs;
}
#pragma warning(pop)

