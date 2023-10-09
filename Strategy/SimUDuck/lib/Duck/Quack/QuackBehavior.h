#ifndef QUACKBEHAVIOR_H
#define QUACKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

class QuackBehavior : public IQuackBehavior
{
public:
	std::string Quack() override
	{
		return "Quack Quack!!!\n";
	}
};

#endif
