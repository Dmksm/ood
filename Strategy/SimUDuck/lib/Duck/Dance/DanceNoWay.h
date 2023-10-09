#ifndef DANCENOWAY_H
#define DANCENOWAY_H

#include "IDanceBehavior.h"

class DanceNoWay : public IDanceBehavior
{
public:
	std::string Dance() override 
	{ 
		return "";
	}
};

#endif
