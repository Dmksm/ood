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

class ILineStyleEnumerator
{
public:
	virtual void EnumerateAllLineStyle(std::function<void(IStyle&)> callback) = 0;

	virtual ~ILineStyleEnumerator() = default;
};

class IFillStyleEnumerator
{
public:
	virtual void EnumerateAllFillStyle(std::function<void(IStyle&)> callback) = 0;

	virtual ~IFillStyleEnumerator() = default;
};

class CSimpleFillStyle : public IStyle
{
public:
	CSimpleFillStyle(RGBAColor color)
		: m_color(color)
	{
	}

	std::optional<bool> IsEnabled()const override
	{
		return m_isEnabled;
	}

	void Enable(bool enable)
	{
		m_isEnabled = enable;
	}

	std::optional<RGBAColor> GetColor()const
	{
		return m_color;
	}

	void SetColor(RGBAColor color)
	{
		m_color = color;
	}
private:
	RGBAColor m_color = 0;
	bool m_isEnabled = true;
};

class CSimpleLineStyle : public IStyle
{
public:
	CSimpleLineStyle(RGBAColor color)
		: m_color(color)
	{
	}

	std::optional<bool> IsEnabled()const override
	{
		return m_isEnabled;
	}

	void Enable(bool enable)
	{
		m_isEnabled = enable;
	}

	std::optional<RGBAColor> GetColor()const
	{
		return m_color;
	}

	void SetColor(RGBAColor color)
	{
		m_color = color;
	}
private:
	RGBAColor m_color = 0;
	bool m_isEnabled = true;
};

class CCompositeLineStyle: public IStyle
{
public:
	CCompositeLineStyle(std::shared_ptr<ILineStyleEnumerator> styleEnumerator) 
		: m_styleEnumerator(styleEnumerator)
	{
	};

	std::optional<bool> IsEnabled()const override
	{
		std::optional<bool> isEnabled = std::nullopt;
		m_styleEnumerator->EnumerateAllLineStyle([&isEnabled](IStyle& a) mutable {
			isEnabled = a.IsEnabled();
		});
		return isEnabled;
	}

	void Enable(bool enable)
	{
		m_styleEnumerator->EnumerateAllLineStyle([enable](IStyle& a) {
			a.Enable(enable);
		});
	}

	std::optional<RGBAColor> GetColor()const
	{
		bool isEnable = true;
		std::optional<RGBAColor> color = std::nullopt;
		bool isFirst = true;
		m_styleEnumerator->EnumerateAllLineStyle([&color, &isFirst](IStyle& a) mutable {
			if (!a.IsEnabled().has_value())
			{
				color = std::nullopt;
				return;
			}
			if (isFirst)
			{
				color = a.GetColor();
				isFirst = false;
			} 
			else
			{
				if (a.GetColor() != color)
				{
					color = std::nullopt;
					return;
				}
			}
		});
		return color;
	}

	void SetColor(RGBAColor color)
	{
		m_styleEnumerator->EnumerateAllLineStyle([color](IStyle& a) {
			a.SetColor(color);
		});
	}
private:
	std::shared_ptr<ILineStyleEnumerator> m_styleEnumerator;
};


class CCompositeFillStyle : public IStyle
{
public:
	CCompositeFillStyle(std::shared_ptr<IFillStyleEnumerator> styleEnumerator) 
		: m_styleEnumerator(styleEnumerator)
	{
	};

	std::optional<bool> IsEnabled()const override
	{
		std::optional<bool> isEnabled = std::nullopt;
		m_styleEnumerator->EnumerateAllFillStyle([&isEnabled](IStyle& a) mutable {
			isEnabled = a.IsEnabled();
			});
		return isEnabled;
	}

	void Enable(bool enable)
	{
		m_styleEnumerator->EnumerateAllFillStyle([enable](IStyle& a) {
			a.Enable(enable);
			});
	}

	std::optional<RGBAColor> GetColor()const
	{
		bool isEnable = true;
		std::optional<RGBAColor> color = std::nullopt;
		bool isFirst = true;
		m_styleEnumerator->EnumerateAllFillStyle([&color, &isFirst](IStyle& a) mutable {
			if (!a.IsEnabled().has_value())
			{
				color = std::nullopt;
				return;
			}
			if (isFirst)
			{
				color = a.GetColor();
				isFirst = false;
			}
			else
			{
				if (a.GetColor() != color)
				{
					color = std::nullopt;
					return;
				}
			}
			});
		return color;
	}

	void SetColor(RGBAColor color)
	{
		m_styleEnumerator->EnumerateAllFillStyle([color](IStyle& a) {
			a.SetColor(color);
			});
	}
private:
	std::shared_ptr<IFillStyleEnumerator> m_styleEnumerator;
};