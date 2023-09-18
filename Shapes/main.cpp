#include "lib/Shape/Picture.h"
#include "lib/Shape/CCanvas.h"
#include "lib/Shape/CController.h"
#include "lib/Shape/EllipseDrawingStrategy.h"
#include "lib/Shape/Shape.h"
#include <memory>
#include <iostream>
#include <string>
#include <map>

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