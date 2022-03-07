#include "stdafx.h"
#include "InputBox.h"
#include "UTL_Conversion.h"

HFONT	InputBox::hFont			= nullptr;
HWND	InputBox::hWndParent	= nullptr;
HWND	InputBox::hWndPrompt	= nullptr;
HWND	InputBox::hWndInputBox	= nullptr;
HWND	InputBox::hWndEdit		= nullptr;
HWND	InputBox::hWndOK		= nullptr;
HWND	InputBox::hWndCancel	= nullptr;
HBRUSH	InputBox::hbrBkgnd		= nullptr;

int		InputBox::width					= 600;
int		InputBox::fontSize				= 22;
int		InputBox::linesOfText			= 1;
bool	InputBox::password				= false;
wstring	InputBox::fontName				= _T("Consolas");

wstring InputBox::title					= _T("Input Box");
wstring InputBox::prompt				= _T("Please input text");
wstring InputBox::def					= _T("");

pair<bool, wstring>	InputBox::brush		= pair<bool, wstring>(false, _T("#000000"));
pair<bool, wstring> InputBox::background= pair<bool, wstring>(false, _T("#000000"));
pair<bool, wstring> InputBox::pen		= pair<bool, wstring>(false, _T("#ffffff"));

void InputBox::SetTextAlignment(HWND hwnd, int textAlignment)
{
	LONG_PTR tmp;
	if (GetWindowLongPtr(hwnd, GWL_STYLE) != textAlignment) {
		//clean up
		if (textAlignment == 0) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_LEFT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (textAlignment == 1) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_CENTER);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (textAlignment == 2) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_RIGHT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}

		//set new text alignment
		if (textAlignment == 0) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp | (SS_LEFT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (textAlignment == 1) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp | (SS_CENTER);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (textAlignment == 2) {
			tmp = GetWindowLongPtr(hwnd, GWL_STYLE);
			tmp = tmp | (SS_RIGHT);
			SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}
		SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_DRAWFRAME);
	}
}

LRESULT CALLBACK InputBox::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LOGFONT lfont;
	HINSTANCE hInst = nullptr;

	switch (message) {
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
		{
			int r = 0, g = 0, b = 0;
			if (brush.first) {
				if (hbrBkgnd == nullptr) {
					UTL_Conversion::HexToRGB(brush.second, r, g, b);
					hbrBkgnd = CreateSolidBrush(RGB(r, g, b));
				}
			}

			HDC hdcStatic = (HDC)wParam;
			if (pen.first) {
				UTL_Conversion::HexToRGB(pen.second, r, g, b);
				SetTextColor(hdcStatic, RGB(r, g, b));
			}

			if (background.first) {
				UTL_Conversion::HexToRGB(background.second, r, g, b);
				SetBkColor(hdcStatic, RGB(r, g, b));
			}

			return (INT_PTR)hbrBkgnd;
			break;
		}
		case WM_CREATE: {
			memset(&lfont, 0, sizeof(lfont));
			lstrcpy(lfont.lfFaceName, (LPWSTR)InputBox::fontName.c_str());
			lfont.lfHeight			= fontSize;
			lfont.lfWeight			= FW_NORMAL;//FW_BOLD;
			lfont.lfItalic			= FALSE;
			lfont.lfCharSet			= DEFAULT_CHARSET;
			lfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
			lfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
			lfont.lfQuality			= DEFAULT_QUALITY;
			lfont.lfPitchAndFamily	= DEFAULT_PITCH;
			hFont = CreateFontIndirect(&lfont);

			hInst = GetModuleHandle(nullptr);

			if (InputBox::password)
				hWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD , 5, 10 + fontSize / 2 + fontSize * linesOfText, InputBox::width - 30, fontSize + 2, hWnd, nullptr, hInst, nullptr);
			else
				hWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL/*ES_PASSWORD*/, 5, 10 + fontSize / 2 + fontSize * linesOfText, InputBox::width - 30, fontSize + 2, hWnd, nullptr, hInst, nullptr);

			if (hWndEdit == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((hWndEdit), WM_SETFONT, (WPARAM)hFont, 0);

			hWndOK = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Ok"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, InputBox::width - 25 - (fontSize * 120 / 22) * 2 - 20, 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15, fontSize * 120 / 22, fontSize + 8, hWnd, nullptr, hInst, nullptr);
			if (hWndOK == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((hWndOK), WM_SETFONT, (WPARAM)hFont, 0);

			hWndCancel = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, InputBox::width - 25 - fontSize * 120 / 22, 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15, fontSize * 120 / 22, fontSize + 8, hWnd, nullptr, hInst, nullptr);
			if (hWndCancel == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((hWndCancel), WM_SETFONT, (WPARAM)hFont, 0);

			hWndPrompt = CreateWindowEx(WS_EX_STATICEDGE, _T("static"), _T(""), WS_VISIBLE | WS_CHILD, 5, 10, InputBox::width - 30, (fontSize + 2) * linesOfText, hWnd, nullptr, hInst, nullptr);
			if (hWndPrompt == nullptr) {
				return (LRESULT)nullptr;
			}

			SendMessage((hWndPrompt), WM_SETFONT, (WPARAM)hFont, 0);
			SetFocus(hWndEdit);
			break;
		}
		case WM_DESTROY: {
			DeleteObject(hFont);
			EnableWindow(hWndParent, TRUE);
			SetForegroundWindow(hWndParent);
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch (HIWORD(wParam)) {
				case BN_CLICKED:
					if ((HWND)lParam == hWndOK)
						PostMessage(hWndInputBox, WM_KEYDOWN, VK_RETURN, 0);
					if ((HWND)lParam == hWndCancel)
						PostMessage(hWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
					break;
			}
			break;
		}
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

bool InputBox::GetString(wstring &result)
{
	HWND hWnd = GetDesktopWindow();
	RECT rc;
	GetWindowRect(hWnd, &rc);

	HINSTANCE hInst = GetModuleHandle(nullptr);
	WNDCLASSEX wcex;
	if (!GetClassInfoEx(hInst, _T("InputBox"), &wcex)) {
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = nullptr;
		wcex.hCursor = nullptr;
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = _T("InputBox");
		wcex.hIconSm = nullptr;

		if (RegisterClassEx(&wcex) == 0) {
			_tprintf(_T("Error - register class\n"));
			return false;
		}
	}

	hWndParent = hWnd;
	hWndInputBox = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("InputBox"), title.c_str(), 
		WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP | WS_VISIBLE, 
		(rc.right - InputBox::width) / 2, (rc.bottom - InputBox::width / 2) / 2, 
		InputBox::width, 50 + 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15 + (fontSize + 8),
		hWndParent, nullptr, nullptr, nullptr
	);
	if (hWndInputBox == nullptr) {
		return false;
	}

	SetTextAlignment(hWndPrompt, SS_LEFT);
	SetWindowText(hWndPrompt, prompt.c_str());
	SetTextAlignment(hWndEdit, SS_LEFT);
	SetForegroundWindow(hWndInputBox);

	SendMessage((HWND)hWndOK, BM_SETSTYLE, (WPARAM)LOWORD(BS_DEFPUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)hWndCancel, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage(hWndEdit, EM_SETSEL, 0, -1);
	SendMessage(hWndEdit, EM_REPLACESEL, 0, (LPARAM)def.c_str());
	SendMessage(hWndEdit, EM_SETSEL, 0, -1);
	SetFocus(hWndEdit);

	EnableWindow(hWndParent, FALSE);
	ShowWindow(hWndInputBox, SW_SHOW);
	UpdateWindow(hWndInputBox);

	BOOL ret = false;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_ESCAPE) {
				SendMessage(hWndInputBox, WM_DESTROY, 0, 0);
				ret = false;
			} else if (msg.wParam == VK_RETURN) {
				int nCount = GetWindowTextLengthW(hWndEdit) + 1;

				vector<wchar_t> buff(nCount);
				GetWindowTextW(hWndEdit, &buff[0], nCount);
				result = &buff[0];

				SendMessage(hWndInputBox, WM_DESTROY, 0, 0);
				ret = true;
			} else if (msg.wParam == VK_TAB) {
				HWND hWndFocused = GetFocus();
				if (hWndFocused == hWndEdit) SetFocus(hWndOK);
				if (hWndFocused == hWndOK) SetFocus(hWndCancel);
				if (hWndFocused == hWndCancel) SetFocus(hWndEdit);
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ret;
}

