#include "shapes/Shapes.h"
#include "graphics/CCanvas.h"
#include "graphics/Canvas.h"
#include "graphics/Style.h"
#include "graphics/CommonTypes.h"
#include "stdafx.h"

int main()
{
	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Default, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	CCanvas canvas = CCanvas(&renderWindow);
	CSimpleFillStyle fillStyle(255u * 255 * 120);
	CSimpleLineStyle lineStyle({ 120u * 255 * 120 }, 2);
	CTriangle triangle{ fillStyle, lineStyle, 100, 450, 225, 350, 350, 450 };
	CSimpleFillStyle fillStyle2(100u * 80 * 255);
	CSimpleLineStyle lineStyle2({ 120u * 80 * 120 }, 4);
	CRectangle rectangle{ fillStyle2, lineStyle2, 150, 450, 150, 200 };
	CSimpleFillStyle fillStyle3(255u * 255 * 255 * 150);
	CSimpleLineStyle lineStyle3({ 255u * 255 * 255 * 150 }, 1);
	CEllipse ellipse{ fillStyle3, lineStyle3, 1300, 80, 200, 200 };
	CSimpleFillStyle fillStyle4(100u * 100 * 100);
	CSimpleLineStyle lineStyle4({ 100u * 100 * 100 }, 5);
	CRectangle rectangle2{ fillStyle4, lineStyle4, 0, 800, 1900, 500 };

	std::vector<std::shared_ptr<IShape>> houseShape;
	houseShape.push_back(std::make_shared<CTriangle>(triangle));
	houseShape.push_back(std::make_shared<CRectangle>(rectangle));
	std::shared_ptr<IGroupShape> groupHouseShape = CGroupShape::Create(houseShape);

	std::vector<std::shared_ptr<IShape>> shapes;
	shapes.push_back(groupHouseShape);
	shapes.push_back(std::make_shared<CEllipse>(ellipse));
	shapes.push_back(std::make_shared<CRectangle>(rectangle2));

	CSlide slide(shapes, { 200u * 200 * 200 }, 1000, 600);
	slide.Draw(canvas);
	while (renderWindow.isOpen())
	{
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				renderWindow.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.scancode == sf::Keyboard::Scan::Delete)
				{
					std::cout << "the delete key was pressed" << std::endl;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
				}
			}
		}
	}
}