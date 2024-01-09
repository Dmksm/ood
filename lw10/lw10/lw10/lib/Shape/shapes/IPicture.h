#pragma once
#include "../stdafx.h"
#include "strategy/IBehaviourStrategy.h"
#include "IShape.h"

class IPicture
{
public:
	using ShapesList = std::unordered_map<std::string, std::unique_ptr<IShape>>;

	virtual void DeleteShape(const std::string& id) = 0;
	virtual void AddShape(std::unique_ptr<IShape>&& shapePtr) = 0;
	virtual void ChangeShape(const std::string& id, std::unique_ptr<IBehaviourStrategy>&& drawingStrategyPtr) = 0;
	virtual const ShapesList& GetShapes() const = 0;
	virtual const std::unique_ptr<IShape>& GetShape(const std::string& id) const = 0;
	virtual unsigned GetSequenceNumber(const std::string& id) const = 0;
	virtual ~IPicture() = default;
};