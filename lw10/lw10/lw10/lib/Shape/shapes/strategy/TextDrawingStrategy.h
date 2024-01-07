#pragma once
#include "IDrawingStrategy.h"

class TextDrawingStrategy : public IDrawingStrategy
{
public:
	TextDrawingStrategy(double left, double top, double fontSize, const std::string& text)
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