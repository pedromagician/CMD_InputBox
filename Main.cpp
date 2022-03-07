#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "InputBox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int correctParameters = 0;
	bool help = false;

	UTL_Cmd cmd;
	cmd.Add(UTL_Cmd::_STRING,	&InputBox::title,		2, _T("-title"),	_T("-t"),					_T("The 'aaa' argument specifies the name of the dialog."));
	cmd.Add(UTL_Cmd::_STRING,	&InputBox::prompt,		2, _T("-message"),	_T("-m"),					_T("The 'aaa' argument specifies the text of the dialog. A new line can be inserted using \\n."));
	cmd.Add(UTL_Cmd::_STRING,	&InputBox::def,			2, _T("-default"),	_T("-d"),					_T("The 'aaa' argument specifies the default text of the input field."));
	cmd.Add(UTL_Cmd::_TRUE,		&help,					3, _T("-help"),		_T("-h"),	_T("-?"),		_T("The 'aaa' argument displays help."));
	cmd.Add(UTL_Cmd::_BOOL,		&InputBox::password,	2, _T("-password"),	_T("-p"),					_T("The 'aaa' toggle specifies that the text in the dialog box should be displayed with \"*\" characters, e.g. when entering passwords that should not be visible to the person standing behind the user."));
	cmd.Add(UTL_Cmd::_STRING,	&InputBox::fontName,	2, _T("-font"),		_T("-f"),					_T("The 'aaa' argument specifies the font of the dialog box."));
	cmd.Add(UTL_Cmd::_INT,		&InputBox::fontSize,	2, _T("-fontsize"),	_T("-fs"),					_T("The 'aaa' argument specifies the font size of the dialog box."));
	cmd.Add(UTL_Cmd::_INT,		&InputBox::linesOfText, 2, _T("-lines"),	_T("-l"),					_T("The 'aaa' argument specifies the number of lines of dialogue."));
	cmd.Add(UTL_Cmd::_INT,		&InputBox::width,		2, _T("-width"),	_T("-w"),					_T("The 'aaa' argument specifies the width of the dialog box."));
	cmd.Add(UTL_Cmd::_COLOR,	&InputBox::pen,			2, _T("-pen"),		_T("-p"),					_T("The 'aaa' argument specifies the colour of the pen."));
	cmd.Add(UTL_Cmd::_COLOR,	&InputBox::background,	2, _T("-background"),_T("-b"),					_T("The 'aaa' argument specifies the background color."));
	cmd.Add(UTL_Cmd::_COLOR,	&InputBox::brush,		2, _T("-brush"),	_T("-br"),					_T("The 'aaa' argument determines the brush."));

	if (cmd.ParseCommandLinbe(argc, argv, correctParameters) != 0)
		help = true;

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
