#pragma once
#include "IDrawingStrategy.h"

class EllipseDrawingStrategy: public IDrawingStrategy
{
public:
	void Draw(ICanvas& canvas, IVisualObjectInfo& objectInfo) override
	{
		canvas.DrawElipse(objectInfo.GetPosition(), objectInfo.GetColor(), objectInfo.GetBounds());
	};
};