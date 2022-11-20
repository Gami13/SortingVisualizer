// ONLY COMPILES WITH -O1 or less
// compile with: $ g++ -o main.exe main.cpp -lgdi32 -lole32 -loleaut32 -luuid -std=c++20
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
#include <algorithm>
#include <numeric>
#include <future>
struct Step
{
	int i;
	int j;
};

std::vector<Step> steps;
float fastestSort = 0;
float slowestSort = 0;
float averageSort = 0;
int amountOfIterations = 0;
int amountOfComparisons = 0;

#include "sorts.hpp"
#include "files.hpp"

#define LOAD_BUTTON 1
#define QUICKSORT_BUTTON 2
#define GENERATE_RANDOM_BUTTON 3
#define SAVE_BUTTON 4
#define AMOUNT_TO_GENERATE 6
#define SAVE_SORTED_BUTTON 5
#define STATS_DISPLAY 7
#define VISUALIZATION 8
#define UNSORTED_DISPLAY 9
#define SORTED_DISPLAY 10
#define BUBBLE_SORT_BUTTON 11
#define INSERTION_SORT_BUTTON 12
#define SELECTION_SORT_BUTTON 13
#define COCKTAIL_SORT_BUTTON 14
#define HEAP_SORT_BUTTON 15
#define COMB_SORT_BUTTON 16
#define SHELL_SORT_BUTTON 17
#define CYCLE_SORT_BUTTON 18
#define GNOME_SORT_BUTTON 19
#define PASSES_AMOUNT 20
#define STEPS_DELAY 21

#define BUTTON_HEIGHT 40
#define BUTTON_WIDTH 200
#define BUTTON_MARGIN 8
#define GENERATOR_HEIGHT 35
#define SHORT_BUTTON_WIDTH BUTTON_WIDTH / 2 - BUTTON_MARGIN / 2
#define LEFT_BUTTON_X_POS BUTTON_MARGIN
#define RIGHT_BUTTON_X_POS BUTTON_MARGIN + SHORT_BUTTON_WIDTH + BUTTON_MARGIN

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define VISUALIZATION_WIDTH 1365
#define VISUALIZATION_HEIGHT 600
std::string vectorToString(std::vector<unsigned int> v)
{
	std::string s = "";
	for (int i = 0; i < v.size(); i++)
	{
		s += std::to_string(v[i]) + ", ";
	}
	return s;
}

std::vector<unsigned int> fromFile;
std::vector<unsigned int> toBeSorted;
std::vector<unsigned int> visualizationArray;
std::string filepath;
bool shouldVisualize = false;

int amountToGenerate = 0;
int passesAmount = 1;
int stepsDelay = 0;

// The main window class name.
static char WINDOW_NAME[] = _T("SortTest");
static char WINDOW_TITLE[] = _T("Sorting algorithms speed test");

HINSTANCE instance;
HINSTANCE VisualizationInstance;
HWND w2;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/* child messages */
LRESULT CALLBACK Proc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc{GetDC(hwnd)};
	HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	RECT fullPaint = {0, 0, VISUALIZATION_WIDTH, VISUALIZATION_HEIGHT};
	HBRUSH solidBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));

	HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));

	switch (msg)
	{
	case WM_CREATE:
		printf("-------\n");
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (!shouldVisualize)
		{
			FillRect(hdc, &fullPaint, solidBrush);
		}

		if (shouldVisualize)
		{
			/* 		std::cout << "VISUALIZING..." << std::endl;
					const double WIDTH_RATIO = (double)VISUALIZATION_WIDTH / visualizationArray.size();
					const double HEIGHT_RATIO = (double)VISUALIZATION_HEIGHT / *std::max_element(visualizationArray.begin(), visualizationArray.end());

					SelectObject(hdc, whitePen);

					FillRect(hdc, &fullPaint, solidBrush);

					for (int i = 0; i < visualizationArray.size(); i++)
					{

						Rectangle(hdc, i * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[i] * HEIGHT_RATIO, (i + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);
					} */

			/* for (int i = 0; i < steps.size(); i++)
			{

				std::swap(visualizationArray[steps[i].i], visualizationArray[steps[i].j]);

				RECT recti = {(int)(steps[i].i * WIDTH_RATIO), 0, (int)((steps[i].i + 1) * WIDTH_RATIO), VISUALIZATION_HEIGHT};
				RECT rectj = {(int)(steps[i].j * WIDTH_RATIO), 0, (int)((steps[i].j + 1) * WIDTH_RATIO), VISUALIZATION_HEIGHT};
				SelectObject(hdc, blackPen);
				FillRect(hdc, &recti, solidBrush);
				FillRect(hdc, &rectj, solidBrush);

				double si1 = steps[i].i * WIDTH_RATIO;
				double si2 = VISUALIZATION_HEIGHT - visualizationArray[steps[i].i] * HEIGHT_RATIO;
				double si3 = (steps[i].i + 1) * WIDTH_RATIO;
				double sj1 = steps[i].j * WIDTH_RATIO;
				double sj2 = VISUALIZATION_HEIGHT - visualizationArray[steps[i].j] * HEIGHT_RATIO;
				double sj3 = (steps[i].j + 1) * WIDTH_RATIO;

				SelectObject(hdc, redPen);
				Rectangle(hdc, si1, si2, si3, VISUALIZATION_HEIGHT);
				Rectangle(hdc, sj1, sj2, sj3, VISUALIZATION_HEIGHT);

				std::this_thread::sleep_for(std::chrono::milliseconds(stepsDelay));
				SelectObject(hdc, whitePen);
				Rectangle(hdc, si1, si2, si3, VISUALIZATION_HEIGHT);
				Rectangle(hdc, sj1, sj2, sj3, VISUALIZATION_HEIGHT);
			} */

			/* 			steps.clear();

						shouldVisualize = false; */

			(hdc, si1, si2, si3, VISUALIZATION_HEIGHT);
			Rectangle(hdc, sj1, sj2, sj3, VISUALIZATION_HEIGHT);

			std::this_thread::sleep_for(std::chrono::milliseconds(stepsDelay));
			Rectangle(hdc, si1, si2, si3, VISUALIZATION_HEIGHT);
			Rectangle(hdc, sj1, sj2, sj3, VISUALIZATION_HEIGHT);
		}

		EndPaint(hwnd, &ps);

		break;
	case WM_ERASEBKGND:
		return 1;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

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
	wcex.hbrBackground = NULL;
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
RECT rect1;
RECT rect2;
bool clearVisualization = false;
int visualize()
{
	clearVisualization = true;
	InvalidateRect(w2, NULL, TRUE);
	UpdateWindow(w2);

	const double WIDTH_RATIO = (double)VISUALIZATION_WIDTH / visualizationArray.size();
	const double HEIGHT_RATIO = (double)VISUALIZATION_HEIGHT / *std::max_element(visualizationArray.begin(), visualizationArray.end());
	for (int i = 0; i < steps.size(); i++)
	{
		std::swap(visualizationArray[steps[i].i], visualizationArray[steps[i].j]);
		rect1 = {(int)(steps[0].i * WIDTH_RATIO), 0, (int)((steps[0].i + 1) * WIDTH_RATIO), VISUALIZATION_HEIGHT};
		rect2 = {(int)(steps[0].j * WIDTH_RATIO), 0, (int)((steps[0].j + 1) * WIDTH_RATIO), VISUALIZATION_HEIGHT};
	}
}

/* BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam); */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (message)
	{
	case WM_CREATE:
		CreateWindow("BUTTON", "Load file", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)LOAD_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Save to file", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SAVE_SORTED_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Randomize", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN), SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)GENERATE_RANDOM_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Save random", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN), SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SAVE_BUTTON, NULL, NULL);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_NUMBER, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 2.5, BUTTON_WIDTH, GENERATOR_HEIGHT, hwnd, (HMENU)AMOUNT_TO_GENERATE, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, AMOUNT_TO_GENERATE), EM_SETLIMITTEXT, 9, 0);

		CreateWindow("BUTTON", "Quick sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 3.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)QUICKSORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Bubble Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 3.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)BUBBLE_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Insertion Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 4.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)INSERTION_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Selection Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 4.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SELECTION_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Cocktail Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 5.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)COCKTAIL_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Heap Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 5.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)HEAP_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Comb Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 6.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)COMB_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Shell Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 6.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SHELL_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Cycle Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 7.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)CYCLE_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Gnome Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 7.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)GNOME_SORT_BUTTON, NULL, NULL);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_NUMBER, LEFT_BUTTON_X_POS, (GENERATOR_HEIGHT * 0.33) + (BUTTON_HEIGHT - GENERATOR_HEIGHT) + BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 8.5, SHORT_BUTTON_WIDTH, GENERATOR_HEIGHT * 0.7, hwnd, (HMENU)PASSES_AMOUNT, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, PASSES_AMOUNT), EM_SETLIMITTEXT, 5, 0);
		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_NUMBER, RIGHT_BUTTON_X_POS, (GENERATOR_HEIGHT * 0.33) + (BUTTON_HEIGHT - GENERATOR_HEIGHT) + BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 8.5, SHORT_BUTTON_WIDTH, GENERATOR_HEIGHT * 0.7, hwnd, (HMENU)STEPS_DELAY, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, STEPS_DELAY), EM_SETLIMITTEXT, 3, 0);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE, BUTTON_MARGIN, (BUTTON_HEIGHT + BUTTON_MARGIN) * 9.5 + GENERATOR_HEIGHT, BUTTON_WIDTH, 364, hwnd, (HMENU)STATS_DISPLAY, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), EM_SETREADONLY, TRUE, 0);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
					 BUTTON_MARGIN + BUTTON_WIDTH + BUTTON_MARGIN,
					 VISUALIZATION_HEIGHT + BUTTON_MARGIN * 4,
					 (VISUALIZATION_WIDTH / 2) - BUTTON_MARGIN / 2,
					 224,
					 hwnd, (HMENU)UNSORTED_DISPLAY, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, UNSORTED_DISPLAY), EM_SETREADONLY, TRUE, 0);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
					 BUTTON_MARGIN + BUTTON_WIDTH + BUTTON_MARGIN + ((VISUALIZATION_WIDTH / 2) - BUTTON_MARGIN / 2) + BUTTON_MARGIN,
					 VISUALIZATION_HEIGHT + BUTTON_MARGIN * 4,
					 (VISUALIZATION_WIDTH / 2) - BUTTON_MARGIN / 2,
					 224,
					 hwnd, (HMENU)SORTED_DISPLAY, NULL, NULL);

		SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), EM_SETREADONLY, TRUE, 0);
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = Proc2;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = "test";
		wc.lpszClassName = "window2";
		RegisterClassEx(&wc);

		w2 = CreateWindowEx(WS_EX_STATICEDGE, "window2", (LPCSTR)NULL, WS_CHILD | WS_BORDER | SW_SHOW | WS_VISIBLE | WS_EX_COMPOSITED, BUTTON_MARGIN * 2 + BUTTON_WIDTH, BUTTON_MARGIN, VISUALIZATION_WIDTH, VISUALIZATION_HEIGHT, hwnd, (HMENU)VISUALIZATION, VisualizationInstance, NULL);
		ShowWindow(w2, SW_SHOW);
		UpdateWindow(w2);

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
				break;
			}
			if (LOWORD(wParam) == PASSES_AMOUNT)
			{
				int len = GetWindowTextLength((HWND)lParam);
				char *buffer = new char[len + 1];
				GetWindowText((HWND)lParam, buffer, len + 1);
				std::string s = buffer;
				if (s != "")
				{
					passesAmount = std::stoi(s);
				}
				else
				{
					passesAmount = 1;
				}

				delete[] buffer;
				break;
			}
			if (LOWORD(wParam) == STEPS_DELAY)
			{
				int len = GetWindowTextLength((HWND)lParam);
				char *buffer = new char[len + 1];
				GetWindowText((HWND)lParam, buffer, len + 1);
				std::string s = buffer;
				if (s != "")
				{
					stepsDelay = std::stoi(s);
				}
				else
				{
					stepsDelay = 0;
				}

				delete[] buffer;
				break;
			}
			break;
		}
		}

		switch (LOWORD(wParam))
		{
		case SAVE_SORTED_BUTTON:
		{
			std::wstring temp = Files::selectFile();
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
			std::vector<unsigned int> random;
			for (int i = 0; i < amountToGenerate; i++)
			{
				random.push_back(rand() % 10000);
			}
			fromFile = random;
			SendMessage(GetDlgItem(hwnd, UNSORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(fromFile).c_str());
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case SAVE_BUTTON:
		{
			std::wstring temp = Files::selectFile();
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
			std::wstring temp = Files::selectFile();
			filepath = std::string(temp.begin(), temp.end());
			fromFile = Files::loadFrom(filepath);
			/* 	std::cout << fromFile << std::endl; */
			std::cout << filepath << std::endl;
			SendMessage(GetDlgItem(hwnd, UNSORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(fromFile).c_str());
			InvalidateRect(hwnd, NULL, TRUE);

			break;
		}

		case QUICKSORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::quicksort);
			steps.clear();
			Sorts::quicksort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Quick Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());
			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;

			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case BUBBLE_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::bubbleSort);
			steps.clear();
			Sorts::bubbleSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Bubble Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());
			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case INSERTION_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::insertionSort);
			steps.clear();
			Sorts::insertionSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Insertion Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());
			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case SELECTION_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::selectionSort);
			steps.clear();
			Sorts::selectionSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Selection Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case COCKTAIL_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::cocktailSort);
			steps.clear();
			Sorts::cocktailSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Cocktail Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case HEAP_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::heapSort);
			steps.clear();
			Sorts::heapSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Heap Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case COMB_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::combSort);
			steps.clear();
			Sorts::combSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Comb Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case SHELL_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::shellSort);
			steps.clear();
			Sorts::shellSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Shell Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case CYCLE_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::cycleSort);
			steps.clear();
			Sorts::cycleSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Cycle Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		case GNOME_SORT_BUTTON:
		{
			if (fromFile.size() < 1)
			{
				MessageBox(hwnd, "No data to sort", "Error", MB_OK);
				break;
			}

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;

			Sorts::sort(toBeSorted, passesAmount, &Sorts::gnomeSort);
			steps.clear();
			Sorts::gnomeSort(toBeSorted);

			std::string wsBookmark = "Sorting Algorithm: Gnome Sort\r\nAmount of objects: " + std::to_string(toBeSorted.size()) + "\r\nLowest time: " + std::to_string(fastestSort) + "\r\nHighest time: " + std::to_string(slowestSort) + "\r\nAverage time: " + std::to_string(averageSort) + "\r\nComparisons: " + std::to_string(amountOfComparisons) + "\r\nSwaps: " + std::to_string(steps.size()) + "\r\nIterations: " + std::to_string(amountOfIterations) + "\r\n";

			UINT size = (wsBookmark.size() + 1) * sizeof(wchar_t);

			wchar_t *ptr = (wchar_t *)CoTaskMemAlloc(size);
			CopyMemory(ptr, wsBookmark.c_str(), size);

			SendMessage(GetDlgItem(hwnd, STATS_DISPLAY), WM_SETTEXT, 0, (LPARAM)ptr);
			SendMessage(GetDlgItem(hwnd, SORTED_DISPLAY), WM_SETTEXT, 0, (LPARAM)vectorToString(toBeSorted).c_str());

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			InvalidateRect(w2, NULL, TRUE);

			break;
		}
		break;
		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, BUTTON_MARGIN, BUTTON_MARGIN + 2 * (BUTTON_HEIGHT + BUTTON_MARGIN), "Amount to generate:", 19);
		TextOut(hdc, BUTTON_MARGIN, (BUTTON_HEIGHT + BUTTON_MARGIN) * 9 + GENERATOR_HEIGHT, "Statistics:", 11);
		TextOut(hdc, BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 8.5, "Passes:", 7);
		TextOut(hdc, BUTTON_MARGIN + SHORT_BUTTON_WIDTH + BUTTON_MARGIN, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 8.5, "Step delay:", 11);
		TextOut(hdc, BUTTON_MARGIN + BUTTON_WIDTH + BUTTON_MARGIN, VISUALIZATION_HEIGHT + BUTTON_MARGIN * 4 - 20, "Unsorted:", 9);
		TextOut(hdc, BUTTON_MARGIN + BUTTON_WIDTH + BUTTON_MARGIN + ((VISUALIZATION_WIDTH / 2) - BUTTON_MARGIN / 2) + BUTTON_MARGIN, VISUALIZATION_HEIGHT + BUTTON_MARGIN * 4 - 20, "Sorted:", 7);

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

int main()
{
	WinMain(instance, NULL, 0, SW_SHOW);
}

/*  DISPLAYING BEFORE SORT, AFTER SORT, TIME IT TOOK, NUMBER OF ITERATIONS, NUMBER OF CHANGES */