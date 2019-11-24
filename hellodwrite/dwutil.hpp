#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

class CDWriteTextRenderer final {
private:
	IDWriteFactory *m_factory;
	ID2D1RenderTarget *m_renderTarget;
	IDWriteTextFormat *m_textFormat;
	ID2D1Brush *m_brush;

public:
	CDWriteTextRenderer(
		IDWriteFactory *factory,
		ID2D1RenderTarget *renderTarget,
		IDWriteTextFormat *textFormat,
		ID2D1Brush *brush
	) : m_factory(factory),
		m_renderTarget(renderTarget),
		m_textFormat(textFormat),
		m_brush(brush) {}

	void Draw(PCWSTR text);

};

