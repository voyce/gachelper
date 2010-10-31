// gachelper.h

#pragma once
#include "gac.h"
#include <vcclr.h>

using namespace System;

namespace gachelper {

	public ref class GAC
	{
	public:
		static bool TryGetFullPath(String ^name, String ^%fullPath)
		{
			wchar_t sz[1024];
			pin_ptr<const wchar_t> wszName = PtrToStringChars(name);
			if (Utils::IsInGAC(wszName, sz, _countof(sz)))
			{
				fullPath = gcnew String(sz);
				return true;
			}
			else
				return false;
		}
	};
}
