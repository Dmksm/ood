#pragma once
#include "../Shape.h"

class CRegularPolygon : public CShape
{
public:
	CRegularPolygon(Color color, double radius, double vertexCount)
		: CShape(color)
		, m_radius(radius)
		, m_vertexCount(vertexCount)
	{
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(this->GetColor());
		canvas.DrawRegularPolygon(m_radius, m_vertexCount);
	};

private:
	double m_radius;
	double m_vertexCount;
};