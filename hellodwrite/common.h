#ifndef COMMON_H_

#include <Windows.h>
#include <comdef.h>
#include <stdbool.h>

#ifdef __cplusplus
#define EXTERN_C_ extern "C"
#else
#define EXTERN_C_
#endif /* __cplusplus */

EXTERN_C_ void PrintErrorMessage(LPCTSTR, HRESULT);

EXTERN_C_ inline bool PrintError(LPCTSTR header, HRESULT result) {
	bool failed = FAILED(result);
	if (failed)
		PrintErrorMessage(header, result);
	return failed;
}

#endif /* COMMON_H_ */
