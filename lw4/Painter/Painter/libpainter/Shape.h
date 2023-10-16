#pragma once
#include "gfx/ICanvas.h"

class CShape
{
public:
	CShape(Color color);
	virtual void Draw(ICanvas canvas) const = 0;
	Color GetColor() const;
	virtual ~CShape();
private:
	Color m_color;
};