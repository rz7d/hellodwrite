#include "pch.h"

#include "common.h"

#include <tchar.h>

EXTERN_C_ void PrintErrorMessage(LPCTSTR header, HRESULT result) {
	_ftprintf(stderr, _T("%ls\n"), header);
	_com_error error(result);
#ifdef _UNICODE
	_ftprintf(stderr, _T("HRESULT: %ls\n"), error.ErrorMessage());
#else
	_ftprintf(stderr, _T("HRESULT: %s\n"), error.ErrorMessage());
#endif
}
