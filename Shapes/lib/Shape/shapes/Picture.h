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
#include <unordered_map>
#include <sstream>

class Picture
{
public:
	using ShapesList = std::unordered_map<std::string, std::unique_ptr<Shape>>;

	void DeleteShape(const std::string& id)
	{
		auto it = m_shapes.find(id);
		if (it != m_shapes.end())
		{
			m_shapes.erase(it);
		}
	}

	void AddShape(std::unique_ptr<Shape>&& shapePtr)
	{
		if (IsShapeExist(shapePtr->GetId()))
		{
			throw std::logic_error("Shape with id = "s + shapePtr->GetId() + " already exist! "s);
		};
		m_shapes.insert({ shapePtr->GetId(),  std::move(shapePtr) });
	};

	void ChangeShape(const std::string& id, std::unique_ptr<IDrawingStrategy>&& drawingStrategyPtr)
	{
		if (!IsShapeExist(id))
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		m_shapes.at(id)->SetDrawingStrategy(std::move(drawingStrategyPtr));
	};

	const ShapesList& GetShapes() const
	{
		return m_shapes;
	}

	const std::unique_ptr<Shape>& GetShape(std::string& id) const
	{
		if (!IsShapeExist(id))
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		return m_shapes.at(id);
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
	ShapesList m_shapes = {};

	bool IsShapeExist(const std::string& id) const
	{
		return m_shapes.find(id) != m_shapes.end();
	}
};