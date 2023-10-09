#ifndef IFLYBEHAVIOR_H
#define IFLYBEHAVIOR_H

#include <string>
struct IFlyBehavior
{
	virtual ~IFlyBehavior() = default;
	virtual void Fly() = 0;
};

#endif
