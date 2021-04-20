#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

bool UTL_Cmd::IsHelp(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-?") || comm == _T("-help") || comm == _T("-h")) return true;
	return false;
}

bool UTL_Cmd::IsTitle(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-title")) return true;
	return false;
}

bool UTL_Cmd::IsMessage(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-message")) return true;
	return false;
}

bool UTL_Cmd::IsDefault(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-default") || comm == _T("-def")) return true;
	return false;
}

bool UTL_Cmd::IsPassword(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-pass") || comm == _T("-password")) return true;
	return false;
}

void UTL_Cmd::Help()
{
	_tprintf(_T("InputBox 0.1\n"));
	_tprintf(_T("	InputBox for command line. Amiga Rulez!\n"));
	_tprintf(_T("\nUsage:\n"));
	_tprintf(_T("	InputBox [OPTIONS]\n"));
	_tprintf(_T("\nOptions:\n"));
	_tprintf(_T("	-title TITLE\n"));
	_tprintf(_T("		The TITLE argument specifies the title of the input box.\n"));
	_tprintf(_T("	-message MESSAGE\n"));
	_tprintf(_T("		The MESSAGE argument specifies the text of the input box. Newline can be embedded using \\n.\n"));
	_tprintf(_T("	-pass\n"));
	_tprintf(_T("		The pass switch specifies that the text in the string gadget should be displayed with '*' characters, e.g. for entering passwords that should not be visible to a person behind the user.\n"));
	_tprintf(_T("	-default MESSAGE\n"));
	_tprintf(_T("		The MESSAGE argument specifies the default text of the input box.\n"));
}

int UTL_Cmd::ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters, bool& bHelp, wstring& title, wstring& message, bool& bPassword, wstring& def)
{
	iCorrectParameters = 0;

	for (int i = 0; i < argc; i++) {
		wstring tmp = argv[i];
		tmp = UTL_Conversion::TrimWhiteChar(tmp);

		if (UTL_Cmd::IsHelp(tmp)) {
			bHelp = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsTitle(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				title = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsMessage(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				message = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsPassword(tmp)) {
			i++;
			bPassword = true;
			continue;
		}

		if (UTL_Cmd::IsDefault(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				def = tmp;
				iCorrectParameters++;
			}
			continue;
		}
	}

	return 0;
}
