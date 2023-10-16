#include "CPainter.h"

void CPainter::DrawPicture(CPictureDraft& draft, ICanvas& canvas) const
{
	for (auto& it : draft)
	{
		it.Draw(canvas);
	}
	canvas.Display();
}