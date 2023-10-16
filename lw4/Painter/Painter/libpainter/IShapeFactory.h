#pragma once
#include "../stdafx.h"

class CShape;

class IShapeFactory
{
public:
	virtual std::unique_ptr<CShape> CreateShape(const std::string& description) = 0;

	virtual ~IShapeFactory() = default;
};