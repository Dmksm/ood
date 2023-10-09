#ifndef IFLYBEHAVIOR_H
#define IFLYBEHAVIOR_H

#include <string>
struct IFlyBehavior
{
	virtual ~IFlyBehavior() = default;
	virtual std::string Fly(int departuresNumber) = 0;
};

#endif
