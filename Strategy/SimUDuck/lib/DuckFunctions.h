#ifndef DUCKFUNCTIONS_H
#define DUCKFUNCTIONS_H

#include "Duck/Duck.h"

std::string DrawDuck(Duck const& duck)
{
	return duck.Display();
}

void PlayWithDuck(Duck& duck)
{
	std::cout << DrawDuck(duck) << duck.Quack() << duck.Fly() << duck.Dance() << std::endl;
}

#endif