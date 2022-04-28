#pragma once

class InputBox
{
public:
	enum MONITOR {
		_PRIMARY,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION {
		_CENTER,
		_TOP_CENTER,
		_BOTTOM_CENTER,
		_LEFT_CENTER,
		_RIGHT_CENTER,
		_XY,
		_POINTER,
	};

	static struct InformationAboutPositionOfInputBox {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		POINT		delta;
	} position;

	static int width;
	static int fontSize;
	static int linesOfText;
	static bool password;
	static wstring fontName;

	static pair<bool, wstring> brush;
	static pair<bool, wstring> background;
	static pair<bool, wstring> pen;

	static wstring title;
	static wstring prompt;
	static wstring def;

	static bool GetString(wstring& _result);

private:
	static HFONT mhFont;
	static HWND  mhWndParent;
	static HWND  mhWndPrompt;
	static HWND  mhWndInputBox;
	static HWND  mhWndEdit;
	static HWND  mhWndOK;
	static HWND  mhWndCancel;
	static HBRUSH mhbrBkgnd;

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	static void SetTextAlignment(HWND _hwnd, int _textAlignment);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
	static long GetWidth(RECT _rect);
	static long GetHeight(RECT _rect);
};
