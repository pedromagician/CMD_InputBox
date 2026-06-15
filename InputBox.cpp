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
static HBRUSH mhbrBkgnd = nullptr;
static HBRUSH mhbrDialog = nullptr;
static HPEN   mhPenBorder = nullptr;
static COLORREF g_bgColor = RGB(240, 240, 240);
static COLORREF g_borderColor = RGB(0, 0, 0);

void InputBox::SetTextAlignment(HWND _hwnd, int _textAlignment)
{
	LONG_PTR style = GetWindowLongPtr(_hwnd, GWL_STYLE);
	LONG_PTR currentAlign = style & (SS_LEFT | SS_CENTER | SS_RIGHT);

	LONG_PTR desiredAlign = 0;
	if (_textAlignment == 0)      desiredAlign = SS_LEFT;
	else if (_textAlignment == 1) desiredAlign = SS_CENTER;
	else if (_textAlignment == 2) desiredAlign = SS_RIGHT;

	if (currentAlign == desiredAlign)
		return;

	style &= ~(SS_LEFT | SS_CENTER | SS_RIGHT);

	style |= desiredAlign;

	if (SetWindowLongPtr(_hwnd, GWL_STYLE, style) == 0) {
		if (GetLastError() != 0) {
			wcout << L"Error - SetWindowLongPtr failed for alignment" << endl;
		}
	}

	if (!SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_DRAWFRAME)) {
		wcout << L"Error - SetWindowPos failed" << endl;
	}
}

LRESULT CALLBACK InputBox::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	LOGFONT lfont;
	HINSTANCE hInst = nullptr;

	switch (_message) {
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
		{
			HDC hdc = (HDC)_wParam;

			int r, g, b;

			if (!Pen().first && !Background().first && !Brush().first)
			{
				SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
				SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
				return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);
			}

			if (Pen().first) {
				r = Pen().second.r;
				g = Pen().second.g;
				b = Pen().second.b;
				SetTextColor(hdc, RGB(r, g, b));
			}
			else {
				SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
			}

			SetBkColor(hdc, g_bgColor);

			return (INT_PTR)mhbrBkgnd;
		}
		case WM_ERASEBKGND:
		{
			HDC hdc = (HDC)_wParam;

			RECT rc;
			GetClientRect(_hWnd, &rc);

			if (mhbrDialog)
				FillRect(hdc, &rc, mhbrDialog);
			else
				FillRect(hdc, &rc, (HBRUSH)(COLOR_BTNFACE + 1));

			return 1;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(_hWnd, &ps);

			if (Border().first && mhWndEdit && mhPenBorder)
			{
				RECT rc;
				GetWindowRect(mhWndEdit, &rc);

				MapWindowPoints(HWND_DESKTOP, _hWnd, (LPPOINT)&rc, 2);

				HGDIOBJ oldPen = SelectObject(hdc, mhPenBorder);
				HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

				Rectangle(hdc,
					rc.left - 1,
					rc.top - 1,
					rc.right + 1,
					rc.bottom + 1
				);

				SelectObject(hdc, oldPen);
				SelectObject(hdc, oldBrush);
			}

			EndPaint(_hWnd, &ps);
			return 0;
		}
		case WM_CREATE: {
			memset(&lfont, 0, sizeof(lfont));
			lstrcpy(lfont.lfFaceName, (LPWSTR)InputBox::FontName().c_str());
			lfont.lfHeight = FontSize();
			lfont.lfWeight = FW_NORMAL;//FW_BOLD;
			lfont.lfItalic = FALSE;
			lfont.lfCharSet = DEFAULT_CHARSET;
			lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			lfont.lfQuality = DEFAULT_QUALITY;
			lfont.lfPitchAndFamily = DEFAULT_PITCH;
			mhFont = CreateFontIndirect(&lfont);

			hInst = GetModuleHandle(nullptr);

			int r, g, b;

			if (Background().first) {
				r = Background().second.r;
				g = Background().second.g;
				b = Background().second.b;
			}
			else if (Brush().first) {
				r = Brush().second.r;
				g = Brush().second.g;
				b = Brush().second.b;
			}
			else {
				r = g = b = 240;
			}
			g_bgColor = RGB(r, g, b);

			if (mhbrBkgnd) {
				DeleteObject(mhbrBkgnd);
				mhbrBkgnd = nullptr;
			}
			mhbrBkgnd = CreateSolidBrush(g_bgColor);

			if (mhbrDialog) {
				DeleteObject(mhbrDialog);
				mhbrDialog = nullptr;
			}
			mhbrDialog = CreateSolidBrush(g_bgColor);

			if (Border().first) {
				r = Border().second.r;
				g = Border().second.g;
				b = Border().second.b;
				g_borderColor = RGB(r, g, b);
			}
			else {
				g_borderColor = RGB(0, 0, 0);
			}

			if (mhPenBorder) {
				DeleteObject(mhPenBorder);
				mhPenBorder = nullptr;
			}
			mhPenBorder = CreatePen(PS_SOLID, 1, g_borderColor);

			// input
			int inputX = 5;
			int inputY = 10 + FontSize() / 2 + FontSize() * LinesOfText();
			int inputWidth = InputBox::Width() - 30;
			int inputHeight = FontSize() + 2;

			DWORD exStyle = WS_EX_STATICEDGE;

			if (Pen().first || Background().first || Brush().first)
				exStyle = 0;

			if (InputBox::Password())
				mhWndEdit = CreateWindowEx(exStyle, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD, inputX, inputY, inputWidth, inputHeight, _hWnd, nullptr, hInst, nullptr);
			else
				mhWndEdit = CreateWindowEx(exStyle, _T("edit"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, inputX, inputY, inputWidth, inputHeight, _hWnd, nullptr, hInst, nullptr);

			if (mhWndEdit == nullptr) return (LRESULT)nullptr;
			SendMessage((mhWndEdit), WM_SETFONT, (WPARAM)mhFont, 0);
			SetTextAlignment(mhWndEdit, SS_LEFT);
			InvalidateRect(_hWnd, NULL, TRUE);

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

			SetTextAlignment(mhWndPrompt, SS_LEFT);
			SetWindowText(mhWndPrompt, Prompt().c_str());

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

			if (!SetWindowPos(_hWnd, HWND_TOPMOST, x, y, 0, 0, flags)) {
				wcout << L"Error - SetWindowPos failed" << endl;
			}

			ShowWindow(_hWnd, SW_SHOW);
			UpdateWindow(_hWnd);
			SetForegroundWindow(_hWnd);

			// icon app
			if (InputBox::IconApp().empty() == false) {
				HICON hIcon = (HICON)LoadImage(hInst, InputBox::IconApp().c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
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
		case WM_CLOSE:
			PostMessage(mhWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
			return 0;
		case WM_DESTROY: {
			if (mhFont) {
				DeleteObject(mhFont);
				mhFont = nullptr;
			}

			if (mhbrBkgnd) {
				DeleteObject(mhbrBkgnd);
				mhbrBkgnd = nullptr;
			}

			if (mhbrDialog) {
				DeleteObject(mhbrDialog);
				mhbrDialog = nullptr;
			}

			if (mhPenBorder) {
				DeleteObject(mhPenBorder);
				mhPenBorder = nullptr;
			}

			HICON hIcon = (HICON)SendMessage(_hWnd, WM_GETICON, ICON_SMALL, 0);
			if (hIcon) DestroyIcon(hIcon);
			hIcon = (HICON)SendMessage(_hWnd, WM_GETICON, ICON_BIG, 0);
			if (hIcon) DestroyIcon(hIcon);

			EnableWindow(mhWndParent, TRUE);
			SetForegroundWindow(mhWndParent);
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch (HIWORD(_wParam)) {
				case BN_CLICKED:
					if ((HWND)_lParam == mhWndOK)
						PostMessage(_hWnd, WM_KEYDOWN, VK_RETURN, 0);
					if ((HWND)_lParam == mhWndCancel)
						PostMessage(_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
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
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
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

	if (parent)
		EnableWindow(parent, FALSE);

	DWORD dwStyle = WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;
	dwStyle = dwStyle & ~WS_MINIMIZEBOX;

	int dialogWidth = InputBox::Width();
	int dialogHeight = 50 + buttonY + buttonHeight;

	int x = rc.left + (GetWidth(rc) - dialogWidth) / 2;
	int y = rc.top + (GetHeight(rc) - dialogHeight) / 2;

	mhWndInputBox = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("InputBox"), InputBox::Title().c_str(), dwStyle,
		x, y, dialogWidth, dialogHeight,
		parent, nullptr, nullptr, nullptr
	);
	if (!mhWndInputBox) {
		wcout << L"CreateWindowEx failed: " << GetLastError() << endl;
		return false;
	}

	HMENU pSysMenu = GetSystemMenu(mhWndInputBox, FALSE);
	if (pSysMenu) {
		RemoveMenu(pSysMenu, SC_RESTORE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_SIZE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_MINIMIZE, MF_BYCOMMAND);
		RemoveMenu(pSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}

	SendMessage((HWND)mhWndOK, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage((HWND)mhWndCancel, BM_SETSTYLE, (WPARAM)LOWORD(BS_PUSHBUTTON), MAKELPARAM(TRUE, 0));
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SendMessage(mhWndEdit, EM_REPLACESEL, 0, (LPARAM)Def().c_str());
	SendMessage(mhWndEdit, EM_SETSEL, 0, -1);
	SetFocus(mhWndEdit);

	BOOL returnCode = false;
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		if (msg.message == WM_KEYDOWN)
		{
			HWND focused = GetFocus();

			if (msg.wParam == VK_ESCAPE)
			{
				DestroyWindow(mhWndInputBox);
				returnCode = false;
				continue;
			}
			else if (msg.wParam == VK_RETURN)
			{
				if (focused == mhWndEdit || focused == mhWndOK)
				{
					int nCount = GetWindowTextLengthW(mhWndEdit) + 1;

					vector<wchar_t> buff(nCount);
					GetWindowTextW(mhWndEdit, buff.data(), nCount);
					_result = buff.data();

					DestroyWindow(mhWndInputBox);
					returnCode = true;
					continue;
				}
				else if (focused == mhWndCancel)
				{
					DestroyWindow(mhWndInputBox);
					returnCode = false;
					continue;
				}
			}
			else if (msg.wParam == VK_TAB)
			{
				bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
				if (!shift)
				{
					if (focused == mhWndEdit) SetFocus(mhWndOK);
					else if (focused == mhWndOK) SetFocus(mhWndCancel);
					else SetFocus(mhWndEdit);
				}
				else
				{
					if (focused == mhWndEdit) SetFocus(mhWndCancel);
					else if (focused == mhWndCancel) SetFocus(mhWndOK);
					else SetFocus(mhWndEdit);
				}
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
