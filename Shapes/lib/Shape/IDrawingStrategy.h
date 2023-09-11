#pragma once
#include "ICanvas.h"
#include "IVisualObjectInfo.h"

class IDrawingStrategy
{
public:
	virtual ~IDrawingStrategy() = default;
	virtual void Draw(ICanvas& canvas, IVisualObjectInfo& objectInfo) = 0;
};