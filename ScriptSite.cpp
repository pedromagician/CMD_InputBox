#include "ScriptSite.h"

ScriptSite::ScriptSite() : m_Count(1), m_hWnd(nullptr)
{
}

STDMETHODIMP_(ULONG) ScriptSite::AddRef()
{
	return InterlockedIncrement(&m_Count);
}

STDMETHODIMP_(ULONG) ScriptSite::Release()
{
	if (!InterlockedDecrement(&m_Count))
	{
		delete this;
		return 0;
	}
	return m_Count;
}

STDMETHODIMP ScriptSite::QueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IActiveScriptSiteWindow) {
		*ppvObject = (IActiveScriptSiteWindow*)this;
		AddRef();
		return NOERROR;
	}
	if (riid == IID_IActiveScriptSite) {
		*ppvObject = (IActiveScriptSite*)this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP ScriptSite::SetWindow(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

STDMETHODIMP ScriptSite::GetWindow(HWND* phWnd)
{
	*phWnd = m_hWnd;
	return S_OK;
}

STDMETHODIMP ScriptSite::EnableModeless(BOOL)
{
	return S_OK;
}

STDMETHODIMP ScriptSite::GetLCID(LCID* plcid)
{
	*plcid = 0;
	return S_OK;
}

STDMETHODIMP ScriptSite::GetItemInfo(LPCOLESTR, DWORD, IUnknown**, ITypeInfo**)
{
	return TYPE_E_ELEMENTNOTFOUND;
}

STDMETHODIMP ScriptSite::GetDocVersionString(BSTR* pbstrVersion)
{
	*pbstrVersion = SysAllocString(L"1.0");
	return S_OK;
}

STDMETHODIMP ScriptSite::OnScriptTerminate(const VARIANT*, const EXCEPINFO*)
{
	return S_OK;
}

STDMETHODIMP ScriptSite::OnStateChange(SCRIPTSTATE)
{
	return S_OK;
}

STDMETHODIMP ScriptSite::OnScriptError(IActiveScriptError*)
{
	return S_OK;
}

STDMETHODIMP ScriptSite::OnEnterScript()
{
	return S_OK;
}

STDMETHODIMP ScriptSite::OnLeaveScript()
{
	return S_OK;
}
