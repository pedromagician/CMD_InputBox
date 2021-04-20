#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "InputBox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iCorrectParameters = 0;
	bool bHelp = false;
	wstring title = _T("");
	wstring message = _T("");
	wstring def = _T("");
	bool bPassword = false;

	if (UTL_Cmd::ParseCommandLinbe(argc, argv, iCorrectParameters, bHelp, title, message, bPassword, def) != 0)
		bHelp = true;

	if (bHelp || iCorrectParameters == 0) {
		UTL_Cmd::Help();
		return 0;
	}

	if (message.empty()) {
		_tprintf(_T("Error - message is emnpty\n"));
		return 1;
	}

	wstring ret = _T("");

	if (bPassword)
		InputBox::Password(title, message, def, ret);
	else
		InputBox::Input(title, message, def, ret);

	_tprintf(ret.c_str());
	return 0;
}
