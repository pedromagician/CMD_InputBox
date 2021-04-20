#pragma once

#include "stdafx.h"

class UTL_Conversion {
public:
	static int ToInt(const wstring& buff);

	static wstring LeftTrimString(wstring str, const wstring& val);
	static wstring RightTrimString(wstring str, const wstring& val);
	static wstring TrimString(wstring str, const wstring& val);
	static wstring TrimWhiteChar(const wstring& str);

	static wstring ToLower(wstring str);

	static void StringReplaceAll(wstring& mess, const wstring& oldStr, const wstring& newStr);
};
