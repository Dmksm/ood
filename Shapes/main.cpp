#include "lib/Shape/shapes/Picture.h"
#include "lib/Shape/controller/CController.h"

int main()
{
	Picture picture(std::make_unique<Picture::ShapesList>());
	CController controller(std::move(picture), std::cin, std::cout);
	while (!std::cin.eof() && !std::cin.fail())
	{
		std::cout << "> ";
		if (!controller.HandleCommand())
		{
			std::cout << "Unknown command!" << std::endl;
		}
	}
    return 0;
}