#pragma once

#include "stdafx.h"
#include "CommandLine.h"
#include "Conversion.h"
#include "InputBox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int correctParameters = 0;
	bool help = false;
	wstring monitor = _T("primary");
	wstring position = _T("center");
	int deltaX = 0;
	int deltaY = 0;

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2,	_T("-title"),		_T("-t"),				_T("The 'xxx' argument specifies the name of the dialog."),											&InputBox::title);
	cmd.Add(CommandLine::_STRING,	2,	_T("-message"),		_T("-m"),				_T("The 'xxx' argument specifies the text of the dialog. A new line can be inserted using \\n."),	&InputBox::prompt);
	cmd.Add(CommandLine::_STRING,	2,	_T("-default"),		_T("-d"),				_T("The 'xxx' argument specifies the default text of the input field."),							&InputBox::def);
	cmd.Add(CommandLine::_TRUE,		3,	_T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																				&help);
	cmd.Add(CommandLine::_TRUE,		2,	_T("-password"),	_T("-pass"),			_T("The toggle specifies that the text in the dialog box should be displayed with \"*\" characters, e.g. when entering passwords that should not be visible to the person standing behind the user."),	&InputBox::password);
	cmd.Add(CommandLine::_STRING,	2,	_T("-font"),		_T("-f"),				_T("The 'xxx' argument specifies the font of the dialog box."),										&InputBox::fontName);
	cmd.Add(CommandLine::_INT,		2,	_T("-fontsize"),	_T("-fs"),				_T("The 'xxx' argument specifies the font size of the dialog box."),								&InputBox::fontSize);
	cmd.Add(CommandLine::_INT,		2,	_T("-lines"),		_T("-l"),				_T("The 'xxx' argument specifies the number of lines of dialogue."),								&InputBox::linesOfText);
	cmd.Add(CommandLine::_INT,		2,	_T("-width"),		_T("-w"),				_T("The 'xxx' argument specifies the width(px) of the dialog box."),								&InputBox::width);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-pen"),			_T("-p"),				_T("The 'xxx' argument specifies the colour of the pen."),											&InputBox::pen);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-background"),	_T("-b"),				_T("The 'xxx' argument specifies the background color."),											&InputBox::background);
	cmd.Add(CommandLine::_COLOR,	2,	_T("-brush"),		_T("-br"),				_T("The 'xxx' argument determines the brush."),														&InputBox::brush);
	cmd.Add(CommandLine::_STRING,	2,	_T("-monitor"),		_T("-mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),		&monitor);
	cmd.Add(CommandLine::_STRING,	2,	_T("-position"),	_T("-pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."), &position);
	cmd.Add(CommandLine::_INT,		1,	_T("-x"),									_T("The 'xxx' argument specifies the position offset along the X coordinate."),										&deltaX);
	cmd.Add(CommandLine::_INT,		1,	_T("-y"),									_T("The 'xxx' argument specifies the position offset along the Y coordinate."),										&deltaY);

	if (cmd.ParseCommandLine(argc, argv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (InputBox::prompt.empty()) {
		_tprintf(_T("Error - message is emnpty\n"));
		return 1;
	}

	InputBox::position.monitor	= InputBox::_PRIMARY;
	InputBox::position.id		= 0;
	InputBox::position.type		= InputBox::_CENTER;
	InputBox::position.delta	= { 0, 0 };

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			InputBox::position.monitor = InputBox::_PRIMARY;
	else if (monitor == _T("mouse"))		InputBox::position.monitor = InputBox::_MOUSE;
	else if (monitor == _T("mousepointer"))	InputBox::position.monitor = InputBox::_MOUSE_POINTER;
	else {
		InputBox::position.monitor = InputBox::_ID;
		InputBox::position.id = (UINT)Conversion::ToInt(monitor);
	}

	position = Conversion::ToLower(Conversion::TrimWhiteChar(position));
	if (position == _T("center"))		InputBox::position.type = InputBox::_CENTER;
	else if (position == _T("top"))		InputBox::position.type = InputBox::_TOP_CENTER;
	else if (position == _T("bottom"))	InputBox::position.type = InputBox::_BOTTOM_CENTER;
	else if (position == _T("left"))	InputBox::position.type = InputBox::_LEFT_CENTER;
	else if (position == _T("right"))	InputBox::position.type = InputBox::_RIGHT_CENTER;
	else if (position == _T("pointer"))	InputBox::position.type = InputBox::_POINTER;
	else if (position == _T("xy"))		InputBox::position.type = InputBox::_XY;

	InputBox::position.delta = { deltaX, deltaY };

	wstring result = _T("");
	if (InputBox::GetString(result) == 0)
		return 1;

	_tprintf(result.c_str());
	return 0;
}