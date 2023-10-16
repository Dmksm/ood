#include "Shape.h"

CShape::CShape(Color color): m_color(color)
{
}

CShape::~CShape()
{
}

Color CShape::GetColor() const
{
	return m_color;
}