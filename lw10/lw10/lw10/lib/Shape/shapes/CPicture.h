#pragma once
#include "../gfx/CCanvas.h"
#include "CShape.h"
#include "IPicture.h"
#include "strategy/IDrawingStrategy.h"
#include "strategy/EllipseDrawingStrategy.h"
#include "strategy/RectangleDrawingStrategy.h"
#include "strategy/LineDrawingStrategy.h"
#include "strategy/TriangleDrawingStrategy.h"
#include "strategy/TextDrawingStrategy.h"
#include "../stdafx.h"

class CPicture: public IPicture
{
public:
	void DeleteShape(const std::string& id) override
	{
		auto it = m_shapes.find(id);
		if (it != m_shapes.end())
		{
			m_SequenceNumberList.erase(it->second->GetId());
			m_shapes.erase(it);
		}
	}

	void AddShape(std::unique_ptr<IShape>&& shapePtr) override
	{
		if (shapePtr == nullptr || IsShapeExist(shapePtr->GetId()))
		{
			throw std::logic_error("Shape with id = "s + shapePtr->GetId() + " already exist! "s);
		};
		std::string id = shapePtr->GetId();
		m_shapes.insert({ id,  std::move(shapePtr) });
		m_SequenceNumberList.insert({ id, ++m_sequenceNumber });
	};

	void ChangeShape(const std::string& id, std::unique_ptr<IDrawingStrategy>&& drawingStrategyPtr) override
	{
		if (!IsShapeExist(id))
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		m_shapes.at(id)->SetDrawingStrategy(std::move(drawingStrategyPtr));
	};

	const ShapesList& GetShapes() const override
	{
		return m_shapes;
	}

	const std::unique_ptr<IShape>& GetShape(const std::string& id) const override
	{
		if (!IsShapeExist(id))
		{
			throw std::logic_error("Shape with id = "s + id + " does not exist! "s);
		};
		return m_shapes.at(id);
	}

	unsigned GetSequenceNumber(const std::string& id) const override
	{
		return m_SequenceNumberList.at(id);
	}

private:
	ShapesList m_shapes = {};
	unsigned m_sequenceNumber = 0;
	std::map<std::string, unsigned> m_SequenceNumberList = {};

	bool IsShapeExist(const std::string& id) const
	{
		return m_shapes.find(id) != m_shapes.end();
	}
};