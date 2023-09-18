#pragma once
#include "../gfx/ICanvas.h"
#include "../gfx/Color.h"
#include <string>
#include <iostream>

class IDrawingStrategy
{
public:
	virtual ~IDrawingStrategy() = default;
	virtual void Move(double dx, double dy) = 0;
	virtual void Display(std::string id, Color color) = 0;
	virtual void Draw(ICanvas& canvas, Color color) = 0;
};