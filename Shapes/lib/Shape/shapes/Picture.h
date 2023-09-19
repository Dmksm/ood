#pragma once
#include "../gfx/CCanvas.h"
#include "Shape.h"
#include "strategy/IDrawingStrategy.h"
#include "strategy/EllipseDrawingStrategy.h"
#include "strategy/RectangleDrawingStrategy.h"
#include "strategy/LineDrawingStrategy.h"
#include "strategy/TriangleDrawingStrategy.h"
#include "strategy/TextDrawingStrategy.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <map>
#include <sstream>

class Picture
{
public:
	using ShapesList = std::map<unsigned, std::unique_ptr<Shape>>;

	Picture(ShapesList& shapes): m_shapes(shapes)
	{
	};

	void MoveShape(const std::string& id, double dx, double dy) const
	{
		ShapesList::const_iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator == m_shapes.end())
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		shapeIterator->second->Move(dx, dy);
	}

	void Move(double dx, double dy) const
	{
		for (auto& it : m_shapes)
		{
			it.second->Move(dx, dy);
		}
	}

	void DeleteShape(const std::string& id)
	{
		m_shapes.erase(GetShapeIterator(id));
	}

	void AddShape(
		const std::string& id,
		const std::string& color,
		const std::string& type,
		std::istream& args
	) 
	{
		ValidateColor(color);
		ShapesList::const_iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator != m_shapes.end())
		{
			throw std::logic_error("Shape with id = "s + id + " already exist! "s);
		}

		Shape shape(MakeDrawingStrategy(type, args), id, color);
		m_shapes.insert({ ++m_token,  std::make_unique<Shape>(shape) });
	};

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

	void ChangeShape(
		const std::string& id,
		const std::string& type,
		std::istream& args
	)
	{
		ShapesList::const_iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator == m_shapes.end())
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		}
		shapeIterator->second->SetDrawingStrategy(MakeDrawingStrategy(type, args));
	};

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

	void ChangeColor(const std::string& id, const std::string& color) const
	{
		ValidateColor(color);
		ShapesList::const_iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator == m_shapes.end())
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		shapeIterator->second->SetColor(color);
	}

	const ShapesList& GetShapes() const
	{
		return m_shapes;
	}

	void Draw(ICanvas& canvas)
	{
		for (auto& it : m_shapes)
		{
			it.second->Draw(canvas);
		}
		canvas.Display();
	}

	void DrawShape(const std::string& id, ICanvas& canvas) const
	{
		ShapesList::const_iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator == m_shapes.end())
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		shapeIterator->second->Draw(canvas);
		canvas.Display();
	}

	/*void CloneShape(const std::string& id, const std::string& newId)
	{
		ShapesList::iterator shapeIterator = GetShapeIterator(id);
		if (shapeIterator == m_shapes->end())
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};

		Shape shape(shapeIterator->second.GetDrawingStrategy(), 
			id, shapeIterator->second.GetColor());
		m_shapes->insert({ ++m_token, shape });
	}*/

private:
	unsigned m_token = 0;
	ShapesList m_shapes;

	void ValidateColor(const std::string& color) const
	{
		if (!IsValidHexCode(color))
		{
			throw std::logic_error("Undefined hex color code = "s + color + ". "s);
		}
	}

	ShapesList::const_iterator GetShapeIterator(const std::string& id) const
	{
		for (auto& it : m_shapes)
		{
			if (it.second->GetId() == id)
			{
				return m_shapes.find(it.first);
			}
		}
		return m_shapes.end();
	}

	bool IsValidHexCode(const std::string& hexColor) const
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
};