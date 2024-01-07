﻿#include "lib/Shape/shapes/Picture.h"
#include "lib/Shape/controller/CController.h"
#include "lib/Shape/gfx/CCanvas.h"

int main()
{
	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Default, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	Picture picture;
	CController controller(std::make_unique<Picture>(std::move(picture)),
		&renderWindow,
		std::cin, std::cout);

	controller.Execute();

	return 0;
}