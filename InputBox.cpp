#include "stdafx.h"
#include "InputBox.h"
#include "UTL_Conversion.h"

HFONT	InputBox::mhFont		= nullptr;
HWND	InputBox::mhWndParent	= nullptr;
HWND	InputBox::mhWndPrompt	= nullptr;
HWND	InputBox::mhWndInputBox	= nullptr;
HWND	InputBox::mhWndEdit		= nullptr;
HWND	InputBox::mhWndOK		= nullptr;
HWND	InputBox::mhWndCancel	= nullptr;
HBRUSH	InputBox::mhbrBkgnd		= nullptr;

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

void InputBox::SetTextAlignment(HWND _hwnd, int _textAlignment)
{
	LONG_PTR tmp;
	if (GetWindowLongPtr(_hwnd, GWL_STYLE) != _textAlignment) {
		//clean up
		if (_textAlignment == 0) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_LEFT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 1) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_CENTER);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 2) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp & ~(SS_RIGHT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}

		//set new text alignment
		if (_textAlignment == 0) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_LEFT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 1) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_CENTER);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		} else if (_textAlignment == 2) {
			tmp = GetWindowLongPtr(_hwnd, GWL_STYLE);
			tmp = tmp | (SS_RIGHT);
			SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)tmp);
		}
		SetWindowPos(_hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_DRAWFRAME);
	}
}

LRESULT CALLBACK InputBox::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	LOGFONT lfont;
	HINSTANCE hInst = nullptr;

	switch (_message) {
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
		{
			int r = 0, g = 0, b = 0;
			if (brush.first) {
				if (mhbrBkgnd == nullptr) {
					UTL_Conversion::HexToRGB(brush.second, r, g, b);
					mhbrBkgnd = CreateSolidBrush(RGB(r, g, b));
				}
			}

			HDC hdcStatic = (HDC)_wParam;
			if (pen.first) {
				UTL_Conversion::HexToRGB(pen.second, r, g, b);
				SetTextColor(hdcStatic, RGB(r, g, b));
			}

			if (background.first) {
				UTL_Conversion::HexToRGB(background.second, r, g, b);
				SetBkColor(hdcStatic, RGB(r, g, b));
			}

			return (INT_PTR)mhbrBkgnd;
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
			mhFont = CreateFontIndirect(&lfont);

			hInst = GetModuleHandle(nullptr);

			if (InputBox::password)
				mhWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD , 5, 10 + fontSize / 2 + fontSize * linesOfText, InputBox::width - 30, fontSize + 2, _hWnd, nullptr, hInst, nullptr);
			else
				mhWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL/*ES_PASSWORD*/, 5, 10 + fontSize / 2 + fontSize * linesOfText, InputBox::width - 30, fontSize + 2, _hWnd, nullptr, hInst, nullptr);

			if (mhWndEdit == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((mhWndEdit), WM_SETFONT, (WPARAM)mhFont, 0);

			mhWndOK = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Ok"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, InputBox::width - 25 - (fontSize * 120 / 22) * 2 - 20, 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15, fontSize * 120 / 22, fontSize + 8, _hWnd, nullptr, hInst, nullptr);
			if (mhWndOK == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((mhWndOK), WM_SETFONT, (WPARAM)mhFont, 0);

			mhWndCancel = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, InputBox::width - 25 - fontSize * 120 / 22, 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15, fontSize * 120 / 22, fontSize + 8, _hWnd, nullptr, hInst, nullptr);
			if (mhWndCancel == nullptr) {
				return (LRESULT)nullptr;
			}
			SendMessage((mhWndCancel), WM_SETFONT, (WPARAM)mhFont, 0);

			mhWndPrompt = CreateWindowEx(WS_EX_STATICEDGE, _T("static"), _T(""), WS_VISIBLE | WS_CHILD, 5, 10, InputBox::width - 30, (fontSize + 2) * linesOfText, _hWnd, nullptr, hInst, nullptr);
			if (mhWndPrompt == nullptr) {
				return (LRESULT)nullptr;
			}

			SendMessage((mhWndPrompt), WM_SETFONT, (WPARAM)mhFont, 0);
			SetFocus(mhWndEdit);
			break;
		}
		case WM_DESTROY: {
			DeleteObject(mhFont);
			EnableWindow(mhWndParent, TRUE);
			SetForegroundWindow(mhWndParent);
			DestroyWindow(_hWnd);
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch (HIWORD(_wParam)) {
				case BN_CLICKED:
					if ((HWND)_lParam == mhWndOK)
						PostMessage(mhWndInputBox, WM_KEYDOWN, VK_RETURN, 0);
					if ((HWND)_lParam == mhWndCancel)
						PostMessage(mhWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
					break;
			}
			break;
		}
		default: {
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	return 0;
}

bool InputBox::GetString(wstring & _result)
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

	mhWndParent = hWnd;
	mhWndInputBox = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("InputBox"), title.c_str(), 
		WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP | WS_VISIBLE, 
		(rc.right - InputBox::width) / 2, (rc.bottom - InputBox::width / 2) / 2, 
		InputBox::width, 50 + 10 + fontSize / 2 + fontSize * linesOfText + (fontSize + 2) + 15 + (fontSize + 8),
		mhWndParent, nullptr, nullptr, nullptr
	);
	if (mhWndInputBox == nullptr) {
		return false;
	}

	SetTextAlignment(mhWndPrompt, SS_LEFT);
	SetWindowText(mhWndPrompt, prompt.c_str());
	SetTextAlignment(mhWndEdit, SS_LEFT);
	SetForegroundWindow(mhWndInputBox);

	SendMessage((HWND)mhWndOK, BM_SETSTYLE, (WPARAM)LOWORD(BS_DEFPUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)mhWndCancel, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SendMessage(mhWndEdit, EM_REPLACESEL, 0, (LPARAM)def.c_str());
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SetFocus(mhWndEdit);

	EnableWindow(mhWndParent, FALSE);
	ShowWindow(mhWndInputBox, SW_SHOW);
	UpdateWindow(mhWndInputBox);

	BOOL ret = false;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_ESCAPE) {
				SendMessage(mhWndInputBox, WM_DESTROY, 0, 0);
				ret = false;
			} else if (msg.wParam == VK_RETURN) {
				int nCount = GetWindowTextLengthW(mhWndEdit) + 1;

				vector<wchar_t> buff(nCount);
				GetWindowTextW(mhWndEdit, &buff[0], nCount);
				_result = &buff[0];

				SendMessage(mhWndInputBox, WM_DESTROY, 0, 0);
				ret = true;
			} else if (msg.wParam == VK_TAB) {
				HWND hWndFocused = GetFocus();
				if (hWndFocused == mhWndEdit) SetFocus(mhWndOK);
				if (hWndFocused == mhWndOK) SetFocus(mhWndCancel);
				if (hWndFocused == mhWndCancel) SetFocus(mhWndEdit);
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ret;
}

