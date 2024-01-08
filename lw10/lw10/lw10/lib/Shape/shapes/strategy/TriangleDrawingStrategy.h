#pragma once
#include "IDrawingStrategy.h"
#include "../../stdafx.h"

class TriangleDrawingStrategy : public IDrawingStrategy
{
public:
	TriangleDrawingStrategy(double x1, double y1, double x2, double y2, double x3, double y3)
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

	void Draw(ICanvas& canvas, Color color) override
	{
		canvas.SetColor(color);
		canvas.DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x3, m_y3);
	};

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
		RectD currFrame = GetFrame();
		double centerX = (m_x1 + m_x2 + m_x3) / 3;
		double centerY = (m_y1 + m_y2 + m_y3) / 3;
		double scaleFactorX = frame.width / currFrame.width;
		double scaleFactorY = frame.height / currFrame.height;

		TransformPoint(m_x1, m_y1, centerX, centerY, scaleFactorX, scaleFactorY);
		TransformPoint(m_x2, m_y2, centerX, centerY, scaleFactorX, scaleFactorY);
		TransformPoint(m_x3, m_y3, centerX, centerY, scaleFactorX, scaleFactorY);
	}

	std::string GetStrategyParams() override
	{
		std::stringstream ss;
		ss << "triangle " << m_x1 << " " << m_y1
			<< " " << m_x2 << " " << m_y2 << " " << m_x3 << " " << m_y3;
		return ss.str();
	};

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2; 
	double m_x3;
	double m_y3;

	void TransformPoint(
		double& x,
		double& y,
		double centerX, 
		double centerY, 
		double scaleFactorX, 
		double scaleFactorY
	)
	{
		x -= centerX;
		x *= scaleFactorX;
		x += centerX;

		y -= centerY;
		y *= scaleFactorY;
		y += centerY;
	}
};