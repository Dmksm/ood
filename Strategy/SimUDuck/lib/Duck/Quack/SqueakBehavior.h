#ifndef SQUEAKBEHAVIOR_H
#define SQUEAKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

class SqueakBehavior : public IQuackBehavior
{
public:
	std::string Quack() override
	{
		return "Squeek!!!\n";
	}
};

#endif
