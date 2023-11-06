#pragma once
#include "stdafx.h"
#include "modern_graphics_lib_pro.h"
#include "shape_drawing_lib.h"
#include "graphics_lib.h"
#include "modern_graphics_lib.h"
#include "graphics_lib_pro.h"

// Пространство имен приложения (доступно для модификации)
namespace app
{
	class CCanvasClassAdapter
		: public graphics_lib::ICanvas
		, public modern_graphics_lib::CModernGraphicsRenderer
	{
	public:
		CCanvasClassAdapter(std::ostream& strm)
			: modern_graphics_lib::CModernGraphicsRenderer(strm)
		{
		}

		void MoveTo(int x, int y) override
		{
			BeginDraw();
			m_startX = x;
			m_startY = y;

			updateCurrentPosition(x, y);
		}

		void LineTo(int x, int y) override
		{
			DrawLine(
				modern_graphics_lib::CPoint(m_currX, m_currY),
				modern_graphics_lib::CPoint(x, y)
			);
			if (m_startX == x && m_startY == y)
			{
				EndDraw();
			}
			updateCurrentPosition(x, y);
		}
	private:
		int m_startX;
		int m_startY;
		int m_currX;
		int m_currY;

		void updateCurrentPosition(int x, int y)
		{
			m_currX = x;
			m_currY = y;
		}
	};

	class CCanvasAdapter : public graphics_lib::ICanvas
	{
	public:
		CCanvasAdapter(
			modern_graphics_lib::CModernGraphicsRenderer& modernGraphicsRenderer
		)
			: m_adaptee(modernGraphicsRenderer)
		{
		}

		void MoveTo(int x, int y) override
		{
			m_adaptee.BeginDraw();
			m_startX = x;
			m_startY = y;

			updateCurrentPosition(x, y);
		}

		void LineTo(int x, int y) override
		{
			m_adaptee.DrawLine(
				modern_graphics_lib::CPoint(m_currX, m_currY),
				modern_graphics_lib::CPoint(x, y)
			);
			if (m_startX == x && m_startY == y)
			{
				m_adaptee.EndDraw();
			}
			updateCurrentPosition(x, y);
		}
	private:
		int m_startX;
		int m_startY;
		int m_currX;
		int m_currY;
		modern_graphics_lib::CModernGraphicsRenderer& m_adaptee;

		void updateCurrentPosition(int x, int y)
		{
			m_currX = x;
			m_currY = y;
		}
	};

	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		using namespace shape_drawing_lib;

		CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
		CRectangle rectangle({ 30, 40 }, 18, 24);

		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib::CCanvas simpleCanvas;
		shape_drawing_lib::CCanvasPainter painter(simpleCanvas);
		PaintPicture(painter);
	}

	void PaintPictureOnModernGraphicsRenderer()
	{
		modern_graphics_lib::CModernGraphicsRenderer renderer(std::cout);
		CCanvasAdapter canvasAdapter(renderer);
		shape_drawing_lib::CCanvasPainter painterWithAdapter(canvasAdapter);
		PaintPicture(painterWithAdapter);

		std::cout << "..............................." << std::endl;
		CCanvasClassAdapter canvasClassAdapter(std::cout);
		shape_drawing_lib::CCanvasPainter painterWithClassAdapter(canvasClassAdapter);
		PaintPicture(painterWithClassAdapter);
	}
}