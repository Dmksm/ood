#pragma once
#include "../stdafx.h"
#include "../graphics/CommonTypes.h"

class IStyle
{
public:
	virtual std::optional<bool> IsEnabled()const = 0;
	virtual void Enable(bool enable) = 0;

	virtual std::optional<RGBAColor> GetColor()const = 0;
	virtual void SetColor(RGBAColor color) = 0;

	virtual ~IStyle() = default;
};

class CStyle
{
public:
	bool IsEnabled()const;
	void Enable(bool enable);

	RGBAColor GetColor()const;
	void SetColor(RGBAColor color);
};