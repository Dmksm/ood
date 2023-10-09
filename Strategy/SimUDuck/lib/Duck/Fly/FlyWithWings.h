#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>
#include <string>

class FlyWithWings : public IFlyBehavior
{
public:
	std::string Fly(int departuresNumber) override
	{
		return std::string("I'm flying with wings!! And my departures number is ") +
			std::to_string(departuresNumber) + "\n";
	}
};

#endif
