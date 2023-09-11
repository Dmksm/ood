#pragma once
#include <stdint.h>

class IVisualObjectInfo
{
public:
	using Color = uint32_t;
	struct Point
	{
		double x;
		double y;
	};

	virtual Point GetPosition() = 0;
	virtual Color GetColor() = 0;
};