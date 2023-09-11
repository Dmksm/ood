#pragma once
#include "IDrawingStrategy.h"

class RectangleDrawingStrategy : public IDrawingStrategy
{
public:
	void Draw(ICanvas& canvas, IVisualObjectInfo& objectInfo) override
	{
		canvas.DrawRectangle(objectInfo.GetPosition(), objectInfo.GetColor(), objectInfo.GetBounds());
	};
};