#ifndef MUTEQUAKBEHAVIOR_H
#define MUTEQUAKBEHAVIOR_H

#include "IQuakBehavior.h"

class MuteQuackBehavior : public IQuackBehavior
{
public:
	std::string Quack() override 
	{
		return "";
	}
};

#endif
