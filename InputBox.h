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

	struct InformationAboutPositionOfInputBox {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		POINT		delta;
	};

	static InformationAboutPositionOfInputBox& Position() { static InformationAboutPositionOfInputBox mPosition; return mPosition; };

	static int& Width() { static int mWidth = 600; return mWidth; };
	static int& FontSize() { static int mFontSize = 22; return mFontSize; };
	static int& LinesOfText() { static int mLinesOfText = 1; return mLinesOfText; };
	static bool& Password() { static bool mPassword = false; return mPassword; };
	static wstring& FontName() { static wstring mFontName = _T("Consolas"); return mFontName; };
	static bool& TopMost() { static bool mTopMost = false; return mTopMost; };
	static bool& BlockParent() { static bool mBlockParent = false; return mBlockParent; };

	static pair<bool, wstring>& Brush() { static pair<bool, wstring> mBrush = pair<bool, wstring>(false, _T("#000000")); return mBrush; };
	static pair<bool, wstring>& Background() { static pair<bool, wstring> mBackground = pair<bool, wstring>(false, _T("#000000")); return mBackground; };
	static pair<bool, wstring>& Pen() { static pair<bool, wstring> mPen = pair<bool, wstring>(false, _T("#ffffff")); return mPen; };

	static wstring& Title() { static wstring mTitle = _T("Input Box"); return mTitle;	};
	static wstring& Prompt() { static wstring mPrompt = _T("Please input text"); return mPrompt; };
	static wstring& Def() { static wstring mDef = _T(""); return mDef; };

	static wstring& IconApp() { static wstring mIconApp = _T(""); return mIconApp; };

	static bool GetString(wstring& _result);

private:
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	static void SetTextAlignment(HWND _hwnd, int _textAlignment);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
	static long GetWidth(RECT _rect);
	static long GetHeight(RECT _rect);
};
