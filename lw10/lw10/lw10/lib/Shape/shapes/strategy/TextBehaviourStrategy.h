#pragma once
#include "../../stdafx.h"
#include "IBehaviourStrategy.h"

class TextBehaviourStrategy : public IBehaviourStrategy
{
public:
	TextBehaviourStrategy(double left, double top, double fontSize, const std::string& text)
		: m_left(left)
		, m_top(top)
		, m_fontSize(fontSize)
		, m_text(text)
	{
	}

	void Move(double dx, double dy) override
	{
		m_left += dx;
		m_top += dy;
	}

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.DrawText(m_left, m_top, m_fontSize, m_text);
	};

	void SetFrame(RectD frame) override
	{

	}

	RectD GetFrame() const override
	{
		return RectD({ m_left, m_top, m_fontSize * (1 + m_text.size() / 5), 1.5 * m_fontSize });
	}

	std::string GetStrategyParams() override
	{
		std::stringstream ss;
		ss << "text " << " " << m_left << " " << m_top << " "
			<< m_fontSize << " " << m_text;
		return ss.str();
	};

private:
	double m_left;
	double m_top;
	double m_fontSize;
	std::string m_text;
};