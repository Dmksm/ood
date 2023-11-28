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

	virtual std::optional<double> GetThickness()const = 0;
	virtual void SetThickness(double thickness) = 0;

	virtual ~IStyle() = default;
};

class IStyleEnumerator
{
public:
	virtual void EnumerateAllLineStyle(std::function<void(IStyle&)> callback) = 0;
	virtual void EnumerateAllFillStyle(std::function<void(IStyle&)> callback) = 0;

	virtual ~IStyleEnumerator() = default;
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

	void Enable(bool enable) override
	{
		m_isEnabled = enable;
	}

	std::optional<RGBAColor> GetColor()const override
	{
		return m_color;
	}

	void SetColor(RGBAColor color) override
	{
		m_color = color;
	}

	std::optional<double> GetThickness()const override
	{
		return std::nullopt;
	}
	
	void SetThickness(double thickness) override
	{
	}
private:
	RGBAColor m_color = 0;
	bool m_isEnabled = true;
};

class CSimpleLineStyle : public IStyle
{
public:
	CSimpleLineStyle(RGBAColor color, double thickness)
		: m_color(color)
		, m_thickness(thickness)
	{
	}

	std::optional<bool> IsEnabled()const override
	{
		return m_isEnabled;
	}

	void Enable(bool enable) override
	{
		m_isEnabled = enable;
	}

	std::optional<RGBAColor> GetColor()const
	{
		return m_color;
	}

	void SetColor(RGBAColor color) override
	{
		m_color = color;
	}

	std::optional<double> GetThickness()const override
	{
		return m_thickness;
	}

	void SetThickness(double thickness) override
	{
		m_thickness = thickness;
	}
private:
	RGBAColor m_color = 0;
	bool m_isEnabled = true;
	double m_thickness = 0;
};

class CCompositeLineStyle: public IStyle
{
public:
	CCompositeLineStyle(IStyleEnumerator* styleEnumerator)
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

	std::optional<double> GetThickness()const override
	{
		bool isEnable = true;
		std::optional<double> m_thickness = std::nullopt;
		bool isFirst = true;
		m_styleEnumerator->EnumerateAllLineStyle([&m_thickness, &isFirst](IStyle& a) mutable {
			if (!a.IsEnabled().has_value())
			{
				m_thickness = std::nullopt;
				return;
			}
			if (isFirst)
			{
				m_thickness = a.GetThickness();
				isFirst = false;
			}
			else
			{
				if (a.GetThickness() != m_thickness)
				{
					m_thickness = std::nullopt;
					return;
				}
			}
			});
		return m_thickness;
	}

	void SetThickness(double thickness) override
	{
		m_styleEnumerator->EnumerateAllLineStyle([thickness](IStyle& a) {
			a.SetThickness(thickness);
			});
	}
private:
	std::shared_ptr<IStyleEnumerator> m_styleEnumerator;
};


class CCompositeFillStyle : public IStyle
{
public:
	CCompositeFillStyle(IStyleEnumerator* styleEnumerator)
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

	std::optional<double> GetThickness()const override
	{
		return std::nullopt;
	}

	void SetThickness(double thickness) override
	{
	}
private:
	std::shared_ptr<IStyleEnumerator> m_styleEnumerator;
};