#pragma once
#include "IDrawingStrategy.h"

class RectangleDrawingStrategy : public IDrawingStrategy
{
public:
	RectangleDrawingStrategy(double left, double top, double width, double height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Move(double dx, double dy) override
	{
		m_left += dx;
		m_top += dy;
	}


	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.MoveTo(m_left, m_top);
		canvas.LineTo(m_left + m_width, m_top);
		canvas.LineTo(m_left + m_width, m_top + m_height);
		canvas.LineTo(m_left, m_top + m_height);
		canvas.LineTo(m_left, m_top);
	};

	void Display(std::string id, Color color) override
	{
		std::cout << "rectangle " << id << " " << color << " " << m_left << " " << m_top << " " 
			<< m_width << " " << m_height;
	};

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};