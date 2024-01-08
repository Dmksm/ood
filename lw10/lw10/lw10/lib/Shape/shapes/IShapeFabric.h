#pragma once
#include "../stdafx.h"
#include "../shapes/IPicture.h"

class IShapeFabric
{
public:
	virtual std::unique_ptr<IDrawingStrategy> MakeDrawingStrategy(
		const std::string& type,
		std::istream& args
	) const = 0;
};