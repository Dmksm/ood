#pragma once
#include "stdafx.h"
#include "ModernGraphicsLibPro.h"
#include "ShapeDrawingLibPro.h"
#include "GraphicsLibPro.h"

// Пространство имен приложения (доступно для модификации)
namespace app_pro
{
	class CCanvasClassAdapter
		: public graphics_lib_pro::ICanvas
		, private modern_graphics_lib_pro::CModernGraphicsRenderer
	{
	public:
		CCanvasClassAdapter(std::ostream& strm)
			: modern_graphics_lib_pro::CModernGraphicsRenderer(strm)
		{
			BeginDraw();
		}

		void SetColor(uint32_t rgbColor) override
		{
			m_color = ToRGBA(rgbColor);
		}

		void MoveTo(int x, int y) override
		{
			m_currX = x;
			m_currY = y;
		}

		void LineTo(int x, int y) override
		{
			DrawLine(
				modern_graphics_lib_pro::CPoint(m_currX, m_currY),
				modern_graphics_lib_pro::CPoint(x, y),
				m_color
			);
			m_currX = x;
			m_currY = y;
		}

	private:
		modern_graphics_lib_pro::CRGBAColor m_color = { 0.0, 0.0, 0.0, 0.0 };
		int m_currX = 0;
		int m_currY = 0;

		static modern_graphics_lib_pro::CRGBAColor ToRGBA(uint32_t rgbColor)
		{
			const int MAX_COLOR_VALUE = 0xff;
			const float MAX_RGBA_COLOR_VALUE = 1.0f;
			const float CONVERSION_FACTOR = MAX_RGBA_COLOR_VALUE / MAX_COLOR_VALUE;

			float red = ((rgbColor >> 16) & MAX_COLOR_VALUE) * CONVERSION_FACTOR;
			float green = ((rgbColor >> 8) & MAX_COLOR_VALUE) * CONVERSION_FACTOR;
			float blue = (rgbColor & MAX_COLOR_VALUE) * CONVERSION_FACTOR;

			return modern_graphics_lib_pro::CRGBAColor(red, green, blue, MAX_RGBA_COLOR_VALUE);
		}
	};

	class CCanvasAdapter : public graphics_lib_pro::ICanvas
	{
	public:
		CCanvasAdapter(
			modern_graphics_lib_pro::CModernGraphicsRenderer& modernGraphicsRenderer
		)
			: m_adaptee(modernGraphicsRenderer)
		{
		}

		void SetColor(uint32_t rgbColor) override
		{
			m_color = ToRGBA(rgbColor);
		}

		void MoveTo(int x, int y) override
		{
			m_currX = x;
			m_currY = y;
		}

		void LineTo(int x, int y) override
		{
			m_adaptee.DrawLine(
				modern_graphics_lib_pro::CPoint(m_currX, m_currY),
				modern_graphics_lib_pro::CPoint(x, y),
				m_color
			);
			m_currX = x;
			m_currY = y;
		}
	private:
		modern_graphics_lib_pro::CRGBAColor m_color = { 0.0, 0.0, 0.0, 0.0 };
		int m_currX = 0;
		int m_currY = 0;
		modern_graphics_lib_pro::CModernGraphicsRenderer& m_adaptee;

		static modern_graphics_lib_pro::CRGBAColor ToRGBA(uint32_t rgbColor)
		{
			const int MAX_COLOR_VALUE = 0xff;
			const float MAX_RGBA_COLOR_VALUE = 1.0f;
			const float CONVERSION_FACTOR = MAX_RGBA_COLOR_VALUE / MAX_COLOR_VALUE;

			float red = ((rgbColor >> 16) & MAX_COLOR_VALUE) * CONVERSION_FACTOR;
			float green = ((rgbColor >> 8) & MAX_COLOR_VALUE) * CONVERSION_FACTOR;
			float blue = (rgbColor & MAX_COLOR_VALUE) * CONVERSION_FACTOR;

			return modern_graphics_lib_pro::CRGBAColor(red, green, blue, MAX_RGBA_COLOR_VALUE);
		}
	};

	void PaintPicture(shape_drawing_lib_pro::CCanvasPainter& painter)
	{
		shape_drawing_lib_pro::CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 }, 256 - 1);
		shape_drawing_lib_pro::CRectangle rectangle({ 30, 40 }, 18, 24, 256 * 256 - 1);

		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib_pro::CCanvas simpleCanvas;
		shape_drawing_lib_pro::CCanvasPainter painter(simpleCanvas);
		PaintPicture(painter);
	}

	void PaintPictureOnModernGraphicsRenderer()
	{
		modern_graphics_lib_pro::CModernGraphicsRenderer renderer(std::cout);
		CCanvasAdapter canvasAdapter(renderer);
		shape_drawing_lib_pro::CCanvasPainter painterWithAdapter(canvasAdapter);
		renderer.BeginDraw();
		PaintPicture(painterWithAdapter);
		renderer.EndDraw();

		std::cout << "..............................." << std::endl;
		CCanvasClassAdapter canvasClassAdapter(std::cout);
		shape_drawing_lib_pro::CCanvasPainter painterWithClassAdapter(canvasClassAdapter);
		PaintPicture(painterWithClassAdapter);
	}
}