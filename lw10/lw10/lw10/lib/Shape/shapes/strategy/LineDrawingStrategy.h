#pragma once
#include "IDrawingStrategy.h"

class LineDrawingStrategy : public IDrawingStrategy
{
public:
	LineDrawingStrategy(double x1, double y1, double x2, double y2)
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
	{
	}

	void Move(double dx, double dy) override
	{
		m_x1 += dx;
		m_y1 += dy;
		m_x2 += dx;
		m_y2 += dy;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.DrawLine(m_x1, m_y1, m_x2, m_y2);
	};

	std::string GetStrategyParams() override
	{
		std::stringstream ss;
		ss << "line " << m_x1 << " " << m_y1 << " "
			<< m_x2 << " " << m_y2;
		return ss.str();
	};

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;
};