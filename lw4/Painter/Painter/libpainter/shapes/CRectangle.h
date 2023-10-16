#pragma once
#include "Shape.h"

class CRectangle : public CShape
{
public:
	CRectangle(Color color, double left, double top, double width, double height)
		: CShape(color)
		, m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(this->GetColor());
		canvas.DrawRectangle(m_left, m_top, m_width, m_height);
	};

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};