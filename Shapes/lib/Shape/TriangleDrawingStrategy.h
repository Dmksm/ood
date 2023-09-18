#pragma once
#include "IDrawingStrategy.h"

class TriangleDrawingStrategy : public IDrawingStrategy
{
public:
	TriangleDrawingStrategy(double x1, double y1, double x2, double y2, double x3, double y3)
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
		, m_x3(x3)
		, m_y3(y3)
	{
	}

	void Move(double dx, double dy) override
	{
		m_x1 += dx;
		m_y1 += dy;
		m_x2 += dx;
		m_y2 += dy;
		m_x3 += dx;
		m_y3 += dy;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3);
	};

	void Display(std::string id, Color color) override
	{
		std::cout << "triangle " << id << " " << color << " " << m_x1 << " " << m_y1 
			<< " " << m_x2 << " " << m_y2 << " " << m_x3 << " " << m_y3;
	};

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2; 
	double m_x3;
	double m_y3;
};