#pragma once
#include "../gfx/ICanvas.h"
#include "../gfx/Color.h"
#include "IDrawingStrategy.h"
#include "EllipseDrawingStrategy.h"
#include <memory>
#include <string>

class Shape
{
public:
	Shape(std::unique_ptr<IDrawingStrategy>&& drawingStrategy, const std::string& id, Color color)
		: m_drawingStrategy(std::move(drawingStrategy))
		, m_color(color)
		, m_id(id)
	{
	};

	std::unique_ptr<Shape> Clone() const
	{
		return std::make_unique<Shape>(*this);
	}

	void SetDrawingStrategy(std::unique_ptr<IDrawingStrategy>&& drawingStrategy)
	{
		m_drawingStrategy = std::move(drawingStrategy);
	}

	void Move(double dx, double dy) const
	{
		if (!(dx == 0 && dy == 0))
		{
			m_drawingStrategy->Move(dx, dy);
		}
	}

	void Draw(ICanvas& canvas) const
	{
		m_drawingStrategy->Draw(canvas, m_color);
	};

	void SetColor(Color color)
	{
		if (m_color != color)
		{
			m_color = color;
		}
	}	

	std::string GetId() const
	{
		return m_id;
	}

	Color GetColor() const
	{
		return m_color;
	}

	void Display() const
	{
		if (m_drawingStrategy)
		{
			m_drawingStrategy->Display(m_id, m_color);
		}
	}

private:
	std::unique_ptr<IDrawingStrategy> m_drawingStrategy;
	Color m_color;
	std::string m_id;
};