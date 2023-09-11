#pragma once
#include "CCanvas.h"
#include "Circle.h"
#include "Shape.h"
#include "IDrawingStrategy.h"
#include "EllipseDrawingStrategy.h"
#include "RectangleDrawingStrategy.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <sstream>
using namespace std::string_literals;

class Picture
{
public:
	Picture(std::unique_ptr<std::map<std::string, Shape>>&& shapes)
		: m_shapes(std::move(shapes))
	{
	};

	bool IsShapeExist(const std::string& id)
	{
		return m_shapes->find(id) != m_shapes->end();
	}

	void AddShape(
		const std::string& id,
		const std::string& hexColor,
		const std::string& type,
		std::istream& args
	)
	{
		if (!IsShapeExist(id))
		{
			Shape shape;
			if (type == "circle")
			{
				double x, y, radius;
				args >> x >> y >> radius;
				if (radius < 0)
				{
					throw std::logic_error("Radius can not be negative!");
				}

				Circle shape(x, y, radius);
			}
			if (type == "rectangle")
			{
				Shape shape;
				shape.SetDrawingStrategy(std::make_shared<RectangleDrawingStrategy>());
				double x, y, width, heigth;
				args >> x >> y >> width >> heigth;
				if (width < 0 || heigth < 0)
				{
					throw std::logic_error("width and heigth can not be negative!");
				}
				shape.SetPosition({ x, y });
				shape.SetBounds({ width, heigth });
			}
			else
			{
				throw std::logic_error("Undefined type " + type);
			}
			shape.SetColor(HexToUint32(hexColor));
			m_shapes->insert({ id, shape });
		}
	};

	void DrawShape(std::string id)
	{
		if (IsShapeExist(id))
		{
			CCanvas canvas;
			m_shapes->at(id).Draw(canvas);
		};
	}

private:
	IVisualObjectInfo::Color HexToUint32(const std::string& hexColor)
	{
		if (!IsValidHexCode(hexColor))
		{
			throw std::logic_error("Undefined hex color code = "s + hexColor + ". "s);
		}
		std::string hexCode = hexColor;
		if (hexCode.at(0) == '#') 
		{
			hexCode.erase(0, 1);
		}
		while (hexCode.length() != 6)
		{
			hexCode += "0";
		}
		while (hexCode.length() != 8)
		{
			hexCode += "F";
		}

		return std::stoul(hexCode, nullptr, 16);
	}

	bool IsValidHexCode(const std::string& hexColor)
	{
		const unsigned AVAILABLE_HEX_SIZE = 7;
		const char FIRST_HEX_SYMBOL = '#';
		if ((hexColor[0] != FIRST_HEX_SYMBOL) || (hexColor.length() != AVAILABLE_HEX_SIZE))
		{
			return false;
		}

		for (int position = 1; position < hexColor.length(); position++)
		{
			if (!((hexColor[position] >= '0' && hexColor[position] <= '9')
				|| (hexColor[position] >= 'a' && hexColor[position] <= 'f')
				|| (hexColor[position] >= 'A' && hexColor[position] <= 'F')))
			{
				return false;
			}
		}

		return true;
	}

	std::unique_ptr<std::map<std::string, Shape>> m_shapes;
};