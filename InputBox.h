#pragma once

class InputBox
{
public:
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
};
