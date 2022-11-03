// compile with: $ g++ -o main.exe main.cpp -lgdi32 -lole32 -loleaut32 -luuid
// optimized: $ g++ -o main.exe main.cpp -lgdi32 -lole32 -loleaut32 -luuid -Ofast -march=native -flto

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <shobjidl.h>
#include <chrono>
#include <fstream>
#include <thread>

// The main window class name.
static char WINDOW_NAME[] = _T("SortTest");
static char WINDOW_TITLE[] = _T("Sorting algorithms speed test");
static int WINDOW_WIDTH = 1600;
static int WINDOW_HEIGHT = 900;
HINSTANCE instance;
HINSTANCE VisualizationInstance;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), 0x00000000L);
		return 1;
	}

	instance = hInstance;
	HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, WINDOW_NAME, WINDOW_TITLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), 0x00000000L);

		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/* quicksort with timer */
std::string vectorToString(std::vector<long long int> v)
{
	std::string s = "";
	for (int i = 0; i < v.size(); i++)
	{
		s += std::to_string(v[i]) + ", ";
	}
	return s;
}
struct Step
{
	int i;
	int j;
};

std::vector<Step> steps;
void quicksortB(std::vector<long long int> &arr, int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			/* 			Step s;
						s.i = i;
						s.j = j;
						steps.push_back(s); // save step for drawing */
			tmp = arr[i];
			arr[i] = arr[j];

			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quicksortB(arr, left, j);
	if (i < right)
		quicksortB(arr, i, right);
}
void quicksort(std::vector<long long int> &arr)
{
	quicksortB(arr, 0, arr.size() - 1);
}

std::wstring
selectFile()
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
std::vector<long long int> fromFile;
std::vector<long long int> toBeSorted;
std::string filepath;
static unsigned int BUTTON_HEIGHT = 60;
static unsigned int BUTTON_WIDTH = 200;
static unsigned int BUTTON_MARGIN = 8;
const long long int LOAD_BUTTON = 1;
const long long int QUICKSORT_BUTTON = 2;
const long long int GENERATE_RANDOM_BUTTON = 3;
const long long int SAVE_BUTTON = 4;
const long long int AMOUNT_TO_GENERATE = 6;
const long long int SAVE_SORTED_BUTTON = 5;
const long long int STATS_DISPLAY = 7;
const long long int VISUALIZATION = 8;
const long long int UNSORTED_DISPLAY = 9;
const long long int SORTED_DISPLAY = 10;
int amountToGenerate;
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (message)
	{
	case WM_CREATE:
		CreateWindow(
			"BUTTON", "Load file to sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)LOAD_BUTTON, NULL, NULL);
		CreateWindow(
			"BUTTON", "Quick sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN), BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)QUICKSORT_BUTTON, NULL, NULL);
		CreateWindow(
			"BUTTON", "Generate random", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 2, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)GENERATE_RANDOM_BUTTON, NULL, NULL);
		CreateWindow(
			"BUTTON", "Save random to file", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 3, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SAVE_BUTTON, NULL, NULL);
		CreateWindow(
			"BUTTON", "Save sorted to file", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 4, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SAVE_SORTED_BUTTON, NULL, NULL);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_NUMBER, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 5, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)AMOUNT_TO_GENERATE, NULL, NULL);
		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | EM_SETREADONLY, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 6, BUTTON_WIDTH, 440, hwnd, (HMENU)STATS_DISPLAY, NULL, NULL);
		CreateWindowEx(0, "STATIC", (LPCSTR)NULL, WS_CHILD | WS_BORDER | SW_SHOW | WS_VISIBLE, BUTTON_MARGIN * 2 + BUTTON_WIDTH, BUTTON_MARGIN, 1365, 600, hwnd, (HMENU)VISUALIZATION, VisualizationInstance, NULL);

		EnumChildWindows(hwnd, EnumChildProc, (LPARAM)NULL);
		/* the same for read and sorted */

	case WM_COMMAND:

		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
		{
			if (LOWORD(wParam) == AMOUNT_TO_GENERATE)
			{
				int len = GetWindowTextLength((HWND)lParam);
				char *buffer = new char[len + 1];
				GetWindowText((HWND)lParam, buffer, len + 1);
				std::string s = buffer;
				if (s != "")
				{
					amountToGenerate = std::stoi(s);
				}
				else
				{
					amountToGenerate = 0;
				}

				delete[] buffer;
			}
			break;
		}
		}

		switch (LOWORD(wParam))
		{
		case SAVE_SORTED_BUTTON:
		{
			std::wstring temp = selectFile();
			std::string filepath = std::string(temp.begin(), temp.end());
			std::ofstream file;
			file.open(filepath);
			for (int i = 0; i < toBeSorted.size(); i++)
			{
				file << toBeSorted[i] << ",";
			}
			file.close();
			break;
		}
		case GENERATE_RANDOM_BUTTON:
		{
			std::vector<long long int> random;
			for (int i = 0; i < amountToGenerate; i++)
			{
				random.push_back(rand() % 10000 - 5000);
			}
			fromFile = random;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case SAVE_BUTTON:
		{
			std::wstring temp = selectFile();
			std::string filepath = std::string(temp.begin(), temp.end());
			std::ofstream file;
			file.open(filepath);
			for (int i; i < fromFile.size(); i++)
			{
				file << fromFile[i] << ",";
			}
			file.close();
			break;
		}
		case LOAD_BUTTON:
		{
			std::wstring temp = selectFile();
			filepath = std::string(temp.begin(), temp.end());
			fromFile = loadFrom(filepath);
			/* 	std::cout << fromFile << std::endl; */
			std::cout << filepath << std::endl;
			InvalidateRect(hwnd, NULL, TRUE);

			break;
		}

		case QUICKSORT_BUTTON:
		{
			/* start timer */
			toBeSorted = fromFile;
			/* std::vector<long long int> arr2 = toBeSorted; */
			auto start = std::chrono::steady_clock::now();
			quicksort(toBeSorted);
			/* end timer */
			auto finish = std::chrono::steady_clock::now();

			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms" << std::endl;
			/* WAS CHECKING IF STEPS ARE CORRECT
			for (int i = 0; i < steps.size(); i++)
				{
					std::cout << steps[i].i << ", " << steps[i].j << " | ";
					long long int tmp = arr2[steps[i].i];
					arr2[steps[i].i] = arr2[steps[i].j];

					arr2[steps[i].j] = tmp;
				}
				for (int i = 0; i < arr2.size(); i++)
				{
					std::cout << arr2[i] << ", ";
				} */

			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 100, 150, vectorToString(fromFile).c_str(), vectorToString(fromFile).length());

		TextOut(hdc, 100, 350, vectorToString(toBeSorted).c_str(), vectorToString(toBeSorted).length());

		/* TextOut(hdc, 200, 200, fromFile.c_str(), fromFile.length()); */

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return 0;
}
HHOOK _hook;


LRESULT __stdcall childMessageCallback(int message, WPARAM wParam, LPARAM lParam)
{
	
	std::cout << message << std::endl;
return CallNextHookEx(_hook, message, wParam, lParam);
	
}

void setChildHook()
{
	std::cout << "hook" << std::endl;
	if (!(_hook = SetWindowsHookEx(WH_GETMESSAGE, childMessageCallback, VisualizationInstance, GetCurrentThreadId())))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
	 MSG msg;
	 while (GetMessage(&msg, nullptr, 0, 0))
    {
         TranslateMessage(&msg);
            DispatchMessage(&msg);
        
    }
/* nie dziala xd */
 
}

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{

	int idChild = GetWindowLong(hwndChild, GWL_ID);

	if (idChild == 8)
	{

		setChildHook();

		std::cout << "Zlapalem dziecko :D" << std::endl;
		/* PAINTSTRUCT ps;
		HDC hdcChild = BeginPaint(hwndChild, &ps);

		SetBkColor(hdcChild, RGB(0,0,0));
		 */
		InvalidateRect(hwndChild, NULL, TRUE);
	}
	return TRUE;
}

int main()
{
	WinMain(instance, NULL, 0, SW_SHOW);
}

/* ADD SORTING, DISPLAYING BEFORE SORT, AFTER SORT, TIME IT TOOK, NUMBER OF ITERATIONS, NUMBER OF CHANGES AND CREATE VISUALIZATION */