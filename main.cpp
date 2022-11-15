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
#include <algorithm>
struct Step
{
	int i;
	int j;
};

std::vector<Step> steps;
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
#define MERGE_SORT_BUTTON 14
#define HEAP_SORT_BUTTON 15
#define COMB_SORT_BUTTON 16
#define SHELL_SORT_BUTTON 17
#define CYCLE_SORT_BUTTON 18
#define GNOME_SORT_BUTTON 19

#define TIME_BETWEEN_STEPS 10
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

// The main window class name.
static char WINDOW_NAME[] = _T("SortTest");
static char WINDOW_TITLE[] = _T("Sorting algorithms speed test");

HINSTANCE instance;
HINSTANCE VisualizationInstance;
HWND w2;
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

		CreateWindow("BUTTON", "Merge Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 5.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)MERGE_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Heap Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 5.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)HEAP_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Comb Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 6.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)COMB_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Shell Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 6.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)SHELL_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Cycle Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 7.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)CYCLE_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "Gnome Sort", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, RIGHT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 7.5, SHORT_BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)GNOME_SORT_BUTTON, NULL, NULL);

		CreateWindow("BUTTON", "FASTEST SORT", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, LEFT_BUTTON_X_POS, BUTTON_MARGIN + (BUTTON_HEIGHT + BUTTON_MARGIN) * 8.5, BUTTON_WIDTH, BUTTON_HEIGHT, hwnd, (HMENU)GNOME_SORT_BUTTON, NULL, NULL);

		CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | EM_SETREADONLY, BUTTON_MARGIN, (BUTTON_HEIGHT + BUTTON_MARGIN) * 9.5 + GENERATOR_HEIGHT, BUTTON_WIDTH, 364, hwnd, (HMENU)STATS_DISPLAY, NULL, NULL);

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

			steps.clear();

			toBeSorted = fromFile;
			visualizationArray = toBeSorted;
			/* 		auto start = std::chrono::steady_clock::now();
					Sorts::quicksort(toBeSorted);

					auto finish = std::chrono::steady_clock::now();

					std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms" << std::endl; */
			Sorts::sort(toBeSorted, 100, &Sorts::quicksort);
			steps.clear();
			Sorts::quicksort(toBeSorted);

			InvalidateRect(hwnd, NULL, TRUE);
			shouldVisualize = true;
			std::cout << "pre-visualization" << std::endl;
			/* kek = std::async(visualize); */
			InvalidateRect(w2, NULL, TRUE);
			break;
		}
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, BUTTON_MARGIN, BUTTON_MARGIN + 2 * (BUTTON_HEIGHT + BUTTON_MARGIN), "Amount to generate:", 19);
		TextOut(hdc, BUTTON_MARGIN, (BUTTON_HEIGHT + BUTTON_MARGIN) * 9 + GENERATOR_HEIGHT, "Statistics:", 11);

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
			std::cout << "VISUALIZING..." << std::endl;
			const double WIDTH_RATIO = (double)VISUALIZATION_WIDTH / visualizationArray.size();
			const double HEIGHT_RATIO = (double)VISUALIZATION_HEIGHT / *std::max_element(visualizationArray.begin(), visualizationArray.end());

			SelectObject(hdc, whitePen);

			FillRect(hdc, &fullPaint, solidBrush);

			for (int i = 0; i < visualizationArray.size(); i++)
			{

				Rectangle(hdc, i * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[i] * HEIGHT_RATIO, (i + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);
			}

			for (int i = 0; i < steps.size(); i++)
			{

				unsigned int temp = visualizationArray[steps[i].i];
				visualizationArray[steps[i].i] = visualizationArray[steps[i].j];
				visualizationArray[steps[i].j] = temp;

				RECT recti = {steps[i].i * WIDTH_RATIO, 0, (steps[i].i + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT};
				RECT rectj = {steps[i].j * WIDTH_RATIO, 0, (steps[i].j + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT};
				SelectObject(hdc, blackPen);
				FillRect(hdc, &recti, solidBrush);
				FillRect(hdc, &rectj, solidBrush);

				SelectObject(hdc, redPen);
				Rectangle(hdc, steps[i].i * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[steps[i].i] * HEIGHT_RATIO, (steps[i].i + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);
				Rectangle(hdc, steps[i].j * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[steps[i].j] * HEIGHT_RATIO, (steps[i].j + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);

				/* std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_STEPS)); */
				SelectObject(hdc, whitePen);
				Rectangle(hdc, steps[i].i * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[steps[i].i] * HEIGHT_RATIO, (steps[i].i + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);
				Rectangle(hdc, steps[i].j * WIDTH_RATIO, VISUALIZATION_HEIGHT - visualizationArray[steps[i].j] * HEIGHT_RATIO, (steps[i].j + 1) * WIDTH_RATIO, VISUALIZATION_HEIGHT);
			}

			shouldVisualize = false;
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

int main()
{
	WinMain(instance, NULL, 0, SW_SHOW);
}

/*  DISPLAYING BEFORE SORT, AFTER SORT, TIME IT TOOK, NUMBER OF ITERATIONS, NUMBER OF CHANGES */