#pragma once
#include "../stdafx.h"
#include "../graphics/CommonTypes.h"
#include "../graphics/Canvas.h"
#include "../graphics/Style.h"

class IGroupShape;

class IDrawable
{
public:
	virtual void Draw(ICanvas& canvas) const = 0;

	virtual ~IDrawable() = default;
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

class IGroupShape : public IShape, public IShapes, public IStyleEnumerator
	, public std::enable_shared_from_this<IGroupShape>
{
public:
	virtual ~IGroupShape() = default;
};

class ISlide : public IDrawable
{
public:
	virtual double GetWidth()const = 0;
	virtual double GetHeight()const = 0;

	virtual std::vector<std::shared_ptr<IShape>> GetShapes()const = 0;

	virtual ~ISlide() = default;
};

class CShape : public IShape
{
public:
	CShape(CSimpleFillStyle fillStyle, CSimpleLineStyle lineStyle)
		: m_fillStyle(fillStyle)
		, m_lineStyle(lineStyle)
	{
	}

	virtual RectD GetFrame() = 0;

	virtual void SetFrame(const RectD& rect) = 0;

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

	void SetOutlineStyle(const CSimpleLineStyle& style)
	{
		m_lineStyle = style;
	}

	void SetFillStyle(const CSimpleFillStyle& style)
	{
		m_fillStyle = style;
	}

	virtual void Draw(ICanvas& canvas) const = 0;
	virtual ~CShape() = default;
private:
	CSimpleFillStyle m_fillStyle;
	CSimpleLineStyle m_lineStyle;
};

class CRectangle : public CShape
{
public:
	CRectangle(CSimpleFillStyle fillStyle, CSimpleLineStyle lineStyle, double left, double top, double width, double height)
		: CShape(fillStyle, lineStyle)
		, m_frame({ left, top, width, height })
		, m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	RectD GetFrame() override
	{
		return m_frame;
	}

	void SetFrame(const RectD& rect) override
	{
		m_left = rect.left;
		m_top = rect.top;
		m_height = rect.height;
		m_width = rect.width;
		m_frame = rect;
	}

	void Draw(ICanvas& canvas) const override final
	{
		canvas.BeginFill(GetFillStyle().GetColor().value());
		if (GetlineStyle().GetColor().has_value())
		{
			canvas.SetLineColor(GetlineStyle().GetColor().value());
		}
		if (GetlineStyle().GetThickness().has_value())
		{
			canvas.SetThickness(GetlineStyle().GetThickness().value());
		}
		canvas.MoveTo(m_left, m_top);
		canvas.LineTo(m_left + m_width, m_top);
		canvas.LineTo(m_left + m_width, m_top + m_height);
		canvas.LineTo(m_left, m_top + m_height);
		canvas.LineTo(m_left, m_top);
		canvas.EndFill();
	}

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
	RectD m_frame;
};

class CEllipse : public CShape
{
public:
	CEllipse(CSimpleFillStyle fillStyle, CSimpleLineStyle lineStyle, 
		double left, double top, double width, double height)
		: CShape(fillStyle, lineStyle)
		, m_frame({ left, top, width, height })
		, m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	RectD GetFrame() override
	{
		return m_frame;
	}

	void SetFrame(const RectD& rect) override
	{
		m_left = rect.left; 
		m_top = rect.top;
		m_height = rect.height;
		m_width = rect.width;
		m_frame = rect;
	}

	void Draw(ICanvas& canvas) const override final
	{
		canvas.BeginFill(GetFillStyle().GetColor().value());
		if (GetlineStyle().GetColor().has_value())
		{
			canvas.SetLineColor(GetlineStyle().GetColor().value());
		}
		if (GetlineStyle().GetThickness().has_value())
		{
			canvas.SetThickness(GetlineStyle().GetThickness().value());
		}
		canvas.DrawEllipse(m_left, m_top, m_width / 2, m_height / 2);
		canvas.EndFill();
	}

private:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
	RectD m_frame;
};

class CTriangle : public CShape
{
public:
	CTriangle(CSimpleFillStyle fillStyle, CSimpleLineStyle lineStyle,
		double x1, double y1, double x2, double y2, double x3, double y3)
		: CShape(fillStyle, lineStyle)
		, m_frame({
				std::min(x1, std::min(x2, x3)),
				std::min(y1, std::min(y2, y3)),
				std::max(x1, std::max(x2, x3)) - std::min(x1, std::min(x2, x3)),
				std::max(y1, std::max(y2, y3)) - std::min(y1, std::min(y2, y3))
			})
	{
		if (x1 == x2 == x3 || y1 == y2 == y3)
		{
			throw std::logic_error("shape are not a triangle!");
		}
		m_x1 = x1;
		m_x2 = x2;
		m_x3 = x3;
		m_y1 = y1;
		m_y2 = y2;
		m_y3 = y3;
	}

	RectD GetFrame() override
	{
		return m_frame;
	}

	void SetFrame(const RectD& rect) override
	{
		m_x1 -= rect.left;
		m_x2 -= rect.left;
		m_x3 -= rect.left;
		m_y1 -= rect.top;
		m_y2 -= rect.top;
		m_y3 -= rect.top;

		TransformPoints(m_x1, m_x2, m_x3, rect.left, rect.width);
		TransformPoints(m_y1, m_y2, m_y3, rect.top, rect.height);

		m_frame = rect;
	}

	void Draw(ICanvas& canvas) const override final
	{
		canvas.BeginFill(GetFillStyle().GetColor().value());
		if (GetlineStyle().GetColor().has_value())
		{
			canvas.SetLineColor(GetlineStyle().GetColor().value());
		}
		if (GetlineStyle().GetThickness().has_value())
		{
			canvas.SetThickness(GetlineStyle().GetThickness().value());
		}
		canvas.MoveTo(m_x1, m_y1);
		canvas.LineTo(m_x2, m_y2);
		canvas.LineTo(m_x3, m_y3);
		canvas.LineTo(m_x1, m_y1);
		canvas.EndFill();
	}

private:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;
	double m_x3;
	double m_y3;
	RectD m_frame;

	void TransformPoints(double& d1, double& d2, double& d3, double point, double size)
	{
		if (d1 <= d2 <= d3)
		{
			d1 = point;
			d3 = point + size;
			d2 = point + size * (d1 / d3);
		}
		if (d1 <= d3 <= d2)
		{
			d1 = point;
			d2 = point + size;
			d3 = point + size * (d1 / d2);
		}
		if (d2 <= d1 <= d3)
		{
			d2 = point;
			d3 = point + size;
			d1 = point + size * (d2 / d3);
		}
		if (d2 <= d3 <= d1)
		{
			d2 = point;
			d1 = point + size;
			d3 = point + size * (d2 / d1);
		}
		if (d3 <= d1 <= d2)
		{
			d3 = point;
			d2 = point + size;
			d1 = point + size * (d3 / d2);
		}
		if (d3 <= d2 <= d1)
		{
			d3 = point;
			d1 = point + size;
			d2 = point + size * (d3 / d1);
		}
	}
};

class CGroupShape : public IGroupShape
{
public:
	CGroupShape(std::vector<std::shared_ptr<IShape>> shapes)
		: m_shapes(shapes)
		, m_lineStyle(this)
		, m_fillStyle(this)
	{
	}

	void EnumerateAllLineStyle(std::function<void(IStyle&)> callback) override
	{
		for (auto it : m_shapes)
		{
			callback(it->GetlineStyle());
		}
	}

	void EnumerateAllFillStyle(std::function<void(IStyle&)> callback) override
	{
		for (auto it : m_shapes)
		{
			callback(it->GetFillStyle());
		}
	}

	RectD GetFrame() override
	{
		RectD frame = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0 ,0};
		for (auto it : m_shapes)
		{
			if (frame.left > it->GetFrame().left)
			{
				frame.left = it->GetFrame().left;
			}
			if (frame.top > it->GetFrame().top)
			{
				frame.top = it->GetFrame().top;
			}
			if (frame.left + frame.width < it->GetFrame().left + it->GetFrame().width)
			{
				frame.width = it->GetFrame().left + it->GetFrame().width - frame.left;
			}
			if (frame.top + frame.height < it->GetFrame().top + it->GetFrame().height)
			{
				frame.height = it->GetFrame().top + it->GetFrame().height - frame.top;
			}
		}
		return frame;
	}

	void SetFrame(const RectD& rect) override
	{
		RectD currFrame = GetFrame();
		for (auto& it : m_shapes)
		{
			RectD frame = it->GetFrame();
			frame.left += rect.left - currFrame.left;
			frame.top += rect.top - currFrame.top;
			frame.width *= rect.width / currFrame.width;
			frame.height *= rect.height / currFrame.height;
			it->SetFrame(frame);
		}
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

	size_t GetShapesCount()const override
	{
		return m_shapes.size();
	}

	void InsertShape(
		const std::shared_ptr<IShape>& shape,
		size_t position = 0
	) override
	{
		m_shapes.insert(m_shapes.begin() + position, shape);
	}

	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override
	{
		return m_shapes.at(index);
	}

	void RemoveShapeAtIndex(size_t index) override
	{
		m_shapes.erase(m_shapes.begin() + index);
	}

	void Draw(ICanvas& canvas)const override
	{
		for (auto& it : m_shapes)
		{
			it->Draw(canvas);
		}
	}
private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
	CCompositeLineStyle m_lineStyle;
	CCompositeFillStyle m_fillStyle;
};

class CSlide: public ISlide
{
public:
	CSlide(std::vector<std::shared_ptr<IShape>> shapes)
		: m_shapes(shapes)
	{
	}

	std::vector<std::shared_ptr<IShape>> GetShapes()const override
	{
		return m_shapes;
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

	void Draw(ICanvas& canvas)const override
	{
		for (auto& it : m_shapes)
		{
			it->Draw(canvas);
		}
	}
private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
	RGBAColor m_backgroundColor;
	double m_width;
	double m_height;
};