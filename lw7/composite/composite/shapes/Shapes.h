#pragma once
#include "../stdafx.h"
#include "../graphics/CommonTypes.h"
#include "../graphics/Canvas.h"

class IGroupShape;

class IDrawable
{
public:
	virtual void Draw(ICanvas& canvas) const = 0;

	virtual ~IDrawable() = default;
};

class IStyle
{
public:
	virtual std::optional<bool> IsEnabled()const = 0;
	virtual void Enable(bool enable) = 0;

	virtual std::optional<RGBAColor> GetColor()const = 0;
	virtual void SetColor(RGBAColor color) = 0;

	virtual ~IStyle() = default;
};

class IShape : public IDrawable
{
public:
	virtual RectD GetFrame() = 0;
	virtual void SetFrame(const RectD& rect) = 0;

	virtual IStyle& GetlineStyle() = 0;
	virtual const IStyle& GetlineStyle()const = 0;

	virtual IStyle& GetFillStyle() = 0;
	virtual const IStyle& GetFillStyle()const = 0;

	virtual std::shared_ptr<IGroupShape> GetGroup() = 0;
	virtual std::shared_ptr<const IGroupShape> GetGroup() const = 0;

	virtual ~IShape() = default;
};

class IShapes
{
public:
	virtual size_t GetShapesCount()const = 0;
	virtual void InsertShape(
		const std::shared_ptr<IShape>& shape,
		size_t position = std::numeric_limits<size_t>::max()
	) = 0;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index) = 0;
	virtual void RemoveShapeAtIndex(size_t index) = 0;

	virtual ~IShapes() = default;
};

class IGroupShape : public IShape, public IShapes
{
public:
	virtual ~IGroupShape() = default;
};

class ISlide : public IDrawable
{
public:
	virtual double GetWidth()const = 0;
	virtual double GetHeight()const = 0;

	virtual IShapes& GetShapes()const = 0;

	virtual ~ISlide() = default;
};

class CStyle : public IStyle
{
public:
	std::optional<bool> IsEnabled()const override
	{
		return m_isEnable;
	}

	void Enable(bool enable) override
	{
		m_isEnable = enable;
	}

	std::optional<RGBAColor> GetColor()const override
	{
		return m_color;
	}

	void SetColor(RGBAColor color) override
	{
		m_color = color;
	}

private:
	RGBAColor m_color;
	std::optional<bool> m_isEnable = std::nullopt;
};

class CShape : public IShape
{
public:
	RectD GetFrame() override
	{
		return m_frame;
	}

	void SetFrame(const RectD& rect) override
	{
		m_frame = rect;
	}

	IStyle& GetlineStyle() override
	{
		return m_lineStyle;
	}

	const IStyle& GetlineStyle()const override
	{
		return m_lineStyle;
	}

	IStyle& GetFillStyle() override
	{
		return m_fillStyle;
	}

	const IStyle& GetFillStyle()const override
	{
		return m_fillStyle;
	}

	void SetOutlineStyle(const CStyle& style)
	{
		m_lineStyle = style;
	}

	void SetFillStyle(const CStyle& style)
	{
		m_fillStyle = style;
	}

	virtual void Draw(ICanvas& canvas) const = 0;
	virtual ~CShape() = default;
private:
	CStyle m_fillStyle;
	CStyle m_lineStyle;
	RectD m_frame;
};

class CRectangle : public CShape
{
public:
	CRectangle(double left, double top, double width, double height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(ICanvas& canvas) const final
	{
		canvas.MoveTo(m_left, m_top);
		canvas.LineTo(m_left + m_width, m_top);
		canvas.LineTo(m_left + m_width, m_top + m_height);
		canvas.LineTo(m_left, m_top + m_height);
		canvas.LineTo(m_left, m_top);
	}

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};

class CEllipse : public CShape
{
public:
	CEllipse(double left, double top, double width, double height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(ICanvas& canvas) const final
	{
		canvas.DrawEllipse(m_left, m_top, m_width, m_height);
	}

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};

class CTriangle : public CShape
{
public:
	CTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
		, m_x3(x3)
		, m_y3(y3)
	{
	}

	void Draw(ICanvas& canvas) const final
	{
		canvas.MoveTo(m_x1, m_y1);
		canvas.LineTo(m_x2, m_y2);
		canvas.LineTo(m_x3, m_y3);
		canvas.LineTo(m_x1, m_y1);
	}

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;
	double m_x3;
	double m_y3;
};

class CSlide: public ISlide, public IGroupShape
{
public:
	size_t GetShapesCount()const override
	{

	}

	void InsertShape(
		const std::shared_ptr<IShape>& shape,
		size_t position = std::numeric_limits<size_t>::max()
	) override
	{

	}

	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override
	{

	}

	void RemoveShapeAtIndex(size_t index) override
	{

	}
	
	IShapes& GetShapes()const override
	{
		//return m_shapes;
	}

	double GetWidth()const override
	{
		return m_width;
	}

	double GetHeight()const override
	{
		return m_height;
	}

	RGBAColor GetBackgroundColor()const
	{
		return m_backgroundColor;
	}

	void SetBackgroundColor(RGBAColor color)
	{
		m_backgroundColor = color;
	}

	void Draw(ICanvas& canvas)
	{
		for (auto& it : m_shapes)
		{
			it.Draw(canvas);
		}
	}
private:
	std::vector<IShape> m_shapes;
	RGBAColor m_backgroundColor;
	double m_width;
	double m_height;
};