#pragma once
#include "../stdafx.h"
#include "../shapes/IPicture.h"

class IShapeBehaviourFactory
{
public:
	virtual std::unique_ptr<IBehaviourStrategy> MakeBehaviourStrategy(
		const std::string& type,
		RectD frame,
		std::optional<unsigned> framefontSize,
		std::optional<std::string> text // точно не так передавать 
	) const = 0;
	virtual ~IShapeBehaviourFactory() = default;
};