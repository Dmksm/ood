#pragma once
#include "Shape.h"

class CTriangle : public CShape
{
public:
	CTriangle(Color color, double x1, double y1, double x2, double y2, double x3, double y3)
		: CShape(color)
		, m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
		, m_x3(x3)
		, m_y3(y3)
	{
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(this->GetColor());
		canvas.DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3);
	};

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2; 
	double m_x3;
	double m_y3;
};