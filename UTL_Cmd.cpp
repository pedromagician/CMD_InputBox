#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

UTL_Cmd::UTL_Cmd()
{
	mArguments.clear();
}

UTL_Cmd::~UTL_Cmd()
{
}

void UTL_Cmd::Help()
{
	_tprintf(_T("InputBox 0.3\n"));
	_tprintf(_T("	InputBox for command line. Amiga Rulez!\n"));
	_tprintf(_T("\nUsage:\n"));
	_tprintf(_T("	InputBox [options]\n"));
	_tprintf(_T("\nOptions:\n"));

	for (const auto& it : mArguments)
	{
		for (const auto& text : it.text) {
			_tprintf((_T("	") + text + _T(" xxx\n")).c_str());
		}
		_tprintf((_T("		") + it.help + _T("\n")).c_str());
	}
}

void UTL_Cmd::Add(ARGUMENT_TYPE _type, int _num, ...)
{
	ARGUMENT arg;
	arg.text.clear();
	arg.type = _type;

	int argCount = _num;
	_num += 2; //help & data pointer
	if (_type == _ENUM) _num++; //convert table

	va_list arglist;
	va_start(arglist, _num);
	for (int x = 0; x < argCount; x++) {
		LPCWSTR tmp = va_arg(arglist, LPCWSTR);
		arg.text.push_back(tmp);
	}
	arg.help = va_arg(arglist, LPCWSTR);
	arg.pVar = static_cast <void*> (va_arg(arglist, void*));
	if (_type == _ENUM) arg.pTable = static_cast <map<wstring, UINT> *> (va_arg(arglist, void*));
	else arg.pTable = nullptr;
	va_end(arglist);

	mArguments.push_back(arg);
}

int UTL_Cmd::ParseCommandLine(int _argc, _TCHAR* _pArgv[], int& _correctParameters)
{
	_correctParameters = 0;

	for (int i = 1; i < _argc; i++) {
		bool unknown = true;
		for (unsigned int a = 0; a < mArguments.size(); a++) {
			if (find(mArguments[a].text.begin(), mArguments[a].text.end(), UTL_Conversion::TrimWhiteChar(_pArgv[i])) != mArguments[a].text.end()) {
				if (mArguments[a].type == _STRING) {
					i++;
					if (i < _argc) {
						wstring tmp = UTL_Conversion::TrimWhiteChar(_pArgv[i]);
						UTL_Conversion::StringReplaceAll(tmp, _T("\\n"), _T("\n"));
						*((wstring*)mArguments[a].pVar) = tmp;
						_correctParameters++;
						unknown = false;
					}
					break;
				}
				else if (mArguments[a].type == _BOOL) {
					*((bool*)mArguments[a].pVar) = !*((bool*)mArguments[a].pVar);
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _TRUE) {
					*((bool*)mArguments[a].pVar) = true;
					_correctParameters++;
					unknown = false;
					break;
				}
				else if (mArguments[a].type == _INT) {
					i++;
					if (i < _argc) {
						*((int*)mArguments[a].pVar) = UTL_Conversion::ToInt(UTL_Conversion::TrimWhiteChar(_pArgv[i]));
						_correctParameters++;
						unknown = false;
					}
					break;
				}
				else if (mArguments[a].type == _COLOR) {
					i++;
					if (i < _argc) {
						((pair<bool, wstring>*)mArguments[a].pVar)->first = true;
						((pair<bool, wstring>*)mArguments[a].pVar)->second = UTL_Conversion::TrimWhiteChar(_pArgv[i]);
						_correctParameters++;
						unknown = false;
					}
					break;
				}
				else if (mArguments[a].type == _ENUM) {
					i++;
					if (i < _argc) {
						wstring key = UTL_Conversion::TrimWhiteChar(_pArgv[i]);
						auto search = mArguments[a].pTable->find(key);
						if (search != mArguments[a].pTable->end()) {
							*((UINT*)mArguments[a].pVar) = search->second;
							unknown = false;
						}
						else {
							_tprintf(wstring(_T("Error - bad argument: ") + key + _T("\n")).c_str());
							return 1;
						}
						_correctParameters++;
					}
					break;
				}
				else {
					_tprintf(_T("Error - unknown type\n"));
					return 1;
				}
			}
		}
		if (unknown) {
			_tprintf(wstring(_T("Error - unknown argument: ") + (wstring)_pArgv[i] + _T("\n")).c_str());
			return 1;
		}
	}
	return 0;
}
