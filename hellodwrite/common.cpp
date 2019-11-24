#include "pch.h"

#include "common.h"

#include <tchar.h>

EXTERN_C_ void PrintErrorMessage(::LPCTSTR header, ::HRESULT result) {
	::_ftprintf(stderr,
#ifdef _UNICODE
		_T("%ls")
#else
		_T("%s")
#endif
		_T("\n"),
		header);

	::LPTSTR buffer;
	if (::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		reinterpret_cast<::LPTSTR>(&buffer),
		0,
		nullptr))
	{
		::_ftprintf(stderr,
			_T("HRESULT: ")
#ifdef _UNICODE
			_T("%ls")
#else
			_T("%s")
#endif
			_T("\n"),
			buffer);
	}
	else {
		::_ftprintf(stderr, _T("HRESULT: %llx\n"), (unsigned long long) result);
	}
}
