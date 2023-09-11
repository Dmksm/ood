#pragma once
#include "IVisualObjectInfo.h"
class ICanvas
{
public:
	void virtual DrawElipse(
		IVisualObjectInfo::Point position,
		IVisualObjectInfo::Color color, 
		IVisualObjectInfo::Point bounds
	) = 0;
	void virtual DrawLine(
		IVisualObjectInfo::Point position,
		IVisualObjectInfo::Color color
	) = 0;
	void virtual DrawRectangle(
		IVisualObjectInfo::Point position,
		IVisualObjectInfo::Color color,
		IVisualObjectInfo::Point bounds
	) = 0;
	~ICanvas() = default;
};