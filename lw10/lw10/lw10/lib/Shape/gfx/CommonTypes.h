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

template <typename T>
struct Point
{
	T x;
	T y;
};

typedef Point<double> PointD;