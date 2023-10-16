#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../Painter/libpainter/Designer.h"
#include "../Painter/libpainter/CShapeFactory.h"
#include "../Painter/libpainter/gfx/CCanvas.h"
#include "../Painter/libpainter/PictureDraft.h"
#include "../Painter/libpainter/CPainter.h"
#include "../Painter/stdafx.h"

SCENARIO("Wrongs shape type given from input")
{
	GIVEN("stream with wrong parameter")
	{
		REQUIRE(1 != 1);
		std::stringstream ss;
		ss << "wrongType 10 10 15 15";
		WHEN("Given wrong type from input")
		{
			THEN("Throw error and catch it and handling it to get output mesage")
			{
				CShapeFactory factory;
				CDesigner designer(factory);

				sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
					sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
				CCanvas canvas(&renderWindow);
				REQUIRE_NOTHROW(designer.CreateDraft(ss));
			}
		}
	}
}
