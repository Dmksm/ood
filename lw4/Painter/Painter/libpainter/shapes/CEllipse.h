#pragma once
#include "Shape.h"

class CEllipse: public CShape
{
public:
	CEllipse(Color color, double cx, double cy, double rx, double ry)
		: CShape(color)
		, m_cx(cx)
		, m_cy(cy)
		, m_rx(rx)
		, m_ry(ry)
	{
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(this->GetColor());
		canvas.DrawEllipse(m_cx, m_cy, m_rx, m_ry);
	};

private:
	double m_cx;
	double m_cy;
	double m_rx;
	double m_ry;
};