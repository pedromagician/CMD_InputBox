#include "stdafx.h"
#include "InputBox.h"
#include "Conversion.h"
#include "Monitors.h"

static HFONT mhFont;
static HWND  mhWndParent;
static HWND  mhWndPrompt;
static HWND  mhWndInputBox;
static HWND  mhWndEdit;
static HWND  mhWndOK;
static HWND  mhWndCancel;
static HBRUSH mhbrBkgnd;

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
			if (Brush().first) {
				if (mhbrBkgnd == nullptr) {
					Conversion::HexToRGB(Brush().second, r, g, b);
					mhbrBkgnd = CreateSolidBrush(RGB(r, g, b));
				}
			}

			HDC hdcStatic = (HDC)_wParam;
			if (Pen().first) {
				Conversion::HexToRGB(Pen().second, r, g, b);
				SetTextColor(hdcStatic, RGB(r, g, b));
			}

			if (Background().first) {
				Conversion::HexToRGB(Background().second, r, g, b);
				SetBkColor(hdcStatic, RGB(r, g, b));
			}

			return (INT_PTR)mhbrBkgnd;
			break;
		}
		case WM_CREATE: {
			memset(&lfont, 0, sizeof(lfont));
			lstrcpy(lfont.lfFaceName, (LPWSTR)InputBox::FontName().c_str());
			lfont.lfHeight			= FontSize();
			lfont.lfWeight			= FW_NORMAL;//FW_BOLD;
			lfont.lfItalic			= FALSE;
			lfont.lfCharSet			= DEFAULT_CHARSET;
			lfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
			lfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
			lfont.lfQuality			= DEFAULT_QUALITY;
			lfont.lfPitchAndFamily	= DEFAULT_PITCH;
			mhFont = CreateFontIndirect(&lfont);

			hInst = GetModuleHandle(nullptr);

			// input
			int inputX = 5;
			int inputY = 10 + FontSize() / 2 + FontSize() * LinesOfText();
			int inputWidth = InputBox::Width() - 30;
			int inputHeight = FontSize() + 2;

			if (InputBox::Password())	mhWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD , inputX, inputY, inputWidth, inputHeight, _hWnd, nullptr, hInst, nullptr);
			else					mhWndEdit = CreateWindowEx(WS_EX_STATICEDGE, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL/*ES_PASSWORD*/, inputX, inputY, inputWidth, inputHeight, _hWnd, nullptr, hInst, nullptr);
			if (mhWndEdit == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndEdit), WM_SETFONT, (WPARAM)mhFont, 0);

			// buttons
			int buttonWidth = FontSize() * 120 / 22;
			int buttonHeight = FontSize() + 8;
			int buttonX = InputBox::Width() - 25 - buttonWidth * 2 - 10 - 20;
			int buttonY = inputY + inputHeight + 15;

			mhWndOK = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Ok"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, buttonX, buttonY, buttonWidth, buttonHeight, _hWnd, nullptr, hInst, nullptr);
			if (mhWndOK == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndOK), WM_SETFONT, (WPARAM)mhFont, 0);

			buttonX = InputBox::Width() - 25 - buttonWidth * 1 - 10;

			mhWndCancel = CreateWindowEx(WS_EX_STATICEDGE, _T("Button"), _T("Cancel"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, buttonX, buttonY, buttonWidth, buttonHeight, _hWnd, nullptr, hInst, nullptr);
			if (mhWndCancel == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndCancel), WM_SETFONT, (WPARAM)mhFont, 0);

			// message
			int messageX = 5;
			int messageY = 10;
			mhWndPrompt = CreateWindowEx(NULL, _T("static"), _T(""), WS_VISIBLE | WS_CHILD, messageX, messageY, inputWidth, FontSize() * LinesOfText(), _hWnd, nullptr, hInst, nullptr);
			if (mhWndPrompt == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndPrompt), WM_SETFONT, (WPARAM)mhFont, 0);

			SetFocus(mhWndEdit);

			// position
			RECT dialogRect;
			GetWindowRect(_hWnd, &dialogRect);

			bool monitor = true;
			RECT monitorSize = { 0 };

			switch (InputBox::Position().monitor) {
			case _PRIMARY:
				monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
				break;
			case _MOUSE:
				monitor = Monitors::GetMonitorInfoMouse(monitorSize);
				break;
			case _MOUSE_POINTER:
				monitor = Monitors::GetMonitorInfoMouse(monitorSize);
				InputBox::Position().type = _POINTER;
				break;
			case _ID:
				monitor = Monitors::GetMonitorInfoId(InputBox::Position().id, monitorSize);
				break;
			}

			if (InputBox::Position().type == _POINTER) {
				RECT mouseMonitorSize = { 0 };
				monitor = Monitors::GetMonitorInfoMouse(mouseMonitorSize);
				if (EqualRect(&mouseMonitorSize, &monitorSize) == false) {
					InputBox::Position().type = _CENTER;
				}
			}

			long x = 0;
			long y = 0;
			if (monitor) {
				switch (InputBox::Position().type) {
				case _CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _XY: {
					x = monitorSize.left;
					y = monitorSize.top;
					break;
				}
				case _TOP_CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = monitorSize.top;
					break;
				}
				case _BOTTOM_CENTER: {
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = monitorSize.bottom - GetHeight(dialogRect);
					break;
				}
				case _LEFT_CENTER: {
					x = monitorSize.left;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _RIGHT_CENTER: {
					x = monitorSize.right - GetWidth(dialogRect);
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
				case _POINTER: {
					POINT mouse;
					::GetCursorPos(&mouse);

					x = mouse.x - GetWidth(dialogRect) / 2;
					y = mouse.y - GetHeight(dialogRect) / 2;

					break;
				}
				default:
					wcout << _T("Error - unknown position: ") << to_wstring(InputBox::Position().type) << endl;
					InputBox::Position().type = _CENTER;
					monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
					x = GetDiameterX(monitorSize) - GetWidth(dialogRect) / 2;
					y = GetDiameterY(monitorSize) - GetHeight(dialogRect) / 2;
					break;
				}
			}
			else {
				wcout << _T("Error - problem loading information from the monitor") << endl;
				InputBox::Position().monitor = _PRIMARY;
				InputBox::Position().type = _XY;
				x = monitorSize.left;
				y = monitorSize.top;
			}

			x += InputBox::Position().delta.x;
			y += InputBox::Position().delta.y;


			UINT flags = SWP_NOSIZE | SWP_SHOWWINDOW;
			if (InputBox::TopMost() == false)
				flags |= SWP_NOZORDER;

			SetWindowPos(_hWnd, HWND_TOPMOST, x, y, 0, 0, flags);

			// icon app
			if (InputBox::IconApp().empty() == false) {
				HICON hIcon = nullptr;
				hIcon = (HICON)LoadImage(hInst, InputBox::IconApp().c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
				if (!hIcon) {
					HBITMAP bitmapForIconApp = nullptr;
					bitmapForIconApp = (HBITMAP)LoadImage(hInst, InputBox::IconApp().c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
					if (bitmapForIconApp) {
						ICONINFO ii = { 0 };
						ii.fIcon = TRUE;
						ii.hbmColor = bitmapForIconApp;
						ii.hbmMask = bitmapForIconApp;

						hIcon = ::CreateIconIndirect(&ii);

						::DeleteObject(bitmapForIconApp);
					}
				}

				if (hIcon) {
					SendMessage(_hWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
					SendMessage(_hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
				}
			}

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
	mhFont			= nullptr;
	mhWndParent		= nullptr;
	mhWndPrompt		= nullptr;
	mhWndInputBox	= nullptr;
	mhWndEdit		= nullptr;
	mhWndOK			= nullptr;
	mhWndCancel		= nullptr;
	mhbrBkgnd		= nullptr;

	mhWndParent = GetActiveWindow();
	if (!mhWndParent)
		mhWndParent = GetForegroundWindow();
	if (!mhWndParent)
		mhWndParent = GetDesktopWindow();

	RECT rc;
	GetWindowRect(mhWndParent, &rc);

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
			wcout << _T("Error - register class") << endl;
			return false;
		}
	}

	// window
	int inputY = 10 + FontSize() / 2 + FontSize() * LinesOfText();
	int inputHeight = FontSize() + 2;
	int buttonY = inputY + inputHeight + 15;
	int buttonHeight = FontSize() + 8;

	HWND parent = nullptr;
	if (InputBox::BlockParent())
		parent = mhWndParent;

	DWORD dwStyle = WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP | WS_VISIBLE;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;
	dwStyle = dwStyle & ~WS_MINIMIZEBOX;

	mhWndInputBox = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("InputBox"), InputBox::Title().c_str(), dwStyle,
		(rc.right - InputBox::Width()) / 2, (rc.bottom - InputBox::Width() / 2) / 2,
		InputBox::Width(), 50 + buttonY + buttonHeight,
		parent, nullptr, nullptr, nullptr
	);
	if (mhWndInputBox == nullptr) {
		return false;
	}

	HMENU pSysMenu = GetSystemMenu(mhWndInputBox, FALSE);
	if (pSysMenu) {
		RemoveMenu(pSysMenu, SC_RESTORE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_SIZE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_MINIMIZE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}

	SetTextAlignment(mhWndPrompt, SS_LEFT);
	SetWindowText(mhWndPrompt, Prompt().c_str());
	SetTextAlignment(mhWndEdit, SS_LEFT);
	SetForegroundWindow(mhWndInputBox);

	SendMessage((HWND)mhWndOK, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)mhWndCancel, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SendMessage(mhWndEdit, EM_REPLACESEL, 0, (LPARAM)Def().c_str());
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SetFocus(mhWndEdit);

	//EnableWindow(mhWndParent, FALSE);
	ShowWindow(mhWndInputBox, SW_SHOW);
	UpdateWindow(mhWndInputBox);

	BOOL returnCode = false;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_ESCAPE) {
				SendMessage(mhWndInputBox, WM_DESTROY, 0, 0);
				returnCode = false;
			} else if (msg.wParam == VK_RETURN) {
				int nCount = GetWindowTextLengthW(mhWndEdit) + 1;

				vector<wchar_t> buff(nCount);
				GetWindowTextW(mhWndEdit, &buff[0], nCount);
				_result = &buff[0];

				SendMessage(mhWndInputBox, WM_DESTROY, 0, 0);
				returnCode = true;
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
	return returnCode;
}

long InputBox::GetDiameterX(RECT _rect)
{
	return _rect.left + (_rect.right - _rect.left) / 2;
}

long InputBox::GetDiameterY(RECT _rect)
{
	return _rect.top + (_rect.bottom - _rect.top) / 2;
}

long InputBox::GetWidth(RECT _rect)
{
	return (_rect.right - _rect.left);
}

long InputBox::GetHeight(RECT _rect)
{
	return (_rect.bottom - _rect.top);
}
