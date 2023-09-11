#pragma once
#include "Shape.h"
#include "EllipseDrawingStrategy.h"
#include "CircleObjextInfo.h"
#include <memory>

class Circle : public Shape
{
public:
	Circle(double x, double y, double radius, Color color)
		: Shape(std::make_shared<EllipseDrawingStrategy>(), color)
	{
		CircleObjectInfo objectInfo({ x, y }, radius);
		m_objectInfo = objectInfo;
	};

	void Draw(ICanvas& canvas) override
	{
		CircleObjectInfo objectInfo();
		m_drawingStrategy->Draw(canvas, objectInfo);
	}

private:
	CircleObjectInfo m_objectInfo;
};