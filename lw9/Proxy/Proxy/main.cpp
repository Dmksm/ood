#include "stdafx.h"
#include "drawer.h"

constexpr int STAR_COUNT = 20;

int main()
{
	{
		Image img = LoadImage(
			" CCCC             \n"
			"CC  CC   ##    ## \n"
			"CC      ####  ####\n"
			"CC  CC   ##    ## \n"
			" CCCC             \n");
		Print(img, std::cout);
	}

	{
		Image img{ { 70, 20 }, '.' };

		DrawLine(img, { 1, 10 }, { 5, 1 }, '#');
		DrawLine(img, { 5, 1 }, { 11, 10 }, '#');
		DrawLine(img, { 1, 10 }, { 11, 10 }, '#');
		DrawLine(img, { 1, 10 }, { 1, 20 }, '#');
		DrawLine(img, { 11, 10 }, { 11, 20 }, '#');
		DrawLine(img, { 1, 19 }, { 11, 19 }, '#');
		DrawCircle(img, { 60, 5 }, 4, '@');
		FillCircle(img, { 60, 5 }, 2, '&');

		Print(img, std::cout);
	}
}