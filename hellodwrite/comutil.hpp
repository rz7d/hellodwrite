#pragma once

#include <Windows.h>
#include <comdef.h>

#include <functional>

template<typename T>
class com_ptr final {
private:
	T *m_ptr{};

public:
	com_ptr(std::function<::HRESULT(T **)> initializer) {
		if (::PrintError(TEXT(""), initializer(&m_ptr))) {
			m_ptr = nullptr;
		}
	}

	com_ptr(const com_ptr &) = delete;
	com_ptr &operator=(const com_ptr &) = delete;

	com_ptr(com_ptr &&) = default;
	com_ptr &operator=(com_ptr &&) = default;

	~com_ptr() {
		if (m_ptr) {
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}

	inline T *get() {
		return m_ptr;
	}

	inline bool is_available() {
		return m_ptr != nullptr;
	}

	inline T *operator->() noexcept {
		return m_ptr;
	}

	inline const T *operator->() const noexcept {
		return m_ptr;
	}

};

class CoContext final {
private:
	::HRESULT m_result;

public:
	CoContext();
	~CoContext();
	::HRESULT Result();
};
