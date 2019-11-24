#include "pch.h"

#include "comutil.hpp"

CoContext::CoContext() {
	m_result = CoInitialize(nullptr);
}

CoContext::~CoContext() {
	CoUninitialize();
}

::HRESULT CoContext::Result() {
	return m_result;
}
