#pragma once
#include "CommonTypes.h"
#include "../stdafx.h"

class ICanvas
{
public:
	virtual void SetLineColor(RGBAColor color) = 0;
	virtual void BeginFill(RGBAColor color) = 0;
	virtual void EndFill() = 0;
	virtual void SetThickness(double thickness) = 0;
	virtual void MoveTo(double x, double y) = 0;
	virtual void LineTo(double x, double y) = 0;
	virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;

	virtual ~ICanvas() = default;
};