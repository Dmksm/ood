#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>
#include <string>

class FlyWithWings : public IFlyBehavior
{
public:
	void Fly() override
	{
		std::cout << "I'm flying with wings!! And my departures number is " 
			<< ++m_departuresNumber << std::endl;
	}
private:
	unsigned m_departuresNumber = 0;
};

#endif
