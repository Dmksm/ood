#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../state/GumBallMachineWithDynamicallyCreatedState.h"
#include "../state/NaiveGumBallMachine.h"
#include "../state/GumBallMachineWithState.h"
#include "../state/stdafx.h"

namespace with_state_mock
{
	class CMockSoldState : public with_state::IState
	{
	public:
		CMockSoldState(with_state::IGumballMachine& gumballMachine, std::stringstream& ss)
			:m_gumballMachine(gumballMachine)
			, m_out(ss)
		{}
		void InsertQuarter() override
		{
			if (m_gumballMachine.IncrementQuaterCount())
			{
				m_out << "You inserted a quarter\n";
			}
			else
			{
				m_out << "Please wait, we're already giving you a gumball\n";
			}
		}
		void EjectQuarter() override
		{
			m_out << "Sorry you already turned the crank\n";
		}
		void TurnCrank() override
		{
			m_out << "Turning twice doesn't get you another gumball\n";
		}
		void Dispense() override
		{
			m_gumballMachine.ReleaseBall();
			if (m_gumballMachine.GetBallCount() == 0)
			{
				m_out << "Oops, out of gumballs\n";
				m_gumballMachine.SetSoldOutState();
			}
			else
			{
				if (m_gumballMachine.GetQuaterCount() > 0)
				{
					m_gumballMachine.SetHasQuarterState();
				}
				else
				{
					m_gumballMachine.SetNoQuarterState();
				}
			}
		}
		void Refill(unsigned numBalls)
		{
			m_out << "You can't refill, you haven't get a gumball yet\n";
		}
		std::string ToString() const override
		{
			return "delivering a gumball";
		}
	private:
		std::stringstream& m_out;
		with_state::IGumballMachine& m_gumballMachine;
	};

	class CMockSoldOutState : public with_state::IState
	{
	public:
		CMockSoldOutState(with_state::IGumballMachine& gumballMachine, std::stringstream& ss)
			:m_gumballMachine(gumballMachine)
			, m_out(ss)
		{}
		void InsertQuarter() override
		{
			m_out << "You can't insert a quarter, the machine is sold out\n";
		}
		void EjectQuarter() override
		{
			if (m_gumballMachine.GetQuaterCount() > 0)
			{
				m_out << "Quarter returned\n";
				m_gumballMachine.EjectAllQuarters();
			}
			else
			{
				m_out << "You can't eject, you haven't inserted a quarter yet\n";
			}
		}
		void TurnCrank() override
		{
			m_out << "You turned but there's no gumballs\n";
		}
		void Dispense() override
		{
			m_out << "No gumball dispensed\n";
		}
		void Refill(unsigned numBalls)
		{
			m_out << "You refilled machine\n";
			m_gumballMachine.RefillNumballs(numBalls);
			if (m_gumballMachine.GetBallCount() != 0)
			{
				m_gumballMachine.GetQuaterCount() > 0
					? m_gumballMachine.SetHasQuarterState()
					: m_gumballMachine.SetNoQuarterState();
			}
		}
		std::string ToString() const override
		{
			return "sold out";
		}
	private:
		std::stringstream& m_out;
		with_state::IGumballMachine& m_gumballMachine;
	};

	class CMockHasQuarterState : public with_state::IState
	{
	public:
		CMockHasQuarterState(with_state::IGumballMachine& gumballMachine, std::stringstream& ss)
			:m_gumballMachine(gumballMachine)
			, m_out(ss)
		{}
		void InsertQuarter() override
		{
			if (m_gumballMachine.IncrementQuaterCount())
			{
				m_out << "You inserted a quarter\n";
			}
			else
			{
				m_out << "You can't insert another quarter\n";
			}
		}
		void EjectQuarter() override
		{
			m_out << "Quarter returned\n";
			m_gumballMachine.EjectAllQuarters();
			m_gumballMachine.SetNoQuarterState();
		}
		void TurnCrank() override
		{
			m_out << "You turned...\n";
			m_gumballMachine.SetSoldState();
		}
		void Dispense() override
		{
			m_out << "No gumball dispensed\n";
		}
		void Refill(unsigned numBalls)
		{
			m_out << "You refilled machine\n";
			m_gumballMachine.RefillNumballs(numBalls);
			if (m_gumballMachine.GetBallCount() == 0)
			{
				m_gumballMachine.SetSoldOutState();
			}
		}
		std::string ToString() const override
		{
			return "waiting for turn of crank";
		}
	private:
		std::stringstream& m_out;
		with_state::IGumballMachine& m_gumballMachine;
	};

	class CMockNoQuarterState : public with_state::IState
	{
	public:
		CMockNoQuarterState(with_state::IGumballMachine& gumballMachine, std::stringstream& ss)
			: m_gumballMachine(gumballMachine)
			, m_out(ss)
		{}
		void InsertQuarter() override
		{
			m_out << "You inserted a quarter\n";
			m_gumballMachine.SetHasQuarterState();
		}
		void EjectQuarter() override
		{
			m_out << "You haven't inserted a quarter\n";
		}
		void TurnCrank() override
		{
			m_out << "You turned but there's no quarter\n";
		}
		void Dispense() override
		{
			m_out << "You need to pay first\n";
		}
		void Refill(unsigned numBalls)
		{
			m_out << "You refilled machine\n";
			m_gumballMachine.RefillNumballs(numBalls);
			if (m_gumballMachine.GetBallCount() == 0)
			{
				m_gumballMachine.SetSoldOutState();
			}
		}
		std::string ToString() const override
		{
			return "waiting for quarter";
		}
	private:
		std::stringstream& m_out;
		with_state::IGumballMachine& m_gumballMachine;
	};

	class CMockGumballMachine : private with_state::IGumballMachine
	{
	public:
		CMockGumballMachine(unsigned numBalls, std::stringstream& ss)
			: m_soldState(*this, ss)
			, m_soldOutState(*this, ss)
			, m_noQuarterState(*this, ss)
			, m_hasQuarterState(*this, ss)
			, m_state(&m_soldOutState)
			, m_count(numBalls)
			, m_out(ss)
		{
			if (m_count > 0)
			{
				m_state = &m_noQuarterState;
			}
		}
		void Refill(unsigned numBalls)
		{
			m_state->Refill(numBalls);
		}
		void EjectQuarter()
		{
			m_state->EjectQuarter();
		}
		void InsertQuarter()
		{
			m_state->InsertQuarter();
		}
		void TurnCrank()
		{
			m_state->TurnCrank();
			m_state->Dispense();
		}
		std::string ToString()const
		{
			auto fmt = boost::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: %1% gumball%2%
Machine is %3%
)");
			return (fmt % m_count % (m_count != 1 ? "s" : "") % m_state->ToString()).str();
		}

		const static unsigned MAX_QUARTER_COUNT = 5;
		unsigned m_quaterCount = 0;
		unsigned m_count = 0;
		std::stringstream& m_out;
		with_state::IState* m_state;

		unsigned GetBallCount() const override
		{
			return m_count;
		}
		virtual void ReleaseBall() override
		{
			if (m_count != 0)
			{
				m_out << "A gumball comes rolling out the slot...\n";
				--m_count;
			}
		}
		void SetSoldOutState() override
		{
			m_state = &m_soldOutState;
		}
		void SetNoQuarterState() override
		{
			m_state = &m_noQuarterState;
		}
		void SetSoldState() override
		{
			m_state = &m_soldState;
		}
		void SetHasQuarterState() override
		{
			m_state = &m_hasQuarterState;
		}

		bool DecrementQuaterCount() override
		{
			if (m_quaterCount == 0)
			{
				return false;
			}
			m_quaterCount--;
			return true;
		};

		bool IncrementQuaterCount() override
		{
			if (m_quaterCount == MAX_QUARTER_COUNT)
			{
				return false;
			}
			m_quaterCount++;
			return true;
		};

		unsigned GetQuaterCount() override
		{
			return m_quaterCount;
		}

		void EjectAllQuarters() override
		{
			m_quaterCount = 0;
		}

		void RefillNumballs(unsigned numBalls) override
		{
			m_count = numBalls;
		}

		CMockSoldState m_soldState;
		CMockSoldOutState m_soldOutState;
		CMockNoQuarterState m_noQuarterState;
		CMockHasQuarterState m_hasQuarterState;
	};

}

namespace naive_mock
{
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

		CMockGumballMachine(unsigned count, std::stringstream& strm)
			: m_count(count)
			, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
			, m_quarterCount(0)
			, m_out(strm)
		{
		}

		void InsertQuarter()
		{
			switch (m_state)
			{
			case State::SoldOut:
				m_out << "You can't insert a quarter, the machine is sold out\n";
				break;
			case State::NoQuarter:
				m_quarterCount++;
				m_out << "You inserted a quarter\n";
				m_state = State::HasQuarter;
				break;
			case State::HasQuarter:
				if (m_quarterCount == MAX_QUARTER_COUNT)
				{
					m_out << "You can't insert another quarter\n";
				}
				else
				{
					m_quarterCount++;
					m_out << "You inserted a quarter\n";
				}
				break;
			case State::Sold:
				if (m_quarterCount < MAX_QUARTER_COUNT)
				{
					++m_quarterCount;
					m_out << "You inserted a quarter\n";
				}
				else
				{
					m_out << "Please wait, we're already giving you a gumball\n";
				}
				break;
			}
		}

		void EjectQuarter()
		{
			switch (m_state)
			{
			case State::HasQuarter:
				m_quarterCount = 0;
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
				if (m_quarterCount > 0)
				{
					m_quarterCount = 0;
					m_out << "Quarter returned\n";
				}
				else
				{
					m_out << "You can't eject, you haven't inserted a quarter yet\n";
				}
				break;
			}
		}

		void TurnCrank()
		{
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
			switch (m_state)
			{
			case State::SoldOut:
				m_out << "You refilled machine\n";
				m_count = numBalls;
				if (numBalls > 0)
				{
					m_state = m_quarterCount > 0 ? State::HasQuarter : State::NoQuarter;
				}
				break;
			case State::NoQuarter:
			case State::HasQuarter:
				m_out << "You refilled machine\n";
				m_count = numBalls;
				if (numBalls == 0)
				{
					m_state = State::SoldOut;
				}
				break;
			case State::Sold:
				m_out << "You can't refill, you haven't get a gumball yet\n";
				break;
			}
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
		std::stringstream& m_out;
		const unsigned MAX_QUARTER_COUNT = 5;

		unsigned m_count;	// Количество шариков
		unsigned m_quarterCount;	// Количество монеток
		State m_state = State::SoldOut;

		void Dispense()
		{
			switch (m_state)
			{
			case State::Sold:
				--m_quarterCount;
				m_out << "A gumball comes rolling out the slot\n";
				--m_count;
				if (m_count == 0)
				{
					m_out << "Oops, out of gumballs\n";
					m_state = State::SoldOut;
				}
				else
				{
					if (m_quarterCount > 0)
					{
						m_state = State::HasQuarter;
					}
					else
					{
						m_state = State::NoQuarter;
					}
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
	};
}

SCENARIO("Work with SoldOut state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		naive_mock::CMockGumballMachine machine(0, ss);
		WHEN("State is SoldOut")
		{
			REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);
			REQUIRE(machine.m_out.str() == "");
			THEN("All method working without erorrs")
			{
				machine.Dispense();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				machine.TurnCrank();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				machine.EjectQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				comparedStream 
					<< "No gumball dispensed\n" 
					<< "You turned but there's no gumballs\n"
					<< "You can't eject, you haven't inserted a quarter yet\n" 
					<< "You can't insert a quarter, the machine is sold out\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
		WHEN("Refill machine with quater")
		{
			machine.m_quarterCount = 2;
			REQUIRE(machine.m_quarterCount > 0);
			THEN("State will be has quater or sold out")
			{
				machine.Refill(0);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				machine.Refill(1);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				machine.Refill(0);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);
			}
		}
		WHEN("Refill machine without quater")
		{
			machine.EjectQuarter();
			REQUIRE(machine.m_quarterCount == 0);
			THEN("State will be no quater or sold out")
			{
				machine.Refill(0);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				machine.Refill(1);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.Refill(0);
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);
			}
		}
	}
}

SCENARIO("Work with NoQuarter state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		naive_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is NoQuarter")
		{
			REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);
			THEN("All method working without erorrs")
			{
				machine.Dispense();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.TurnCrank();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.EjectQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				comparedStream 
					<< "You need to pay first\n" 
					<< "You turned but there's no quarter\n"
					<< "You haven't inserted a quarter\n"
					<< "You inserted a quarter\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}

SCENARIO("Work with HasQuarter state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		naive_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is HasQuarter")
		{
			machine.InsertQuarter();
			REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);
			THEN("All method working without erorrs")
			{
				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				machine.EjectQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				machine.Dispense();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				machine.TurnCrank();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::HasQuarter);

				machine.TurnCrank();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::SoldOut);

				comparedStream
					<< "You inserted a quarter\n"
					<< "You inserted a quarter\n"
					<< "Quarter returned\n"
					<< "You inserted a quarter\n"
					<< "No gumball dispensed\n"
					<< "You turned...\n"
					<< "A gumball comes rolling out the slot\n"
					<< "You inserted a quarter\n"
					<< "You turned...\n"
					<< "A gumball comes rolling out the slot\n"
					<< "Oops, out of gumballs\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}

SCENARIO("Work with Sold state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		naive_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is Sold")
		{
			machine.m_state = naive_mock::CMockGumballMachine::State::Sold;
			REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::Sold);
			THEN("All method working without erorrs")
			{
				machine.EjectQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::Sold);

				machine.InsertQuarter();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::Sold);

				machine.TurnCrank();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::Sold);

				machine.Dispense();
				REQUIRE(machine.m_state == naive_mock::CMockGumballMachine::State::NoQuarter);

				comparedStream
					<< "Sorry you already turned the crank\n"
					<< "You inserted a quarter\n"
					<< "Turning twice doesn't get you another gumball\n"
					<< "A gumball comes rolling out the slot\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}







SCENARIO("Machine with state: Work with SoldOut state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		with_state_mock::CMockGumballMachine machine(0, ss);
		WHEN("State is SoldOut")
		{
			REQUIRE(machine.m_state->ToString() == "sold out");
			REQUIRE(machine.m_out.str() == "");
			THEN("All method working without erorrs")
			{
				machine.TurnCrank();
				REQUIRE(machine.m_state->ToString() == "sold out");

				machine.EjectQuarter();
				REQUIRE(machine.m_state->ToString() == "sold out");

				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "sold out");

				comparedStream
					<< "You turned but there's no gumballs\n"
					<< "No gumball dispensed\n"
					<< "You can't eject, you haven't inserted a quarter yet\n"
					<< "You can't insert a quarter, the machine is sold out\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}

SCENARIO("Machine with state: Work with NoQuarter state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		with_state_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is NoQuarter")
		{
			REQUIRE(machine.m_state->ToString() == "waiting for quarter");
			THEN("All method working without erorrs")
			{
				machine.TurnCrank();
				REQUIRE(machine.m_state->ToString() == "waiting for quarter");

				machine.EjectQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for quarter");

				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");

				comparedStream
					<< "You turned but there's no quarter\n"
					<< "You need to pay first\n"
					<< "You haven't inserted a quarter\n"
					<< "You inserted a quarter\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}

SCENARIO("Machine with state: Work with HasQuarter state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		with_state_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is HasQuarter")
		{
			machine.InsertQuarter();
			REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");
			THEN("All method working without erorrs")
			{
				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");

				machine.EjectQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for quarter");

				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");

				machine.TurnCrank();
				REQUIRE(machine.m_state->ToString() == "waiting for quarter");

				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");

				machine.TurnCrank();
				REQUIRE(machine.m_state->ToString() == "sold out");

				comparedStream
					<< "You inserted a quarter\n"
					<< "You inserted a quarter\n"
					<< "Quarter returned\n"
					<< "You inserted a quarter\n"
					<< "You turned...\n"
					<< "A gumball comes rolling out the slot...\n"
					<< "You inserted a quarter\n"
					<< "You turned...\n"
					<< "A gumball comes rolling out the slot...\n"
					<< "Oops, out of gumballs\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}

SCENARIO("Machine with state: Work with Sold state")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss, comparedStream;
		with_state_mock::CMockGumballMachine machine(2, ss);
		WHEN("State is Sold")
		{
			machine.SetSoldState();
			REQUIRE(machine.m_state->ToString() == "delivering a gumball");
			THEN("All method working without erorrs")
			{
				machine.EjectQuarter();
				REQUIRE(machine.m_state->ToString() == "delivering a gumball");

				machine.InsertQuarter();
				REQUIRE(machine.m_state->ToString() == "delivering a gumball");

				machine.TurnCrank();
				REQUIRE(machine.m_state->ToString() == "waiting for turn of crank");

				comparedStream
					<< "Sorry you already turned the crank\n"
					<< "You inserted a quarter\n"
					<< "Turning twice doesn't get you another gumball\n"
					<< "A gumball comes rolling out the slot...\n";
				REQUIRE(machine.m_out.str() == comparedStream.str());
			}
		}
	}
}