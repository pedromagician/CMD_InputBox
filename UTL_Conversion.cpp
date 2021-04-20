#include "UTL_conversion.h"

#define WHITE_SPACE_CHARACTERS _T(" \t\n\v\f\r ")

int UTL_Conversion::ToInt(const wstring& buff)
{
	return _tcstol(buff.c_str(), nullptr, 10);
}

wstring UTL_Conversion::LeftTrimString(wstring str, const wstring& val)
{
	wstring::size_type pos = str.find_first_not_of(val);
	if (pos != wstring::npos)
		str.erase(0, pos);
	else
		str.erase(str.begin(), str.end());

	return str;
}

wstring UTL_Conversion::RightTrimString(wstring str, const wstring& val)
{
	wstring::size_type pos = str.find_last_not_of(val);
	if (pos != wstring::npos)
		str.erase(pos + 1);
	else
		str.erase(str.begin(), str.end());

	return str;
}

wstring UTL_Conversion::TrimString(wstring str, const wstring& val)
{
	wstring::size_type pos = str.find_last_not_of(val);
	if (pos != wstring::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(val);
		if (pos != wstring::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());

	return str;
}

wstring UTL_Conversion::TrimWhiteChar(const wstring& str)
{
	return TrimString(str, WHITE_SPACE_CHARACTERS);
}

wstring UTL_Conversion::ToLower(wstring str)
{
	transform(str.begin(), str.end(), str.begin(), [](wchar_t c) { return towlower(c); });
	return str;
}

void UTL_Conversion::StringReplaceAll(wstring& mess, const wstring& oldStr, const wstring& newStr)
{
	const size_t oldLen = oldStr.length();
	const size_t newLen = newStr.length();
	size_t position = 0;
	while ((position = mess.find(oldStr, position)) != string::npos)
	{
		mess.replace(position, oldLen, newStr);
		position += newLen;
	}
}
