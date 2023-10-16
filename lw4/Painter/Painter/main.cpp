#include "libpainter/Designer.h"
#include "libpainter/CShapeFactory.h"
#include "libpainter/gfx/CCanvas.h"
#include "libpainter/PictureDraft.h"
#include "stdafx.h"

int main()
{	
	CShapeFactory factory;
	CDesigner designer(factory);

	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The Shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));

	while (!std::cin.eof() && !std::cin.fail())
	{
		designer.CreateDraft(std::cin);
	}

	return 0;
}