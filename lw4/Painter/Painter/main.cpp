#include "libpainter/Designer.h"
#include "libpainter/controller/CController.cpp"
#include "libpainter/gfx/CCanvas.h"
#include "stdafx.h"

int main()
{
	CDesigner designer(IShapeFactory);

	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	/*CController controller(std::make_unique<CDesigner>(std::move(designer)),
		std::make_unique<CCanvas>(&renderWindow),
		std::cin, std::cout);

	while (!std::cin.eof() && !std::cin.fail())
	{
		std::cout << "> ";
		if (!controller.HandleCommand())
		{
			std::cout << "Unknown command!" << std::endl;
		}
	}*/

	return 0;
}