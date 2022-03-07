#include "UTL_conversion.h"

int UTL_Conversion::ToInt(const wstring& buff, int base)
{
	return _tcstol(buff.c_str(), nullptr, base);
}

wstring UTL_Conversion::LeftTrimString(wstring str, const wstring& val)
{
	size_t start = str.find_first_not_of(val);
	str = (start == string::npos) ? _T("") : str.substr(start);
	return str;
}

wstring UTL_Conversion::RightTrimString(wstring str, const wstring& val)
{
	size_t end = str.find_last_not_of(val);
	str = (end == string::npos) ? _T("") : str.substr(0, end + 1);
	return str;
}

wstring UTL_Conversion::TrimString(wstring str, const wstring& val)
{
	str = LeftTrimString(str, val);
	str = RightTrimString(str, val);
	return str;
}

wstring UTL_Conversion::TrimWhiteChar(const wstring& str)
{
	return TrimString(str, _T(" \t\n\v\f\r "));
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

void UTL_Conversion::HexToRGB(wstring hex, int& r, int& g, int& b)
{
	hex = TrimWhiteChar(hex);
	hex = LeftTrimString(hex, _T("#"));

	wstring sr = _T("ff"), sg = _T("ff"), sb = _T("ff");
	if (hex.length() == 6) {
		sr = hex.substr(0, 2);
		sg = hex.substr(2, 2);
		sb = hex.substr(4, 2);
	}
	else if (hex.length() == 3) {
		sr = hex.substr(0, 1);
		sg = hex.substr(1, 1);
		sb = hex.substr(2, 1);
	}
	else {
		_tprintf(_T("Error - bad color\n"));
	}
	r = ToInt(sr, 16);
	g = ToInt(sg, 16);
	b = ToInt(sb, 16);
}
