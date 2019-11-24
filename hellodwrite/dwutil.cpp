#include "pch.h"

#include "dwutil.hpp"

void CDWriteTextRenderer::Draw(PCWSTR text) {
	auto size = m_renderTarget->GetSize();
	m_renderTarget->DrawText(
		text,
		static_cast<UINT>(::wcslen(text)),
		m_textFormat,
		D2D1::RectF(50.0f, 50.0f, 50.0f + size.width, 50.0f + size.height),
		m_brush
	);
}
