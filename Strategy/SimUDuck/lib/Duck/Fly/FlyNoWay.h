#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	std::string Fly(int departuresNumber) override
	{
		return "";
	}
};

#endif
