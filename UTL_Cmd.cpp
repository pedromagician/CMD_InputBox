#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

UTL_Cmd::UTL_Cmd()
{
	arguments.clear();
}

UTL_Cmd::~UTL_Cmd()
{
}

void UTL_Cmd::Help()
{
	_tprintf(_T("InputBox 0.2\n"));
	_tprintf(_T("	InputBox for command line. Amiga Rulez!\n"));
	_tprintf(_T("\nUsage:\n"));
	_tprintf(_T("	InputBox [OPTIONS]\n"));
	_tprintf(_T("\nOptions:\n"));

	for (const auto &it : arguments)
	{
		for (const auto& text : it.text) {
			_tprintf((_T("	") + text + _T(" aaa\n")).c_str());
		}
		_tprintf((_T("		") + it.help + _T("\n")).c_str());
	}
}

void UTL_Cmd::Add(ARGUMENT_TYPE type, void* var, int num, ...)
{
	ARGUMENT arg;
	arg.text.clear();
	arg.type = type;
	arg.var = var;

	va_list arglist;
	num++;
	va_start(arglist, num);
	for (int x = 0; x < num - 1; x++) {
		LPCWSTR tmp = va_arg(arglist, LPCWSTR);
		arg.text.push_back(tmp);
	}
	arg.help = va_arg(arglist, LPCWSTR);
	va_end(arglist);

	arguments.push_back(arg);
}

int UTL_Cmd::ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters)
{
	iCorrectParameters = 0;

	for (int i = 0; i < argc; i++) {
		for (unsigned int a = 0; a < arguments.size(); a++) {
			if (find(arguments[a].text.begin(), arguments[a].text.end(), UTL_Conversion::TrimWhiteChar(argv[i])) != arguments[a].text.end()) {
				if (arguments[a].type == _STRING) {
					i++;
					if (i < argc) {
						wstring tmp = UTL_Conversion::TrimWhiteChar(argv[i]);
						UTL_Conversion::StringReplaceAll(tmp, _T("\\n"), _T("\n"));
						*((wstring*)arguments[a].var) = tmp;
						iCorrectParameters++;
					}
					break;
				}
				else if (arguments[a].type == _BOOL) {
					*((bool*)arguments[a].var) = !*((bool*)arguments[a].var);
					iCorrectParameters++;
					break;
				}
				else if (arguments[a].type == _TRUE) {
					*((bool*)arguments[a].var) = true;
					iCorrectParameters++;
					break;
				}
				else if (arguments[a].type == _INT) {
					i++;
					if (i < argc) {
						*((int*)arguments[a].var) = UTL_Conversion::ToInt(UTL_Conversion::TrimWhiteChar(argv[i]));
						iCorrectParameters++;
					}
					break;
				}
				else if (arguments[a].type == _COLOR) {
					i++;
					if (i < argc) {
						((pair<bool, wstring>*)arguments[a].var)->first = true;
						((pair<bool, wstring>*)arguments[a].var)->second = UTL_Conversion::TrimWhiteChar(argv[i]);
						iCorrectParameters++;
					}
					break;
				}
			}
		}
	}
	return 0;
}
