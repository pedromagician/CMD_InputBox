#include "stdafx.h"

#ifdef _DEBUG
Debug theDebug;
#endif

#include "CommandLine.h"
#include "Conversion.h"
#include "InputBox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);

	int correctParameters = 0;
	bool help = false;
	wstring monitor = _T("primary");

	map<wstring, int> string2positionType{
		{_T("center"),	InputBox::_CENTER},
		{_T("top"),		InputBox::_TOP_CENTER},
		{_T("bottom"),	InputBox::_BOTTOM_CENTER},
		{_T("left"),	InputBox::_LEFT_CENTER},
		{_T("right"),	InputBox::_RIGHT_CENTER},
		{_T("pointer"),	InputBox::_POINTER},
		{_T("xy"),		InputBox::_XY},
	};

	InputBox::Position().monitor	= InputBox::_PRIMARY;
	InputBox::Position().id			= 0;
	InputBox::Position().type		= InputBox::_CENTER;
	InputBox::Position().delta		= { 0, 0 };
	int type = (int)InputBox::Position().type;

	CommandLine cmd;
	{
		cmd.AddString({ _T("title"), _T("t") },
			_T("Specifies the title text displayed in the dialog window."),
			InputBox::Title());

		cmd.AddString({ _T("message"), _T("m") },
			_T("Specifies the message text displayed in the dialog. Use \\n for line breaks; Unicode characters supported via \\u{XXXX}."),
				InputBox::Prompt());

		cmd.AddString({ _T("default"), _T("d") },
			_T("Specifies the default text pre-filled in the input field."),
			InputBox::Def());

		cmd.AddBool({ _T("help"), _T("h"), _T("?") },
			_T("Displays this help information."),
			help);

		cmd.AddBool({ _T("password"), _T("pass") },
			_T("Masks all input characters with '*' (password mode). Useful for sensitive data entry."),
				InputBox::Password());

		cmd.AddString({ _T("font"), _T("f") },
			_T("Specifies the font name used for rendering text in the dialog."),
			InputBox::FontName());

		cmd.AddInt({ _T("fontsize"), _T("fs") },
			_T("Specifies the font size in points."),
			InputBox::FontSize());

		cmd.AddInt({ _T("lines"), _T("l") },
			_T("Specifies the number of lines allocated for the message text area."),
			InputBox::LinesOfText());

		cmd.AddInt({ _T("width"), _T("w") },
			_T("Specifies the dialog width in pixels."),
			InputBox::Width());

		cmd.AddColor({ _T("pen"), _T("p") },
			_T("Specifies the pen color (text/gadget foreground color)."),
			InputBox::Pen().second);

		cmd.AddColor({ _T("background"), _T("b") },
			_T("Specifies the background fill color of the dialog."),
			InputBox::Background().second);

		cmd.AddColor({ _T("brush"), _T("br") },
			_T("Specifies the brush color (area fill or pattern color)."),
			InputBox::Brush().second);

		cmd.AddColor({ _T("border"), _T("bo") },
			_T("Specifies the border frame color around the dialog."),
			InputBox::Border().second);

		cmd.AddString({ _T("monitor"), _T("mon") },
			_T("Specifies the target monitor for display: Primary|Mouse|MousePointer|0|1|2|n (where n is monitor index)."),
			monitor);

		cmd.AddEnum({ _T("position"), _T("pos") },
			_T("Specifies the dialog placement: Center|Top|Bottom|Left|Right|Pointer|xy."),
			string2positionType, type);

		cmd.AddInt({ _T("x") },
			_T("Specifies the X-axis offset from the base position in pixels."),
			InputBox::Position().delta.x);

		cmd.AddInt({ _T("y") },
			_T("Specifies the Y-axis offset from the base position in pixels."),
			InputBox::Position().delta.y);

		cmd.AddBool({ _T("topmost") },
			_T("Forces the dialog to remain above all non-topmost windows."),
			InputBox::TopMost());

		cmd.AddString({ _T("iconapp") },
			_T("Specifies the application icon file path (supports ICO/BMP formats)."),
			InputBox::IconApp());

		cmd.AddBool({ _T("block") },
			_T("Blocks parent process execution until the dialog is closed (modal behavior)."),
			InputBox::BlockParent());
	}

	if (!cmd.ParseCommandLine(argc, argv, correctParameters) || correctParameters == 0 || help) {
		cmd.Help();
		return 0;
	}
	InputBox::Position().type = (InputBox::POSITION)type;
	InputBox::Pen().first = cmd.WasProvided(L"pen");
	InputBox::Background().first = cmd.WasProvided(L"background");
	InputBox::Brush().first = cmd.WasProvided(L"brush");
	InputBox::Border().first = cmd.WasProvided(L"border");

	if (InputBox::Prompt().empty()) {
		wcout << _T("Error - message is empty") << endl;
		return 1;
	}

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			InputBox::Position().monitor = InputBox::_PRIMARY;
	else if (monitor == _T("mouse"))		InputBox::Position().monitor = InputBox::_MOUSE;
	else if (monitor == _T("mousepointer"))	InputBox::Position().monitor = InputBox::_MOUSE_POINTER;
	else {
		InputBox::Position().monitor = InputBox::_ID;
		InputBox::Position().id = (UINT)Conversion::ToInt(monitor);
	}

	Conversion::UnicodeCodeConverter(InputBox::Title());
	Conversion::UnicodeCodeConverter(InputBox::Prompt());

	wstring result = _T("");
	if (InputBox::GetString(result) != 0) {
		wcout << result;
	}

	return 0;
}
