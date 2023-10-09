#ifndef IDANCEBEHAVIOR_H
#define IDANCEBEHAVIOR_H

struct IDanceBehavior
{
	virtual ~IDanceBehavior() {};
	virtual std::string Dance() = 0;
};

#endif
