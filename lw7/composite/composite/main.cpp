#include "shapes/Shapes.h"
#include "graphics/CCanvas.h"
#include "graphics/Canvas.h"
#include "graphics/Style.h"
#include "graphics/CommonTypes.h"
#include "stdafx.h"

int main()
{
	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	CCanvas canvas = CCanvas(&renderWindow);
	CSimpleFillStyle fillStyle(255 * 255 * 120);
	CSimpleLineStyle lineStyle(120 * 255 * 120);
	CTriangle triangle{ fillStyle, lineStyle, 100, 450, 225, 350, 350, 450 };
	CSimpleFillStyle fillStyle2(100 * 80 * 255);
	CSimpleLineStyle lineStyle2(120 * 80 * 120);
	CRectangle rectangle{ fillStyle2, lineStyle2, 150, 450, 150, 200 };
	CSimpleFillStyle fillStyle3(255 * 255 * 255 * 150);
	CSimpleLineStyle lineStyle3(255 * 255 * 255 * 150);
	CEllipse ellipse{ fillStyle3, lineStyle3, 1300, 80, 200, 200 };
	CSimpleFillStyle fillStyle4(100 * 100 * 100);
	CSimpleLineStyle lineStyle4(100 * 100 * 100);
	CRectangle rectangle2{ fillStyle4, lineStyle4, 0, 800, 1900, 500 };

	std::vector<std::shared_ptr<IShape>> shapes;
	shapes.push_back(std::make_shared<CTriangle>(triangle));
	shapes.push_back(std::make_shared<CRectangle>(rectangle));
	shapes.push_back(std::make_shared<CEllipse>(ellipse));
	shapes.push_back(std::make_shared<CRectangle>(rectangle2));

	/*CGroupShape groupShape(shapes);
	groupShape.Draw(canvas);*/
	CSlide slide(shapes);
	slide.Draw(canvas);
	
	system("pause");
}