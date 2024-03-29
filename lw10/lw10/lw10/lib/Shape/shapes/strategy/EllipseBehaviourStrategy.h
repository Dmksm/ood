#pragma once
#include "IBehaviourStrategy.h"

class EllipseBehaviourStrategy: public IBehaviourStrategy
{
public:
	EllipseBehaviourStrategy(double cx, double cy, double rx, double ry)
		: m_cx(cx)
		, m_cy(cy)
		, m_rx(rx)
		, m_ry(ry)
	{
	}

	void Move(double dx, double dy) override
	{
		m_cx += dx;
		m_cy += dy;
	}

	RectD GetFrame() const override
	{
		return RectD({ m_cx - m_rx, m_cy - m_ry, 2 * m_rx, 2 * m_ry });
	}

	void SetFrame(RectD frame) override
	{
		m_rx = frame.width / 2;
		m_ry = frame.height / 2;
		m_cx = frame.left + m_rx;
		m_cy = frame.top + m_ry;
	}

	ShapeType GetType() override
	{
		return ShapeType::Ellipse;
	};

private:
	double m_cx;
	double m_cy;
	double m_rx;
	double m_ry;
};