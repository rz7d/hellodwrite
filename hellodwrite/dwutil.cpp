#include "pch.h"

#include "dwutil.hpp"

void CDWriteTextRenderer::Draw(PCWSTR text) {
	auto size = m_renderTarget->GetSize();
	m_renderTarget->DrawText(
		text,
		static_cast<UINT>(::wcslen(text)),
		m_textFormat,
		D2D1::RectF(10.0F, 10.0F, size.width - 10.0F, size.height - 10.0F),
		m_brush
	);
}
