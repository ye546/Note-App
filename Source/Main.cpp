#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include "resource.h"

//needed for "visual style"
#pragma comment(lib, "Comctl32.lib")			
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

static std::string GetWindowStringtext(HWND hWnd);
static std::string OpenFile(std::string filename);
static bool SaveFile(std::string filename, std::string content);

static HINSTANCE hInst;
static HWND text_window, get_text, open_file;
static int w = 800, h = 600;

static std::string font_name;
static bool underline, italic;
static int thickness;
static long lfHeight;

void GetMonitorResolution(int& horizontal, int& vertical);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Note App";
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&wc);

	//GetMonitorResolution(w, h);

	HWND hwnd = CreateWindowEx(
		0, CLASS_NAME,                     
		L"Note App",   
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL,          
		NULL,      
		hInstance,  
		NULL       
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//define a font
	HFONT hf;
	NONCLIENTMETRICS ncm = { 0 };

	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	hf = CreateFontIndirect(&ncm.lfStatusFont);
	hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hf, TRUE);
	SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	HFONT hf;
	HDC hdc;

	hdc = GetDC(NULL);
	lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);

	int width = 640, height = 480;
	
	//GetMonitorResolution(w, h);

	//width = (w / 2 + 595);
	//height = (h / 2 + 842);

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_CREATE:
		text_window = CreateWindowExA(
			WS_EX_CLIENTEDGE,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			100, 10, width, height,
			hwnd,
			NULL,
			hInst,
			(LPVOID)lParam
		);
		break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
			case ID_ARIAL_ITALIC:
				font_name = "Arial";
				italic = true;
				thickness = 0;
				underline = false;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_ARIAL_NORMAL:
				font_name = "Arial";
				italic = false;
				underline = false;
				thickness = 0;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_ARIAL_BOLD:
				font_name = "Arial";

				thickness = FW_BOLD;
				underline = false;
				italic = false;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_ARIAL_UNDERLINE:
				font_name = "Arial";

				underline = true;
				thickness = 0;
				italic = false;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_TIMESNEWROMAN_ITALIC:
				font_name = "Times New Roman";

				italic = true;
				underline = false;
				thickness = 0;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_TIMESNEWROMAN_NORMAL:
				font_name = "Times New Roman";

				underline = false;
				italic = false;
				thickness = 0;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_TIMESNEWROMAN_BOLD:
				font_name = "Times New Roman";

				underline = false;
				italic = false;
				thickness = FW_BOLD;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_TIMESNEWROMAN_UNDERLINE:
				font_name = "Times New Roman";

				underline = true;
				italic = false;
				thickness = 0;

				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_TEXTSIZE_8:
				lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_9:
				lfHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_10:
				lfHeight = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_11:
				lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_12:
				lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_14:
				lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_16:
				lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_18:
				lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_20:
				lfHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_24:
				lfHeight = -MulDiv(24, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_26:
				lfHeight = -MulDiv(26, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_28:
				lfHeight = -MulDiv(28, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_36:
				lfHeight = -MulDiv(36, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_48:
				lfHeight = -MulDiv(48, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;
			case ID_TEXTSIZE_72:
				lfHeight = -MulDiv(72, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hf = CreateFontA(lfHeight, 0, 0, 0, thickness, italic, underline, 0, 0, 0, 0, 0, 0, font_name.c_str());
				SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
				break;

			case ID_FILE_OPEN:
			{
				//this is needed to call GetSaveFileName
				OPENFILENAMEA ofn;
				char szFileName[MAX_PATH];

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFilter = (LPCSTR)"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = (LPSTR)szFileName;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = MAX_PATH;
				ofn.nFilterIndex = 1;

				GetOpenFileNameA(&ofn);

				std::string text = OpenFile(ofn.lpstrFile);

				if (SetWindowTextA(text_window, text.c_str())) {
					//MessageBoxA(NULL, "Successfully loaded file!", "Success", MB_OK);
					//bravo...
				}
			}
			break;
			case ID_FILE_SAVE:
			{
				//this is needed to call GetSaveFileName
				OPENFILENAMEA ofn;
				char szFileName[MAX_PATH] = "Namnlös.txt";

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFilter = (LPCSTR)"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = (LPSTR)szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = (LPCSTR)"txt";

				GetSaveFileNameA(&ofn);

				std::string tmp = GetWindowStringtext(text_window);
				if (SaveFile(ofn.lpstrFile, tmp)) {
					std::string x = " saved!";
					tmp = ofn.lpstrFile + x;
					MessageBoxA(NULL, tmp.c_str(), "Success", MB_ICONINFORMATION);
				}
				else {
					std::string x = "Failed to save ";
					tmp = x + ofn.lpstrFile;
					MessageBoxA(NULL, tmp.c_str(), "Error", MB_ICONERROR);
				}
			}
			break;
			default:
				break;
		}

		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hwnd, &ps);
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


std::string GetWindowStringtext(HWND hWnd) {
	int len = GetWindowTextLength(hWnd) + 1; //+1 for trailing backslash
	std::vector<char> buffer(len);
	GetWindowTextA(hWnd, &buffer[0], buffer.size());
	std::string text = &buffer[0];
	std::string return_string(text.begin(), text.end());
	return return_string;
}

std::string OpenFile(std::string filename) {
	std::ifstream file(filename);
	std::string tmp;

	std::vector<std::string>* buffer = new std::vector<std::string>;

	if (file.is_open()) {
		while (std::getline(file, tmp))
			buffer->push_back(tmp);
	}
	
	std::string return_string;
	for (auto& i : *buffer)
		return_string += i;

	delete buffer;
	buffer = nullptr;

	return return_string;
}

bool SaveFile(std::string filename, std::string content) {
	std::ofstream file(filename, std::ofstream::app);
	if (file << content)
		return true;

	return false;
}

void GetMonitorResolution(int& horizontal, int& vertical) {
	RECT window_rect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &window_rect);
	
	horizontal = window_rect.right;
	vertical = window_rect.bottom;
}
