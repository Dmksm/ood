#include "libpainter/Designer.h"
#include "libpainter/CShapeFactory.h"
#include "libpainter/gfx/CCanvas.h"
#include "libpainter/PictureDraft.h"
#include "libpainter/CPainter.h"
#include "stdafx.h"

int main()
{	
	CShapeFactory factory;
	CDesigner designer(factory);

	sf::RenderWindow renderWindow(sf::VideoMode::getDesktopMode(), "The factory shape",
		sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16, 4, 6, 0, true));
	CCanvas canvas(&renderWindow);

	CPainter painter;
	while (!std::cin.eof() && !std::cin.fail())
	{
		CPictureDraft draft = designer.CreateDraft(std::cin);
		painter.DrawPicture(draft, canvas);
		canvas.Display();
	}

	return 0;
}