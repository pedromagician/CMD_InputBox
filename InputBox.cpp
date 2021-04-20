#include "stdafx.h"
#include "InputBox.h"
#include "ScriptSite.h"
#include "UTL_Conversion.h"

static HHOOK hHook;
static bool password = false;

void InputBox::NewLineToVBSNewLine(wstring &buff)
{
	size_t position = 0;
	while (position < buff.length())
	{
		if (buff[position] == '\r' || buff[position] == '\n')
			buff.replace(position, 2, _T("\" + vbNewLine + \""));

		position += 1;
	}
}

LRESULT CALLBACK InputBox::InputBoxCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < HC_ACTION)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (nCode == HCBT_ACTIVATE) {
		if (password == true) {
			HWND TextBox = FindWindowExA((HWND)wParam, nullptr, "Edit", nullptr);
			SendDlgItemMessageW((HWND)wParam, GetDlgCtrlID(TextBox), EM_SETPASSWORDCHAR, L'\x25cf', 0);
		}
	}

	if (nCode == HCBT_CREATEWND) {
		if (!(GetWindowLongPtr((HWND)wParam, GWL_STYLE) & WS_CHILD))
			SetWindowLongPtr((HWND)wParam, GWL_EXSTYLE, GetWindowLongPtr((HWND)wParam, GWL_EXSTYLE) | WS_EX_DLGMODALFRAME);
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void InputBox::InputBoxHelper(wstring title, wstring prompt, wstring def, bool pass, wstring& strResult)
{
	password = pass;

	HRESULT hr = CoInitialize(nullptr);
	CComPtr<IActiveScript> VBScript;
	CComPtr<IActiveScriptParse> VBScriptParse;
	ScriptSite* pScriptSite = new ScriptSite();
	hr = VBScript.CoCreateInstance(OLESTR("VBScript"));
	hr = VBScript->SetScriptSite(pScriptSite);
	hr = VBScript->QueryInterface(&VBScriptParse);
	hr = VBScriptParse->InitNew();

	UTL_Conversion::StringReplaceAll(title, _T("\""), _T("\"\""));
	UTL_Conversion::StringReplaceAll(prompt, _T("\""), _T("\"\""));
	UTL_Conversion::StringReplaceAll(prompt, _T("\\n"), _T("\n"));
	UTL_Conversion::StringReplaceAll(def, _T("\""), _T("\"\""));

	wstring evaluationString = _T("InputBox(\"") + prompt + _T("\", \"") + title + _T("\", \"") + def + _T("\")");
	NewLineToVBSNewLine(evaluationString);

	hr = pScriptSite->SetWindow(GetAncestor(GetActiveWindow(), GA_ROOTOWNER));

	HINSTANCE ModHwnd = GetModuleHandle(nullptr);
	DWORD ThreadID = GetCurrentThreadId();
	hHook = SetWindowsHookEx(WH_CBT, &InputBoxCallback, ModHwnd, ThreadID);

	CComVariant result;
	EXCEPINFO ei = {};
	hr = VBScriptParse->ParseScriptText(evaluationString.c_str(), nullptr, nullptr, nullptr, 0, 0, SCRIPTTEXT_ISEXPRESSION, &result, &ei);

	UnhookWindowsHookEx(hHook);
	VBScriptParse = nullptr;
	VBScript = nullptr;
	pScriptSite->Release();
	pScriptSite = nullptr;
	CoUninitialize();

	_bstr_t bstrResult = (_bstr_t)result;
	strResult = (wchar_t*)bstrResult;
}

void InputBox::Input(const wstring& prompt, const wstring& title, const wstring& def, wstring& strResult)
{
	return InputBoxHelper(prompt, title, def, false, strResult);
}

void InputBox::Password(const wstring& prompt, const wstring& title, const wstring& def, wstring& strResult)
{
	return InputBoxHelper(prompt, title, def, true, strResult);
}
