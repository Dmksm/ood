#pragma once
#include "gfx/ICanvas.h"
#include "PictureDraft.h"

class CPainter
{
public:
	void DrawPicture(CPictureDraft& draft, ICanvas& canvas) const;
};	