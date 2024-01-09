#include "lib/Shape/shapes/CPicture.h"
#include "lib/Shape/shapes/CShapeBehaviourFactory.h"
#include "lib/Shape/controller/CController.h"
#include "lib/Shape/gfx/CCanvas.h"

int main()
{
	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	CPicture picture;
	CShapeBehaviourFactory shapeFactory;
	CController controller(std::make_unique<CShapeBehaviourFactory>(std::move(shapeFactory)),
		std::make_unique<CPicture>(std::move(picture)),
		&renderWindow,
		std::cin, std::cout);

	controller.Execute();

	return 0;
}
//выделение должно происходить только при клике по самой фигуре не включая рамку
//треугольник чтобы ресайзился также как остальные фигуры