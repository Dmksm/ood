#pragma once
#include "../../gfx/ICanvas.h"
#include "../../gfx/Color.h"
#include "../../stdafx.h"
//��������� ����� ���������� ��������
//��� ������������ �����������
class IBehaviourStrategy
{
public:
	virtual void Move(double dx, double dy) = 0;
	virtual std::string GetStrategyParams() = 0;
	virtual void SetFrame(RectD frame) = 0;
	virtual RectD GetFrame() const = 0;
	virtual void Draw(ICanvas& canvas, Color color) = 0;
	virtual ~IBehaviourStrategy() = default;
};