#pragma once
#include "../stdafx.h"
#include "../shapes/IPicture.h"
#include "../shapes/IShapeBehaviourFactory.h"
#include "strategy/IBehaviourStrategy.h"
#include "strategy/EllipseBehaviourStrategy.h"
#include "strategy/RectangleBehaviourStrategy.h"
#include "strategy/LineBehaviourStrategy.h"
#include "strategy/TriangleBehaviourStrategy.h"
#include "strategy/TextBehaviourStrategy.h"

class CShapeBehaviourFactory : public IShapeBehaviourFactory
{
public:
	std::unique_ptr<IBehaviourStrategy> MakeBehaviourStrategy(
		const std::string& type,
		RectD frame,
		std::optional<unsigned> framefontSize,
		std::optional<std::string> text
	) const override
	{
		if (type == "ellipse")
		{
			return MakeEllipseStrategy(frame);
		}
		else if (type == "rectangle")
		{
			return MakeRectangleStrategy(frame);
		}
		else if (type == "line")
		{
			return MakeLineStrategy(frame);
		}
		else if (type == "triangle")
		{
			return MakeTriangleStrategy(frame);
		}
		else if (type == "text")
		{
			return MakeTextStrategy(frame, framefontSize, text);
		}
		else
		{
			throw std::logic_error("Undefined type " + type);
		}
	}
private:
	std::unique_ptr<EllipseBehaviourStrategy> MakeEllipseStrategy(RectD frame) const
	{
		return std::make_unique<EllipseBehaviourStrategy>(
			frame.left + frame.width / 2,
			frame.top + frame.height / 2,
			frame.width / 2,
			frame.height / 2
		);
	}

	std::unique_ptr<RectangleBehaviourStrategy> MakeRectangleStrategy(RectD frame) const
	{
		return std::make_unique<RectangleBehaviourStrategy>(
			frame.left, frame.top,
			frame.width, frame.height
		);
	}

	std::unique_ptr<LineBehaviourStrategy> MakeLineStrategy(RectD frame) const
	{
		return std::make_unique<LineBehaviourStrategy>(
			frame.left, frame.top,
			frame.left + frame.width, frame.top + frame.height
		);
	}

	std::unique_ptr<TriangleBehaviourStrategy> MakeTriangleStrategy(RectD frame) const
	{
		return std::make_unique<TriangleBehaviourStrategy>(
			frame.left, frame.top + frame.height,
			frame.left + frame.width / 2, frame.top,
			frame.left + frame.width, frame.top + frame.height
		);
	}

	std::unique_ptr<TextBehaviourStrategy> MakeTextStrategy(
		RectD frame,
		std::optional<unsigned> framefontSize, 
		std::optional<std::string> text
	) const
	{
		if (!framefontSize.has_value() || !text.has_value())
		{
			throw std::logic_error("insufficient data for build text strategy!");
		}
		return std::make_unique<TextBehaviourStrategy>(
			frame.width, frame.height, framefontSize.value(), text.value()
		);
	}
};