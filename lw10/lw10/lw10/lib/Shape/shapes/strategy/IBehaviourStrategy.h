#pragma once
#include "../../gfx/ICanvas.h"
#include "../../gfx/Color.h"
#include "../../stdafx.h"
//придумать болле подходящее название
//нет виртуального деструктора
class IBehaviourStrategy
{
public:
	virtual void Move(double dx, double dy) = 0;
	virtual std::string GetType() = 0;
	virtual void SetFrame(RectD frame) = 0;
	virtual RectD GetFrame() const = 0;
	virtual ~IBehaviourStrategy() = default;
};