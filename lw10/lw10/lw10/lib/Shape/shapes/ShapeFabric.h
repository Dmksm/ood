#pragma once
#include "../stdafx.h"
#include "../shapes/Picture.h"

class CShapeFabric
{
public:
	std::unique_ptr<IDrawingStrategy> MakeDrawingStrategy(
		const std::string& type,
		std::istream& args
	)
	{
		if (type == "circle")
		{
			return MakeEllipseStrategy(args);
		}
		else if (type == "rectangle")
		{
			return MakeRectangleStrategy(args);
		}
		else if (type == "line")
		{
			return MakeLineStrategy(args);
		}
		else if (type == "triangle")
		{
			return MakeTriangleStrategy(args);
		}
		else if (type == "text")
		{
			return MakeTextStrategy(args);
		}
		else
		{
			throw std::logic_error("Undefined type " + type);
		}
	}
private:
	std::unique_ptr<EllipseDrawingStrategy> MakeEllipseStrategy(std::istream& args)
	{
		double x, y, radius;
		args >> x >> y >> radius;
		if (radius < 0)
		{
			throw std::logic_error("Radius can not be negative!");
		}
		return std::make_unique<EllipseDrawingStrategy>(x, y, radius, radius);
	}

	std::unique_ptr<RectangleDrawingStrategy> MakeRectangleStrategy(std::istream& args)
	{
		double x, y, width, heigth;
		args >> x >> y >> width >> heigth;
		if (width < 0 || heigth < 0)
		{
			throw std::logic_error("width and heigth can not be negative!");
		}

		return std::make_unique<RectangleDrawingStrategy>(x, y, width, heigth);
	}

	std::unique_ptr<LineDrawingStrategy> MakeLineStrategy(std::istream& args)
	{
		double x1, y1, x2, y2;
		args >> x1 >> y1 >> x2 >> y2;

		return std::make_unique<LineDrawingStrategy>(x1, y1, x2, y2);
	}

	std::unique_ptr<TriangleDrawingStrategy> MakeTriangleStrategy(std::istream& args)
	{
		double x1, y1, x2, y2, x3, y3;
		args >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

		return std::make_unique<TriangleDrawingStrategy>(x1, y1, x2, y2, x3, y3);
	}

	std::unique_ptr<TextDrawingStrategy> MakeTextStrategy(std::istream& args)
	{
		double width, heigth, fontSize;
		std::string text;
		args >> width >> heigth >> fontSize;
		getline(args, text);
		if (width < 0 || heigth < 0 || fontSize < 0)
		{
			throw std::logic_error("width and heigth and font size can not be negative!");
		}

		return std::make_unique<TextDrawingStrategy>(width, heigth, fontSize, text);
	}
};