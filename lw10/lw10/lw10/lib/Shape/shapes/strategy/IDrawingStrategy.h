#pragma once
#include "../../gfx/ICanvas.h"
#include "../../gfx/Color.h"
#include <string>
#include <sstream>
#include <iostream>

class IDrawingStrategy
{
public:
	virtual void Move(double dx, double dy) = 0;
	virtual std::string GetStrategyParams() = 0;
	virtual void Draw(ICanvas& canvas, Color color) = 0;
};