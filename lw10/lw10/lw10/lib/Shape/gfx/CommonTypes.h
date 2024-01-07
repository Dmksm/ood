#pragma once
#include "../stdafx.h"
template <typename T>
struct Rect
{
	T left;
	T top;
	T width;
	T height;
};

typedef Rect<double> RectD;