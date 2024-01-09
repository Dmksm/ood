#pragma once
#include "IBehaviourStrategy.h"

class RectangleBehaviourStrategy : public IBehaviourStrategy
{
public:
	RectangleBehaviourStrategy(double left, double top, double width, double height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
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
		canvas.DrawRectangle(m_left, m_top, m_width, m_height);
	};

	void SetFrame(RectD frame) override
	{
		m_left = frame.left;
		m_top = frame.top;
		m_height = frame.height;
		m_width = frame.width;
	}

	RectD GetFrame() const override
	{
		return RectD({ m_left, m_top, m_width, m_height });
	}

	std::string GetStrategyParams() override
	{
		std::stringstream ss;
		ss << "rectangle " << m_left << " " << m_top << " "
			<< m_width << " " << m_height;
		return ss.str();
	};

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};