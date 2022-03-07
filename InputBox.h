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

	static bool GetString(wstring &result);

private:
	static HFONT hFont;
	static HWND  hWndParent;
	static HWND  hWndPrompt;
	static HWND  hWndInputBox;
	static HWND  hWndEdit;
	static HWND  hWndOK;
	static HWND  hWndCancel;
	static HBRUSH hbrBkgnd;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void SetTextAlignment(HWND hwnd, int textAlignment);
};
