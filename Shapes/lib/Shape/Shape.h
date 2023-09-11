#pragma once
#include "ICanvas.h"
#include "IVisualObjectInfo.h"
#include "IDrawingStrategy.h"
#include "EllipseDrawingStrategy.h"
#include <memory>

class Shape: public IVisualObjectInfo
{
public:
	Shape(std::shared_ptr<IDrawingStrategy>&& drawingStrategy = 
		std::make_shared<EllipseDrawingStrategy>(), Color color = 0)
		: m_drawingStrategy(std::move(drawingStrategy))
		, m_color(color)
	{
	};

	void SetDrawingStrategy(std::shared_ptr<IDrawingStrategy>&& drawingStrategy)
	{
		m_drawingStrategy = std::move(drawingStrategy);
	}

	void virtual Draw(ICanvas& canvas)
	{
		m_drawingStrategy->Draw(canvas, *this);
	};

	void SetColor(Color color)
	{
		m_color = color;
	}	

	Color GetColor()
	{
		return m_color;
	}

private:
	std::shared_ptr<IDrawingStrategy> m_drawingStrategy;
	Color m_color = 0;
};