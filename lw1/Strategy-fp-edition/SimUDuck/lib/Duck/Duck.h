#ifndef DUCK_H
#define DUCK_H

//#include "Fly/FlyBehavior.h"
//#include "Quack/IQuakBehavior.h"
//#include "Dance/IDanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>

class Duck
{
public:
	using FlyBehavior = std::function<void()>; 
	using QuackBehavior = std::function<void()>;
	using DanceBehavior = std::function<void()>;

	Duck(FlyBehavior flyBehavior,
		QuackBehavior quackBehavior,
		DanceBehavior danceBehavior)
		: m_flyBehavior(std::move(flyBehavior))
		, m_quackBehavior(std::move(quackBehavior))
		, m_danceBehavior(std::move(danceBehavior))
	{
	}

	void Quack() const
	{
		m_quackBehavior();
	}

	void Swim()
	{
		"I'm swimming";
	}

	virtual void Fly()
	{
		m_flyBehavior();
	}

	virtual void Dance()
	{
		m_danceBehavior();
	}

	void SetFlyBehavior(const FlyBehavior& flyBehavior)
	{
		m_flyBehavior = flyBehavior;
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	FlyBehavior m_flyBehavior;
	QuackBehavior m_quackBehavior;
	DanceBehavior m_danceBehavior;
};

#endif
