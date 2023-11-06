#include "stdafx.h"
#include "App.h"
#include "AppProEdition.h"

int main()
{
	std::cout << "Should we use new API (y)?";
	std::string userInput;
	if (getline(std::cin, userInput) && (userInput == "y" || userInput == "Y"))
	{
		app::PaintPictureOnModernGraphicsRenderer();
		std::cout << "-------------PRO-------------" << std::endl;
		app_pro::PaintPictureOnModernGraphicsRenderer();
	}
	else
	{
		app::PaintPictureOnCanvas();
		std::cout << "-------------PRO-------------" << std::endl;
		app_pro::PaintPictureOnCanvas();
	}

	return 0;
}