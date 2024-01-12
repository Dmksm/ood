#pragma once
#include "CommonTypes.h"
#include "../stdafx.h"
#include "Color.h"

class IView
{
public:
	//почему зависимость от модельных
	enum class ShapeType : int
	{
		Rectangle = 1,
		Triangle = 2,
		Ellipse = 3,
	};

	virtual void MoveTo(double x, double y) = 0;
	virtual void SetColor(Color color) = 0;
	virtual void LineTo(double x, double y) = 0;
	virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
	virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;
	virtual void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) = 0;
	virtual void DrawRectangle(double left, double top, double width, double height) = 0;
	virtual void DrawLine(double x1, double y1, double x2, double y2) = 0;
	//отображение виджетов не в канвас
	virtual void DrawFrame(RectD frame) = 0;
	virtual void DrawWidgetPanel() = 0;
	virtual std::vector<RectD> GetSelectionMarkerFrame(RectD frame) = 0;
	virtual RectD GetWidgetFrame(ShapeType type) = 0;
	virtual RectD GetWorkSpaceFrameBorder() = 0;
	virtual void Display() = 0;
	virtual ~IView() = default;
};