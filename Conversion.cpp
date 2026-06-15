#include "conversion.h"

static const wchar_t WHITE_SPACE_CHARACTERS[] = L" \t\n\v\f\r\u00A0";

int Conversion::ToInt(const wstring& _buff, int _base)
{
	return (int)_tcstol(_buff.c_str(), nullptr, _base);
}

wstring Conversion::LeftTrimString(const wstring& input, const wstring& chars)
{
	const size_t pos = input.find_first_not_of(chars);
	if (pos == wstring::npos)
		return wstring();

	return input.substr(pos);
}

wstring Conversion::RightTrimString(const wstring& input, const wstring& chars)
{
	const size_t pos = input.find_last_not_of(chars);
	if (pos == wstring::npos)
		return wstring();

	return input.substr(0, pos + 1);
}

wstring Conversion::TrimString(const wstring& input, const wstring& chars)
{
	const size_t end = input.find_last_not_of(chars);
	if (end == wstring::npos)
		return wstring();

	const size_t begin = input.find_first_not_of(chars);
	return input.substr(begin, end - begin + 1);
}

wstring Conversion::TrimWhiteChar(const wstring& _val)
{
	return TrimString(_val, WHITE_SPACE_CHARACTERS);
}

wstring Conversion::ToLower(const wstring& input)
{
	if (input.empty())
		return {};

	wstring result = input;
	CharLowerBuffW(result.data(), static_cast<DWORD>(result.size()));
	return result;
}

void Conversion::StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr)
{
	const size_t oldLen = _oldStr.length();
	const size_t newLen = _newStr.length();
	size_t position = 0;
	while ((position = _mess.find(_oldStr, position)) != wstring::npos)
	{
		_mess.replace(position, oldLen, _newStr);
		position += newLen;
	}
}

wstring Conversion::Utf32ToUtf16(char32_t ch)
{
	// BMP character (without surrogate pair)
	if (ch <= 0xFFFF)
	{
		// We will omit the surrogate range (D800–DFFF), as these should not appear here.
		if (ch >= 0xD800 && ch <= 0xDFFF)
			return L"";

		wchar_t w = static_cast<wchar_t>(ch);
		return wstring(1, w);
	}

	// character outside BMP → surrogate pair
	ch -= 0x10000;
	wchar_t high = static_cast<wchar_t>(0xD800 + (ch >> 10));
	wchar_t low = static_cast<wchar_t>(0xDC00 + (ch & 0x3FF));

	wchar_t buff[2] = { high, low };
	return wstring(buff, 2);
}

void Conversion::UnicodeCodeConverter(wstring& _mess)
{
	size_t startPos = 0, endPos = 0;

	bool unicodeProcess = true;
	while (unicodeProcess)
	{
		startPos = _mess.find(_T("\\u{"), startPos);
		if (startPos == wstring::npos) break;
		if (startPos != 0) {
			if (_mess[startPos - 1] == '\\') {
				wstring left = _mess.substr(0, startPos);
				wstring right = _mess.substr(startPos + 1, _mess.length() - startPos - 1);
				_mess = left + right;
				continue;
			}
		}

		endPos = _mess.find(_T("}"), startPos);
		if (endPos == wstring::npos) break;

		size_t tmpPos = _mess.find(_T("\\u{"), startPos + 1);
		if (tmpPos != wstring::npos && tmpPos < endPos) {
			startPos = endPos;
		}
		else {
			if (startPos < endPos) {
				wstring unicode = Conversion::TrimWhiteChar(_mess.substr(startPos + 3, endPos - startPos - 3));

				if (unicode.empty() == false) {
					if (unicode.length() % 2 != 0)
						unicode = _T("0") + unicode;


					char32_t ch32 = (char32_t)_tcstol(unicode.c_str(), nullptr, 16);
					wstring utf16String = Utf32ToUtf16(ch32);

					// replace the entire sequence \u{XXXX}
					_mess.replace(startPos, endPos - startPos + 1, utf16String);
				}
				else {
					startPos = endPos;
				}
			}
			else {
				startPos = endPos;
			}
		}
	}
}
