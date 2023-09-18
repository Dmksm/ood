#include "lib/Shape/shapes/Picture.h"
#include "lib/Shape/controller/CController.h"
#include "lib/Shape/gfx/CCanvas.h"

void renderingThread(CController* controller)
{

	while (!std::cin.eof() && !std::cin.fail())
	{
		std::cout << "> ";
		if (!controller->HandleCommand())
		{
			std::cout << "Unknown command!" << std::endl;
		}
	}
}

int main()
{
	const unsigned WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1080, FRAME_RATE = 145;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "The Shape",
		sf::Style::Default, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	window.setActive(false);
	window.draw(sf::RectangleShape({ 100,100 }));


	Picture picture(std::make_unique<Picture::ShapesList>());

	CController controller(std::make_unique<Picture>(std::move(picture)),
		std::make_unique<CCanvas>(&window),
		std::cin, std::cout);

	// launch the rendering thread
	sf::Thread thread(&renderingThread, &controller);
	thread.launch();


	window.setActive(true);
	// the rendering loop
	while (window.isOpen())
	{
		// draw...

		window.setActive(true);
		// end the current frame -- this is a rendering function (it requires the context to be active)
		window.display();
	}
	
    return 0;
}