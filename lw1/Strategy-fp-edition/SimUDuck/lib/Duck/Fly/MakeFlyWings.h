#ifndef MAKEFLYWITHWINGS_H
#define MAKEFLYWITHWINGS_H
#include <iostream>
#include "../Duck.h"
#include "FlyWithWings.h"

Duck::FlyBehavior MakeFlyWithWings()
{
	return [departuresNumber = 0u]() mutable 
		{
			std::cout << "My departures number is " << ++departuresNumber << std::endl;
			FlyWithWings();
		};
}

#endif