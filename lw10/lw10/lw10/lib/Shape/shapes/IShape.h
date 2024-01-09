#pragma once
#include "../gfx/ICanvas.h"
#include "../gfx/Color.h"
#include "strategy/IBehaviourStrategy.h"
#include "../stdafx.h"

class IShape
{
public:
	virtual void SetFrame(RectD frame) = 0;
	virtual RectD GetFrame() const = 0;
	virtual void SetBehaviourStrategy(std::unique_ptr<IBehaviourStrategy>&& drawingStrategy) = 0;
	virtual void Move(double dx, double dy) const = 0;
	virtual void SetColor(Color color) = 0;
	virtual std::string GetId() const = 0;
	virtual Color GetColor() const = 0;
	virtual std::string GetType() const = 0;
	virtual ~IShape() = default;
};