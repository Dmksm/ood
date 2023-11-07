#pragma once
#include "stdafx.h"
#include "ShapeDrawingLib.h"
#include "GraphicsLib.h"
#include "ModernGraphicsLib.h"

// Пространство имен приложения (доступно для модификации)
namespace app
{
	class CCanvasClassAdapter
		: public graphics_lib::ICanvas
		, private modern_graphics_lib::CModernGraphicsRenderer
	{
	public:
		CCanvasClassAdapter(std::ostream& strm)
			: modern_graphics_lib::CModernGraphicsRenderer(strm)
		{
			BeginDraw();
		}

		void MoveTo(int x, int y) override
		{
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
			updateCurrentPosition(x, y);
		}
	private:
		int m_startX = 0;
		int m_startY = 0;
		int m_currX = 0;
		int m_currY = 0;

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

		void BeginDraw()
		{
			m_adaptee.BeginDraw();
		}

		void MoveTo(int x, int y) override
		{
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
			updateCurrentPosition(x, y);
		}

		void EndDraw()
		{
			m_adaptee.EndDraw();
		}
	private:
		int m_startX = 0;
		int m_startY = 0;
		int m_currX = 0;
		int m_currY = 0;
		modern_graphics_lib::CModernGraphicsRenderer& m_adaptee;

		void updateCurrentPosition(int x, int y)
		{
			m_currX = x;
			m_currY = y;
		}
	};

	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		shape_drawing_lib::CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
		shape_drawing_lib::CRectangle rectangle({ 30, 40 }, 18, 24);

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
		canvasAdapter.BeginDraw();
		PaintPicture(painterWithAdapter);
		canvasAdapter.EndDraw();

		std::cout << "..............................." << std::endl;
		CCanvasClassAdapter canvasClassAdapter(std::cout);
		shape_drawing_lib::CCanvasPainter painterWithClassAdapter(canvasClassAdapter);
		PaintPicture(painterWithClassAdapter);
	}
}