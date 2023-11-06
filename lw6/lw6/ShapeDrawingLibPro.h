#pragma once
#include "stdafx.h"
#include "GraphicsLibPro.h"

// Пространство имен библиотеки для рисования фигур (использует graphics_lib)
// Код библиотеки недоступен для изменения
namespace shape_drawing_lib_pro
{
	struct Point
	{
		int x;
		int y;
	};

	// Интерфейс объектов, которые могут быть нарисованы на холсте из graphics_lib
	class ICanvasDrawable
	{
	public:
		virtual void Draw(graphics_lib_pro::ICanvas& canvas)const = 0;
		virtual ~ICanvasDrawable() = default;
	};

	class CTriangle : public ICanvasDrawable
	{
	public:
		CTriangle(const Point& p1, const Point& p2, const Point& p3, uint32_t color = 0x000000)
			: m_p1(p1)
			, m_p2(p2)
			, m_p3(p3)
			, m_color(color)
		{
		}

		void Draw(graphics_lib_pro::ICanvas& canvas)const override
		{
			canvas.SetColor(m_color);
			canvas.MoveTo(m_p1.x, m_p1.y);
			canvas.LineTo(m_p2.x, m_p2.y);
			canvas.LineTo(m_p3.x, m_p3.y);
			canvas.LineTo(m_p1.x, m_p1.y);
		}
	private:
		Point m_p1;
		Point m_p2;
		Point m_p3;
		uint32_t m_color;
	};

	class CRectangle : public ICanvasDrawable
	{
	public:
		CRectangle(const Point& leftTop, int width, int height, uint32_t color = 0x000000)
			: m_leftTop(leftTop)
			, m_width(width)
			, m_height(height)
			, m_color(color)
		{
		}

		void Draw(graphics_lib_pro::ICanvas& canvas)const override
		{
			int leftX = m_leftTop.x;
			int leftY = m_leftTop.y;
			canvas.SetColor(m_color);
			canvas.MoveTo(leftX, leftY);
			canvas.LineTo(leftX + m_width, leftY);
			canvas.LineTo(leftX + m_width, leftY + m_height);
			canvas.LineTo(leftX, leftY + m_height);
			canvas.LineTo(leftX, leftY);
		}
	private:
		Point m_leftTop;
		int m_width;
		int m_height;
		uint32_t m_color;
	};

	// Художник, способный рисовать ICanvasDrawable-объекты на ICanvas
	class CCanvasPainter
	{
	public:
		CCanvasPainter(graphics_lib_pro::ICanvas& canvas) : m_canvas(canvas)
		{
		}

		void Draw(const ICanvasDrawable& drawable)
		{
			drawable.Draw(m_canvas);
		}
	private:
		graphics_lib_pro::ICanvas& m_canvas;
	};
}