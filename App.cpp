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

	map<wstring, UINT> string2positionType{
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

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2,	_T("title"),		_T("t"),				_T("The 'xxx' argument specifies the name of the dialog."),																					&InputBox::Title());
	cmd.Add(CommandLine::_STRING,	2,	_T("message"),		_T("m"),				_T("The 'xxx' argument specifies the text of the dialog. A new line can be inserted using \\n. Inserting Unicode characters \\u{1F44D}."),	&InputBox::Prompt());
	cmd.Add(CommandLine::_STRING,	2,	_T("default"),		_T("d"),				_T("The 'xxx' argument specifies the default text of the input field."),							&InputBox::Def());
	cmd.Add(CommandLine::_TRUE,		3,	_T("help"),			_T("h"),	_T("?"),	_T("To view help."),																				&help);
	cmd.Add(CommandLine::_TRUE,		2,	_T("password"),		_T("pass"),				_T("The toggle specifies that the text in the dialog box should be displayed with \"*\" characters, e.g. when entering passwords that should not be visible to the person standing behind the user."),	&InputBox::Password());
	cmd.Add(CommandLine::_STRING,	2,	_T("font"),			_T("f"),				_T("The 'xxx' argument specifies the font of the dialog box."),										&InputBox::FontName());
	cmd.Add(CommandLine::_INT,		2,	_T("fontsize"),		_T("fs"),				_T("The 'xxx' argument specifies the font size of the dialog box."),								&InputBox::FontSize());
	cmd.Add(CommandLine::_INT,		2,	_T("lines"),		_T("l"),				_T("The 'xxx' argument specifies the number of lines of dialogue."),								&InputBox::LinesOfText());
	cmd.Add(CommandLine::_INT,		2,	_T("width"),		_T("w"),				_T("The 'xxx' argument specifies the width(px) of the dialog box."),								&InputBox::Width());
	cmd.Add(CommandLine::_COLOR,	2,	_T("pen"),			_T("p"),				_T("The 'xxx' argument specifies the colour of the pen."),											&InputBox::Pen());
	cmd.Add(CommandLine::_COLOR,	2,	_T("background"),	_T("b"),				_T("The 'xxx' argument specifies the background color."),											&InputBox::Background());
	cmd.Add(CommandLine::_COLOR,	2,	_T("brush"),		_T("br"),				_T("The 'xxx' argument determines the brush."),														&InputBox::Brush());
	cmd.Add(CommandLine::_STRING,	2,	_T("monitor"),		_T("mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),		&monitor);
	cmd.Add(CommandLine::_ENUM,		2,	_T("position"),		_T("pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."), &InputBox::Position().type,	&string2positionType);
	cmd.Add(CommandLine::_INT,		1,	_T("x"),									_T("The 'xxx' argument specifies the position offset along the X coordinate."),						&InputBox::Position().delta.x);
	cmd.Add(CommandLine::_INT,		1,	_T("y"),									_T("The 'xxx' argument specifies the position offset along the Y coordinate."),						&InputBox::Position().delta.y);
	cmd.Add(CommandLine::_TRUE,		1,	_T("topmost"),								_T("This argument places the window above all windows that are not in the highest position.The window retains its highest position even if it is deactivated."),	&InputBox::TopMost());
	cmd.Add(CommandLine::_STRING,	1,	_T("iconapp"),								_T("The 'xxx' argument specifies the icon file for the application. (ICO/BMP)"),					&InputBox::IconApp());
	cmd.Add(CommandLine::_TRUE,		1,	_T("block"),								_T("This argument blocks the parent process that started the InputBox."),							&InputBox::BlockParent());

	if (cmd.ParseCommandLine(argc, argv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (InputBox::Prompt().empty()) {
		wcout << _T("Error - message is emnpty") << endl;
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
