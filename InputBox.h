#pragma once

class InputBox
{
public:
	static void Input(const wstring& title, const wstring &prompt, const wstring& def, wstring& strResult);
	static void Password(const wstring& title, const wstring& prompt, const wstring& def, wstring& strResult);

private:
	static void NewLineToVBSNewLine(wstring &buff);
	static LRESULT CALLBACK InputBoxCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static void InputBoxHelper(wstring title, wstring prompt, wstring def, bool pass, wstring &strResult);
};
