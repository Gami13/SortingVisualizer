namespace Files
{

	std::wstring selectFile()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
											  COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog *pFileOpen;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
								  IID_IFileOpenDialog, reinterpret_cast<void **>(&pFileOpen));
			COMDLG_FILTERSPEC rgSpec[] = {
				{L"Text Files", L"*.txt"}};
			pFileOpen->SetFileTypes(1, rgSpec);

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							return pszFilePath;
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return 0;
	}
	std::vector<long long int> loadFrom(std::string path)
	{
		std::ifstream file;
		file.open(path);

		std::vector<long long int> output;
		std::string line;
		while (getline(file, line, ','))
		{
			output.push_back(std::stoi(line));
		}
		return output;
	}
}