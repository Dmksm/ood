#pragma once
#include <stdint.h>
#include "IVisualObjectInfo.h"
#include "EllipseDrawingStrategy.h"

class CircleObjectInfo: public IVisualObjectInfo
{
public:
	CircleObjectInfo(
		Point position,
		double radius,
		Color color
	): m_color(color)
	, m_position(position)
	, m_radius(radius)
	{
	};

	double GetRadius()
	{
		return m_radius;
	};

	Point GetPosition() override
	{
		return m_position;
	};

	Color GetColor() override
	{
		return m_color;
	};

private:
	Point m_position;
	double m_radius;
	Color m_color;
};