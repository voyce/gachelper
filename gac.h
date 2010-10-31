#pragma once

#include <tchar.h>
#include <fusion.h>

typedef HRESULT (__stdcall *CreateAsmCache)(IAssemblyCache**ppAsmCache, DWORD dwReserved);

class Utils
{
public:
	static bool IsInGAC(const wchar_t *szName, wchar_t *szPath, size_t cbPath)
	{
		HMODULE hfusion = GetModuleHandle(L"mscorwks.dll");
		if (hfusion == NULL)
		{
			OutputDebugString(L"Failed to get mscorwks.dll, trying clr.dll");
			hfusion = GetModuleHandle(L"clr.dll");
			if (hfusion == NULL)
			{
				OutputDebugString(L"Failed to get clr.dll");
				return false;
			}
		}
		CreateAsmCache pfnCreateAssemblyCache = (CreateAsmCache)GetProcAddress(hfusion,
			  "CreateAssemblyCache");
		if (pfnCreateAssemblyCache == NULL)
		{
			OutputDebugString(L"Failed to get function pointer");
			return false;
		}
		IAssemblyCache *gac;
		HRESULT hr = (pfnCreateAssemblyCache)(&gac, 0L);
		if (FAILED(hr))
		{
			OutputDebugString(L"CreateAssemblyCache failed");
			return false;
		}
		ASSEMBLY_INFO info;
		info.cbAssemblyInfo = sizeof(info);
		wchar_t wsz[512];
		info.pszCurrentAssemblyPathBuf = wsz;
		info.cchBuf = sizeof(wsz);
		hr = gac->QueryAssemblyInfo(QUERYASMINFO_FLAG_VALIDATE, szName, &info);
		if (FAILED(hr))
		{
			TCHAR sz[512];
			_stprintf_s(sz, _T("QueryAssemblyInfo failed for %s: %ld"), szName, hr);
			OutputDebugString(sz);
			gac->Release();
			return false;
		}
		OutputDebugString(info.pszCurrentAssemblyPathBuf);
		wcscpy_s(szPath, cbPath, info.pszCurrentAssemblyPathBuf);
		gac->Release();
		return true;
	}
};
