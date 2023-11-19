#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../state/GumBallMachineWithDynamicallyCreatedState.h"
#include "../state/NaiveGumBallMachine.h"
#include "../state/GumBallMachineWithState.h"
#include <fstream>

class CMockGumballMachine
{
public:
	enum class State
	{
		SoldOut,		// Жвачка закончилась
		NoQuarter,		// Нет монетки
		HasQuarter,		// Есть монетка
		Sold,			// Монетка выдана
	};

	CMockGumballMachine(unsigned count, std::ostream& strm)
		: m_count(count)
		, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
		, m_out(strm)
	{
	}

	void InsertQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			m_out << "You can't insert a quarter, the machine is sold out\n";
			break;
		case State::NoQuarter:
			m_out << "You inserted a quarter\n";
			m_state = State::HasQuarter;
			break;
		case State::HasQuarter:
			m_out << "You can't insert another quarter\n";
			break;
		case State::Sold:
			m_out << "Please wait, we're already giving you a gumball\n";
			break;
		}
	}

	void EjectQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::HasQuarter:
			m_out << "Quarter returned\n";
			m_state = State::NoQuarter;
			break;
		case State::NoQuarter:
			m_out << "You haven't inserted a quarter\n";
			break;
		case State::Sold:
			m_out << "Sorry you already turned the crank\n";
			break;
		case State::SoldOut:
			m_out << "You can't eject, you haven't inserted a quarter yet\n";
			break;
		}
	}

	void TurnCrank()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			m_out << "You turned but there's no gumballs\n";
			break;
		case State::NoQuarter:
			m_out << "You turned but there's no quarter\n";
			break;
		case State::HasQuarter:
			m_out << "You turned...\n";
			m_state = State::Sold;
			Dispense();
			break;
		case State::Sold:
			m_out << "Turning twice doesn't get you another gumball\n";
			break;
		}
	}

	void Refill(unsigned numBalls)
	{
		m_count = numBalls;
		m_state = numBalls > 0 ? State::NoQuarter : State::SoldOut;
	}

	std::string ToString()const
	{
		std::string state =
			(m_state == State::SoldOut) ? "sold out" :
			(m_state == State::NoQuarter) ? "waiting for quarter" :
			(m_state == State::HasQuarter) ? "waiting for turn of crank"
			: "delivering a gumball";
		auto fmt = boost::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: %1% gumball%2%
Machine is %3%
)");
		return (fmt % m_count % (m_count != 1 ? "s" : "") % state).str();
	}

	std::string GetOut()
	{

	}

	std::ostream& m_out;
	void Dispense()
	{
		using namespace std;
		switch (m_state)
		{
		case State::Sold:
			m_out << "A gumball comes rolling out the slot\n";
			--m_count;
			if (m_count == 0)
			{
				m_out << "Oops, out of gumballs\n";
				m_state = State::SoldOut;
			}
			else
			{
				m_state = State::NoQuarter;
			}
			break;
		case State::NoQuarter:
			m_out << "You need to pay first\n";
			break;
		case State::SoldOut:
		case State::HasQuarter:
			m_out << "No gumball dispensed\n";
			break;
		}
	}

	unsigned m_count;	// Количество шариков
	State m_state = State::SoldOut;
};

SCENARIO("Class adapter operations")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss;
		CMockGumballMachine machine(0, ss);
		REQUIRE(machine.m_state == CMockGumballMachine::State::NoQuarter);
		REQUIRE(machine.m_out == "You turned but there's no quarter\n");
		WHEN("Draw on canvas and canvas adapter")
		{
			THEN("get old graphic output and modern graphic output")
			{
			}
		}
	}
}