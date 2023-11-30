#include "NaiveGumBallMachine.h"
#include "GumBallMachineWithState.h"
#include "GumBallMachineWithDynamicallyCreatedState.h"
#include "stdafx.h"

template <typename GumballMachineType>
void TestGumballMachine(GumballMachineType& m)
{
	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.EjectQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.EjectQuarter();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;
}

void TestNaiveGumballMachine()
{
	naive::CGumballMachine m(5);
	TestGumballMachine(m);
}

void TestGumballMachineWithState()
{
	with_state::CGumballMachine m(5);
	TestGumballMachine(m);
}

void TestGumballMachineWithDynamicState()
{
	with_dynamic_state::CGumballMachine m(5);
	TestGumballMachine(m);
}

int main()
{
	TestNaiveGumballMachine();

	std::cout << "\n-----------------\n";
	TestGumballMachineWithState();

	std::cout << "\n-----------------\n";
	TestGumballMachineWithDynamicState();

	return 0;
}