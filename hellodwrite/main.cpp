#include "pch.h"

#include "common.h"
#include "comutil.hpp"
#include "dwutil.hpp"

#include <iostream>
#include <functional>
#include <vector>

#include <clocale>

#include <tchar.h>

static constexpr ::LPCTSTR WINDOW_CLASS_NAME = _T("MyWindow");

::LRESULT CALLBACK WndProc(::HWND, ::UINT, ::WPARAM, ::LPARAM);

int _tmain(void) {
	::_tsetlocale(LC_ALL, _T(""));
	::_putts(_T("hello, world"));

	CoContext ctx;
	if (PrintError(_T("CoInitialize"), ctx.Result()))
		return -1;

	::HINSTANCE hInstance = reinterpret_cast<::HINSTANCE>(
		::GetModuleHandle(nullptr)
		);

	::WNDCLASS wndClass{ 0 };
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	::RegisterClass(&wndClass);

	::HWND hWnd = ::CreateWindow(
		WINDOW_CLASS_NAME,
		_T("Hello DWrite"),
		WS_OVERLAPPEDWINDOW | CS_HREDRAW | CS_VREDRAW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		600,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);

	::MSG msg;
	while (::GetMessage(&msg, hWnd, 0, 0) > 0) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

::LRESULT CALLBACK WndProc(::HWND hWnd, ::UINT msg, ::WPARAM wParam, ::LPARAM lParam) {
	static ::ID2D1Factory *d2dFactory;
	static ::IDWriteFactory *dwFactory;
	static ::ID2D1HwndRenderTarget *renderTarget;

	switch (msg) {
	case WM_CREATE: {
		if (PrintError(_T("D2D1CreateFactory"),
			::D2D1CreateFactory(::D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory))) {
			goto error;
		}
		if (PrintError(_T("DWriteCreateFactory"),
			::DWriteCreateFactory(::DWRITE_FACTORY_TYPE_SHARED, __uuidof(::IDWriteFactory),
				reinterpret_cast<::IUnknown **>(&dwFactory)))) {
			goto error;
		}

		::RECT rect;
		::GetClientRect(hWnd, &rect);
		if (PrintError(_T("CreateHwndRenderTarget"),
			d2dFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
				&renderTarget
			))) {
			goto error;
		}

		::com_ptr<::IDWriteRenderingParams> defaultParams(
			[](auto **x) {return dwFactory->CreateRenderingParams(x); }
		);

		::com_ptr<::IDWriteRenderingParams> params(
			[&defaultParams](auto **x) {
				return dwFactory->CreateCustomRenderingParams(
					defaultParams->GetGamma(),
					defaultParams->GetEnhancedContrast(),
					defaultParams->GetClearTypeLevel(),
					defaultParams->GetPixelGeometry(),
					DWRITE_RENDERING_MODE_NATURAL_SYMMETRIC,
					x
				);
			}
		);

		renderTarget->SetTextRenderingParams(params.get());
		renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		goto exit;
	}
	case WM_SIZE:
		renderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
		return 0;
	case WM_PAINT: {
		::PAINTSTRUCT ps;
		::BeginPaint(hWnd, &ps);
		renderTarget->BeginDraw();
		renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White, 1.0F));

		::com_ptr<::ID2D1SolidColorBrush> brush(
			[](auto **x) {
				return renderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Black, 1.0F), x);  }
		);

		::com_ptr<::IDWriteTextFormat> textFormat(
			[](auto **x) {
				return dwFactory->CreateTextFormat(
					L"",
					nullptr,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					100,
					L"en-us",
					x
				);
			}
		);

		if (brush.is_available() && textFormat.is_available()) {
			::CDWriteTextRenderer renderer(dwFactory, renderTarget, textFormat.get(), brush.get());
			renderer.Draw(L"hello\nこんにちは\n你好");
		}

		renderTarget->EndDraw();
		::EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DESTROY:
		renderTarget->Release();
		dwFactory->Release();
		d2dFactory->Release();
		::PostQuitMessage(0);
		return 0;

	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

error:
	::PostQuitMessage(-1);
exit:
	return 0;
}
