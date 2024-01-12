#pragma once
#include "IBehaviourStrategy.h"
#include "../../stdafx.h"

class TriangleBehaviourStrategy : public IBehaviourStrategy
{
public:
	TriangleBehaviourStrategy(double x1, double y1, double x2, double y2, double x3, double y3)
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
		, m_x3(x3)
		, m_y3(y3)
	{
	}

	void Move(double dx, double dy) override
	{
		m_x1 += dx;
		m_y1 += dy;
		m_x2 += dx;
		m_y2 += dy;
		m_x3 += dx;
		m_y3 += dy;
	}

	RectD GetFrame() const override
	{
		return RectD({
			 std::min(m_x1, std::min(m_x2, m_x3)),
			 std::min(m_y1, std::min(m_y2, m_y3)),
			 std::max(m_x1, std::max(m_x2, m_x3)) - std::min(m_x1, std::min(m_x2, m_x3)),
			 std::max(m_y1, std::max(m_y2, m_y3)) - std::min(m_y1, std::min(m_y2, m_y3))
		});
	}

	void SetFrame(RectD frame) override
	{
		m_x1 = frame.left;
		m_y1 = frame.top + frame.height;
		m_x2 = frame.left + frame.width / 2;
		m_y2 = frame.top;
		m_x3 = frame.left + frame.width;
		m_y3 = frame.top + frame.height;
	}

	std::string GetType() override
	{
		return "triangle";
	};

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2; 
	double m_x3;
	double m_y3;
};