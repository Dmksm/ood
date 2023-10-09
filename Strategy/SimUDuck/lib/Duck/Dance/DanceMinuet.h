#ifndef DANCEMINUET_H
#define DANCEMINUET_H

#include "IDanceBehavior.h"
#include <iostream>

class DanceMinuet : public IDanceBehavior
{
public:
	std::string Dance() override
	{
		return "I'm dancing minuet!!\n";
	}
};

#endif
