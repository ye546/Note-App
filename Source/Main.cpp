#ifndef UNICODE
#define UNICODE
#endif 

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Comctl32.lib")			//needed for "visual style"

#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include "resource.h"


static std::string GetWindowStringtext(HWND hWnd);
static std::string OpenFile(std::string filename);
static bool SaveFile(std::string filename, std::string content);
void GetMonitorResolution(int& horizontal, int& vertical);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HINSTANCE hInst;
static HWND text_window, get_text, open_file;
static int w, h;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Note App";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&wc);

	GetMonitorResolution(w, h);

	HWND hwnd = CreateWindowEx(
		0,                             
		CLASS_NAME,                     
		L"Note App",    
		WS_OVERLAPPEDWINDOW | CDS_FULLSCREEN | WS_VSCROLL,           

		// Size and position
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

	// Run the message loop.

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
	long lfHeight;
	HDC hdc;

	hdc = GetDC(NULL);
	lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);

	int width, height;
	
	GetMonitorResolution(w, h);

	width = (w / 2 + 595);
	height = (h / 2 + 842);

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
			WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT,
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
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_ARIAL_NORMAL:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_ARIAL_BOLD:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_ARIAL_UNDERLINE:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, 0, 0, 0, 0, 0, "Arial");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_TIMESNEWROMAN_ITALIC:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_TIMESNEWROMAN_NORMAL:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, FALSE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_TIMESNEWROMAN_BOLD:
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
			SendMessage(text_window, WM_SETFONT, (WPARAM)hf, TRUE);
			break;

		case ID_TIMESNEWROMAN_UNDERLINE:;
			hf = CreateFontA(lfHeight, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, 0, 0, 0, 0, 0, "Times New Roman");
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
