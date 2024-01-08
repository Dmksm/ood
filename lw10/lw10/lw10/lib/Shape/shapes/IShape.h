#pragma once
#include "../gfx/ICanvas.h"
#include "../gfx/Color.h"
#include "strategy/IDrawingStrategy.h"
#include "../stdafx.h"

class IShape
{
public:
	virtual void SetFrame(RectD frame) = 0;
	virtual RectD GetFrame() const = 0;
	virtual void SetDrawingStrategy(std::unique_ptr<IDrawingStrategy>&& drawingStrategy) = 0;
	virtual void Move(double dx, double dy) const = 0;
	virtual void Draw(ICanvas& canvas) const = 0;
	virtual void SetColor(Color color) = 0;
	virtual std::string GetId() const = 0;
	virtual Color GetColor() const = 0;
	virtual std::string GetStrategyParams() const = 0;
	virtual ~IShape() = default;
};