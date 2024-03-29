#pragma once
#include "../gfx/ICanvas.h"
#include "../gfx/Color.h"
#include "strategy/IBehaviourStrategy.h"
#include "../stdafx.h"
#include "IShape.h"

class CShape: public IShape
{
public:
	CShape(std::unique_ptr<IBehaviourStrategy>&& drawingStrategy,
		const std::string& id,
		Color color)
		: m_behaviourStrategy(std::move(drawingStrategy))
		, m_id(id)
	    , m_color(color)
	{
	};

	void SetFrame(RectD frame) override
	{
		return m_behaviourStrategy->SetFrame(frame);
	}

	RectD GetFrame() const override
	{
		return m_behaviourStrategy->GetFrame();
	}

	void SetBehaviourStrategy(std::unique_ptr<IBehaviourStrategy>&& drawingStrategy) override
	{
		m_behaviourStrategy = std::move(drawingStrategy);
	}

	void Move(double dx, double dy) const override
	{
		if (!(dx == 0 && dy == 0))
		{
			m_behaviourStrategy->Move(dx, dy);
		}
	}

	void SetColor(Color color) override
	{
		if (m_color.GetHexFormat() != color.GetHexFormat())
		{
			m_color = color;
		}
	}	

	std::string GetId() const override
	{
		return m_id;
	}

	Color GetColor() const override
	{
		return m_color;
	}

	std::optional<IBehaviourStrategy::ShapeType> GetType() const override
	{
		if (m_behaviourStrategy)
		{
			return m_behaviourStrategy->GetType();
		}
		return std::nullopt;
	}

private:
	std::unique_ptr<IBehaviourStrategy> m_behaviourStrategy = nullptr;
	Color m_color;
	std::string m_id = "";

	void ValidateColor(const std::string& color) const
	{
		if (!IsValidHexCode(color))
		{
			const std::string message = "Undefined hex color code = " + color + ". ";
			throw std::logic_error(message);
		}
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