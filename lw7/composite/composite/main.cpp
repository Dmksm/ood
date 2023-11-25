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
	CTriangle triangle{ fillStyle, lineStyle, 100, 100, 150, 250, 100, 400 };
	CRectangle rectangle{ fillStyle, lineStyle, 100, 250, 150, 200 };

	triangle.Draw(canvas);

	/*std::vector<std::shared_ptr<IShape>> shapes;
	shapes.push_back(std::make_shared<CTriangle>(triangle));
	shapes.push_back(std::make_shared<CRectangle>(rectangle));
	CSlide slide(shapes);
	slide.Draw(canvas);*/
	
	system("pause");
}