#ifndef DANCEWALTZ_H
#define DANCEWALTZ_H

#include "IDanceBehavior.h"
#include <iostream>

class DanceWaltz : public IDanceBehavior
{
public:
	std::string Dance() override
	{
		return "I'm dancing waltz!!\n";
	}
};

#endif
