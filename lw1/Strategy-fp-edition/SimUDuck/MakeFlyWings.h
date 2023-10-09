#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H
#include <iostream>
#include <functional>
using FlyBehavior = std::function<void()>;

FlyBehavior MakeFlyWithWings()
{
	unsigned departuresNumber = 0;
	return [departuresNumber]() mutable {
		std::cout << "I'm flying with wings!! And my departures number is " << ++departuresNumber << std::endl;
		};
}

#endif