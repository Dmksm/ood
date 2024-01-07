#pragma once
#include "IDrawingStrategy.h"

class EllipseDrawingStrategy: public IDrawingStrategy
{
public:
	EllipseDrawingStrategy(double cx, double cy, double rx, double ry)
		: m_cx(cx)
		, m_cy(cy)
		, m_rx(rx)
		, m_ry(ry)
	{
	}

	void Move(double dx, double dy) override
	{
		m_cx += dx;
		m_cy += dy;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.DrawEllipse(m_cx, m_cy, m_rx, m_ry);
	};

	std::string GetStrategyParams() override
	{
		std::stringstream ss;
		ss << "ellipse " << m_cx << " " << m_cy << " " << m_rx
			<< " " << m_ry;
		return ss.str();
	};

private:
	double m_cx;
	double m_cy;
	double m_rx;
	double m_ry;
};