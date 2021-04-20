#pragma once

#include "stdafx.h"

class ScriptSite : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	ScriptSite();

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);

	STDMETHOD(SetWindow)(HWND hWnd);
	STDMETHOD(GetWindow)(HWND* phWnd);
	STDMETHOD(EnableModeless)(BOOL);

	STDMETHOD(GetLCID)(LCID* plcid);
	STDMETHOD(GetItemInfo)(LPCOLESTR, DWORD, IUnknown**, ITypeInfo**);
	STDMETHOD(GetDocVersionString)(BSTR* pbstrVersion);
	STDMETHOD(OnScriptTerminate)(const VARIANT*, const EXCEPINFO*);
	STDMETHOD(OnStateChange)(SCRIPTSTATE);
	STDMETHOD(OnScriptError)(IActiveScriptError*);
	STDMETHOD(OnEnterScript)();
	STDMETHOD(OnLeaveScript)();

public:
	LONG m_Count;
	HWND m_hWnd;
};
