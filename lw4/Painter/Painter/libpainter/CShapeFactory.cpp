#include "CShapeFactory.h"
#include "shapes/CEllipse.h"
#include "shapes/CRectangle.h"
#include "shapes/CTriangle.h"
#include "shapes/CRegularPolygon.h"
#include "../stdafx.h"

Color GetColorEnum(const std::string& color)
{
	if (color == "green")
	{
		return Color::Green;
	}
	else if (color == "red")
	{
		return Color::Red;
	}
	else if (color == "blue")
	{
		return Color::Blue;
	}
	else if (color == "yellow")
	{
		return Color::Yellow;
	}
	else if (color == "pink")
	{
		return Color::Pink;
	}
	else if (color == "black")
	{
		return Color::Black;
	}
	else
	{
		throw std::logic_error((std::string)("Unknown color! Given: ") + color);
	}
}

std::unique_ptr<CShape> GetEllipse(Color color, std::stringstream& args)
{
	double cx, cy, rx, ry;
	args >> cx >> cy >> rx >> ry;
	return std::make_unique<CShape>(CEllipse(color, cx, cy, rx, ry));
}

std::unique_ptr<CShape> GetTriangle(Color color, std::stringstream& args)
{
	double x1, y1, x2, y2, x3, y3;
	args >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
	return std::make_unique<CShape>(CTriangle(color, x1, y1, x2, y2, x3, y3));
}

std::unique_ptr<CShape> GetRectangle(Color color, std::stringstream& args)
{
	double left, top, width, height;
	args >> left >> top >> width >> height;
	return std::make_unique<CShape>(CRectangle(color, left, top, width, height));
}

std::unique_ptr<CShape> GetRegularPolygon(Color color, std::stringstream& args)
{
	double radius, vertexCount;
	args >> radius >> vertexCount;
	return std::make_unique<CShape>(CRegularPolygon(color, radius, vertexCount));
}

std::unique_ptr<CShape> CShapeFactory::CreateShape(const std::string& description)
{
	std::stringstream descriptionStream(description);
	std::string shapeType;
	std::string color;
	descriptionStream >> shapeType >> color;
	if (shapeType == "ellipse")
	{
		return GetEllipse(GetColorEnum(color), descriptionStream);
	}
	else if (shapeType == "triangle")
	{
		return GetTriangle(GetColorEnum(color), descriptionStream);
	}
	else if (shapeType == "rectangle")
	{
		return GetRectangle(GetColorEnum(color), descriptionStream);
	}
	else if (shapeType == "regularPolygon")
	{
		return GetRegularPolygon(GetColorEnum(color), descriptionStream);
	}
	else
	{
		throw std::logic_error((std::string)("Unknown shape type! Given: ") + shapeType);
	}
}