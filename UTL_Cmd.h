#pragma once

#include "stdafx.h"

class UTL_Cmd
{
public:
	static int ParseCommandLinbe(int argc, _TCHAR* argv[], int &iCorrectParameters, bool& bHelp, wstring &title, wstring &message, bool &bPassword, wstring& def);

	static bool IsHelp(wstring comm);
	static bool IsTitle(wstring comm);
	static bool IsMessage(wstring comm);
	static bool IsPassword(wstring comm);
	static bool IsDefault(wstring comm);

	static void Help();
};
