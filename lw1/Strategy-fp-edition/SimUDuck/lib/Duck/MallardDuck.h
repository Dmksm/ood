#include "Duck.h"
#include "Fly/FlyWithWings.h"
#include "Fly/MakeFlyWings.h"
#include "Quack/QuackBehavior.h"
#include "Dance/DanceWaltz.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(MakeFlyWithWings()
		, QuackQuack
		, DanceWaltz
	)
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};