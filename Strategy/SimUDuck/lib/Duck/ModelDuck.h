#ifndef MODELDUCK_H
#define MODELDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/QuackBehavior.h"
#include "Dance/DanceNoWay.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<QuackBehavior>(),
			std::make_unique<DanceNoWay>())
	{
	}

	std::string Display() const override
	{
		return "I'm model duck\n";
	}
};

#endif
