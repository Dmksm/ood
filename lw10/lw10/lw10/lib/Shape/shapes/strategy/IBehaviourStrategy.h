#pragma once
#include "../../gfx/ICanvas.h"
#include "../../gfx/Color.h"
#include "../../stdafx.h"
//придумать болле подходящее название
//нет виртуального деструктора
class IBehaviourStrategy
{
public:
	enum class ShapeType: int
	{
		Rectangle = 0,
		Triangle = 1,
		Ellipse = 2,
		Line = 3,
	};

	virtual void Move(double dx, double dy) = 0;
	virtual ShapeType GetType() = 0;
	virtual void SetFrame(RectD frame) = 0;
	virtual RectD GetFrame() const = 0;
	virtual ~IBehaviourStrategy() = default;
};