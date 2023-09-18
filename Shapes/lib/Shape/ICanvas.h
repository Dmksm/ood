#pragma once
#include <string>
#include "Color.h"

class ICanvas
{
public:
	virtual void MoveTo(double x, double y) = 0;
	virtual void SetColor(Color color) = 0;
	virtual void LineTo(double x, double y) = 0;
	virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
	virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;
	virtual void Display() = 0;
};