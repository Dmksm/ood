#include "libpainter/Designer.h"
#include "libpainter/controller/CController.cpp"
#include "libpainter/gfx/CCanvas.h"
#include "stdafx.h"

int main()
{
	CDesigner designer(IShapeFactory);

	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));

	while (!std::cin.eof() && !std::cin.fail())
	{
		designer->CreateDraft();
	}

	return 0;
}