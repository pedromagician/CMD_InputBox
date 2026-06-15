#pragma once

#include "CommandLine.h"

class InputBox
{
public:
	enum MONITOR : unsigned int
	{
		_PRIMARY = 0,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION : int
	{
		_CENTER = 0,
		_TOP_CENTER = 1,
		_BOTTOM_CENTER = 2,
		_LEFT_CENTER = 3,
		_RIGHT_CENTER = 4,
		_XY = 5,
		_POINTER = 6,
	};

	typedef struct iPOINT
	{
		INT x;
		INT y;
	} iPOINT;

	struct InformationAboutPositionOfInputBox {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		iPOINT		delta;
	};

	static InformationAboutPositionOfInputBox& Position() { static InformationAboutPositionOfInputBox mPosition; return mPosition; };

	static int& Width() { static int mWidth = 600; return mWidth; };
	static int& FontSize() { static int mFontSize = 22; return mFontSize; };
	static int& LinesOfText() { static int mLinesOfText = 1; return mLinesOfText; };
	static bool& Password() { static bool mPassword = false; return mPassword; };
	static wstring& FontName() { static wstring mFontName = _T("Consolas"); return mFontName; };
	static bool& TopMost() { static bool mTopMost = false; return mTopMost; };
	static bool& BlockParent() { static bool mBlockParent = false; return mBlockParent; };

	static pair<bool, CommandLine::ColorRGB>& Border() { static pair<bool, CommandLine::ColorRGB> mBorder = pair<bool, CommandLine::ColorRGB>(false, CommandLine::ColorRGB(0, 0, 0)); return mBorder; };
	static pair<bool, CommandLine::ColorRGB>& Brush() { static pair<bool, CommandLine::ColorRGB> mBrush = pair<bool, CommandLine::ColorRGB>(false, CommandLine::ColorRGB(0, 0, 0)); return mBrush; };
	static pair<bool, CommandLine::ColorRGB>& Background() { static pair<bool, CommandLine::ColorRGB> mBackground = pair<bool, CommandLine::ColorRGB>(false, CommandLine::ColorRGB(0, 0, 0)); return mBackground; };
	static pair<bool, CommandLine::ColorRGB>& Pen() { static pair<bool, CommandLine::ColorRGB> mPen = pair<bool, CommandLine::ColorRGB>(false, CommandLine::ColorRGB(255, 255, 255)); return mPen; };

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
