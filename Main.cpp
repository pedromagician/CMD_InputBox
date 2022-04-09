#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "InputBox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int correctParameters = 0;
	bool help = false;

	UTL_Cmd cmd;
	cmd.Add(UTL_Cmd::_STRING,	2,	_T("-title"),		_T("-t"),				_T("The 'xxx' argument specifies the name of the dialog."),											&InputBox::title);
	cmd.Add(UTL_Cmd::_STRING,	2,	_T("-message"),		_T("-m"),				_T("The 'xxx' argument specifies the text of the dialog. A new line can be inserted using \\n."),	&InputBox::prompt);
	cmd.Add(UTL_Cmd::_STRING,	2,	_T("-default"),		_T("-d"),				_T("The 'xxx' argument specifies the default text of the input field."),							&InputBox::def);
	cmd.Add(UTL_Cmd::_TRUE,		3,	_T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																				&help);
	cmd.Add(UTL_Cmd::_BOOL,		2,	_T("-password"),	_T("-pass"),			_T("The 'xxx' toggle specifies that the text in the dialog box should be displayed with \"*\" characters, e.g. when entering passwords that should not be visible to the person standing behind the user."),	&InputBox::password);
	cmd.Add(UTL_Cmd::_STRING,	2,	_T("-font"),		_T("-f"),				_T("The 'xxx' argument specifies the font of the dialog box."),										&InputBox::fontName);
	cmd.Add(UTL_Cmd::_INT,		2,	_T("-fontsize"),	_T("-fs"),				_T("The 'xxx' argument specifies the font size of the dialog box."),								&InputBox::fontSize);
	cmd.Add(UTL_Cmd::_INT,		2,	_T("-lines"),		_T("-l"),				_T("The 'xxx' argument specifies the number of lines of dialogue."),								&InputBox::linesOfText);
	cmd.Add(UTL_Cmd::_INT,		2,	_T("-width"),		_T("-w"),				_T("The 'xxx' argument specifies the width(px) of the dialog box."),								&InputBox::width);
	cmd.Add(UTL_Cmd::_COLOR,	2,	_T("-pen"),			_T("-p"),				_T("The 'xxx' argument specifies the colour of the pen."),											&InputBox::pen);
	cmd.Add(UTL_Cmd::_COLOR,	2,	_T("-background"),	_T("-b"),				_T("The 'xxx' argument specifies the background color."),											&InputBox::background);
	cmd.Add(UTL_Cmd::_COLOR,	2,	_T("-brush"),		_T("-br"),				_T("The 'xxx' argument determines the brush."),														&InputBox::brush);

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

	wstring result = _T("");
	if (InputBox::GetString(result) == 0)
		return 1;

	_tprintf(result.c_str());
	return 0;
}
