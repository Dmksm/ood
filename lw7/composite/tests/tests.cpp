#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../composite/stdafx.h"
#include "../composite/shapes/Shapes.h"
#include "../composite/graphics/CCanvas.h"
#include "../composite/graphics/Canvas.h"
#include "../composite/graphics/Style.h"
#include "../composite/graphics/CommonTypes.h"
#include "../composite/stdafx.h"

class CMockGroupShape : public IGroupShape
{
public:
	CMockGroupShape(std::vector<std::shared_ptr<IShape>> shapes)
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
		RectD frame = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0 ,0 };
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

	std::vector<std::shared_ptr<IShape>> m_shapes;
	CCompositeLineStyle m_lineStyle;
	CCompositeFillStyle m_fillStyle;
};

SCENARIO("test composite shape")
{
	GIVEN("Group shape and shapes")
	{
		std::vector<std::shared_ptr<IShape>> shapes;
		CMockGroupShape groupHouseShape(shapes);
		REQUIRE(groupHouseShape.m_shapes.size() == 0);
		CSimpleFillStyle fillStyle(255u * 255 * 120);
		CSimpleLineStyle lineStyle({ 120u * 255 * 120 }, 2);
		CRectangle rect(fillStyle, lineStyle, 20, 30, 40, 50);

		WHEN("Manipulate shape shape at group")
		{
			THEN("Completed action without errors")
			{
				REQUIRE_NOTHROW(groupHouseShape.InsertShape(std::make_shared<CRectangle>(rect)));
				REQUIRE(groupHouseShape.m_shapes.size() == 1);
				REQUIRE(groupHouseShape.GetFillStyle().GetColor() == rect.GetFillStyle().GetColor());
				REQUIRE_NOTHROW(groupHouseShape.RemoveShapeAtIndex(0));
			}
		}
	}
}