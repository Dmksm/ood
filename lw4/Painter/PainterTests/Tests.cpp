#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../Painter/libpainter/Designer.h"
#include "../Painter/libpainter/CShapeFactory.h"
#include "../Painter/libpainter/gfx/CCanvas.h"
#include "../Painter/libpainter/PictureDraft.h"
#include "../Painter/libpainter/CPainter.h"
#include "../Painter/stdafx.h"

class MockCanvas : public ICanvas
{
public:
	MockCanvas(std::stringstream& buffer)
        : m_buffer(buffer)
    {
    }

    void MoveTo(double x, double y) override
    {
    }

    void SetColor(Color color) override
    {
    }

    void LineTo(double x, double y) override
    {
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
		m_buffer << "ellipse " << cx << " " << cy << " " << rx << " " << ry << std::endl;
    }

    void DrawRegularPolygon(double radius, double vertexCount) override
    {
		m_buffer << "regularPolygon " << radius << " " << vertexCount << std::endl;
    }

    void DrawRectangle(double left, double top, double width, double height) override
    {
		m_buffer << "rectangle " << left << " " << top << " " << width << " " << height << std::endl;
    }

    void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) override
    {
		m_buffer << "triangle " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << std::endl;
    }

    void DrawText(double left, double top, double fontSize, const std::string& text) override
    {
    }

    void DrawLine(double x1, double y1, double x2, double y2) override
    {
    }

    void Display() override
    {
    }
private:
	std::stringstream& m_buffer; 
};

SCENARIO("Unavailable args given from input")
{
	GIVEN("stream with wrong parameter")
	{
		std::stringstream ss, ss1, ss2, ss3;
		ss << "wrongType red 10 10 15 15";
		ss1 << "";
		ss2 << "ellipse red 15 35 12";
		ss3 << "ellipse red 15 35 -1 12";
		WHEN("Given wrong type from input")
		{
			THEN("Throw error and catch it and handling it to get output mesage")
			{
				CShapeFactory factory;
				CDesigner designer(factory);

				REQUIRE_NOTHROW(designer.CreateDraft(ss));
				REQUIRE_NOTHROW(designer.CreateDraft(ss1));
				REQUIRE_NOTHROW(designer.CreateDraft(ss2));
				REQUIRE_NOTHROW(designer.CreateDraft(ss3));
			}
		}
	}
}

SCENARIO("Create shape")
{
	GIVEN("Args from stream and canvas and factory")
	{
		std::stringstream ss, ss1, ss2, ss3, outStream;
		ss << "ellipse red 17 32 96 58";
		ss1 << "triangle green 10 30 50 55 10 25";
		ss2 << "rectangle black 1000 500 50 50";
		ss3 << "regularPolygon pink 1 1";

		MockCanvas canvas(outStream);
		CShapeFactory factory;
		WHEN("Given wrong type from input")
		{
			THEN("Throw error and catch it and handling it to get output mesage")
			{
				std::unique_ptr<CShape> shapePtr = factory.CreateShape(ss.str());
				shapePtr->Draw(canvas);
				REQUIRE(shapePtr->GetColor() == Color::Red);

				shapePtr = factory.CreateShape(ss1.str());
				shapePtr->Draw(canvas);
				REQUIRE(shapePtr->GetColor() == Color::Green);

				shapePtr = factory.CreateShape(ss2.str());
				shapePtr->Draw(canvas);
				REQUIRE(shapePtr->GetColor() == Color::Black);

				shapePtr = factory.CreateShape(ss3.str());
				shapePtr->Draw(canvas);
				REQUIRE(shapePtr->GetColor() == Color::Pink);
				REQUIRE(outStream.str() == "ellipse 17 32 96 58\ntriangle 10 30 50 55 10 25\nrectangle 1000 500 50 50\nregularPolygon 1 1\n");
			}
		}
	}
}

SCENARIO("Picture draft adding shapes and it can be walked in a loop")
{
	GIVEN("Picture draft, factory, designer, stream, canvas")
	{
		CPictureDraft pictureDraft; 
		CShapeFactory factory;
		CDesigner designer(factory); 
		std::stringstream ss, ss1, outStream;
		MockCanvas canvas(outStream);
		ss << "ellipse red 10 20 30 40";
		ss1 << "regularPolygon pink 5 4";

		WHEN("Given wrong type from input")
		{
			REQUIRE(pictureDraft.IsEmpty());
			pictureDraft.AddShape(factory.CreateShape(ss.str()));
			pictureDraft.begin()->Draw(canvas);
			THEN("Throw error and catch it and handling it to get output mesage")
			{
				REQUIRE(!pictureDraft.IsEmpty());
				REQUIRE(outStream.str() == "ellipse 10 20 30 40\n");
				outStream.clear();
				pictureDraft.AddShape(factory.CreateShape(ss1.str()));
				for (auto& it : pictureDraft)
				{
					it.Draw(canvas);
				}

				REQUIRE("ellipse 10 20 30 40\nregularPolygon pink 5 4\n");
			}
		}
	}
}
