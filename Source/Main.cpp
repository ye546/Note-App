#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include "resource.h"

static std::string GetWindowStringtext(HWND hWnd);
static std::string OpenFile(std::string filename);
static bool SaveFile(std::string filename, std::string content);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HINSTANCE hInst;
static HWND text_window, get_text, open_file;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Note App";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&wc);
	
	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Note App",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

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
			WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE,
			10, 10, 300, 500,
			hwnd,
			NULL,
			hInst,
			(LPVOID)lParam
		);
		break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
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
					MessageBoxA(NULL, "Successfully loaded file!", "Success", MB_OK);
				}
			}
			break;
			case ID_FILE_SAVEAS:
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
			case ID_FILE_SAVE:

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

	if (file.is_open()) {
		std::getline(file, tmp);
	}

	return tmp;
}

bool SaveFile(std::string filename, std::string content) {
	std::ofstream file(filename);
	if (file << content)
		return true;

	return false;
}
