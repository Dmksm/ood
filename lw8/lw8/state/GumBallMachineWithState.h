#pragma once
#include "stdafx.h"

namespace with_state
{
	struct IState
	{
		virtual void Refill(unsigned numBalls) = 0;
		virtual void InsertQuarter() = 0;
		virtual void EjectQuarter() = 0;
		virtual void TurnCrank() = 0;
		virtual void Dispense() = 0;
		virtual std::string ToString()const = 0;
		virtual ~IState() = default;
	};

	struct IGumballMachine
	{
		virtual void RefillNumballs(unsigned numBalls) = 0;
		virtual void ReleaseBall() = 0;
		virtual unsigned GetBallCount()const = 0;

		virtual void SetSoldOutState() = 0;
		virtual void SetNoQuarterState() = 0;
		virtual void SetSoldState() = 0;
		virtual void SetHasQuarterState() = 0;
		virtual bool DecrementQuaterCount() = 0;
		virtual bool IncrementQuaterCount() = 0;
		virtual unsigned GetQuaterCount() = 0;
		virtual void EjectAllQuarters() = 0;

		virtual ~IGumballMachine() = default;
	};

	class CSoldState : public IState
	{
	public:
		CSoldState(IGumballMachine& gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}
		void InsertQuarter() override
		{
			if (m_gumballMachine.IncrementQuaterCount())
			{
				std::cout << "You inserted a quarter\n";
			}
			else
			{
				std::cout << "Please wait, we're already giving you a gumball\n";
			}
		}
		void EjectQuarter() override
		{
			std::cout << "Sorry you already turned the crank\n";
		}
		void TurnCrank() override
		{
			std::cout << "Turning twice doesn't get you another gumball\n";
		}
		void Dispense() override
		{
			m_gumballMachine.ReleaseBall();
			if (m_gumballMachine.GetBallCount() == 0)
			{
				std::cout << "Oops, out of gumballs\n";
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
			std::cout << "You can't refill, you haven't get a gumball yet\n";
		}
		std::string ToString() const override
		{
			return "delivering a gumball";
		}
	private:
		IGumballMachine& m_gumballMachine;
	};

	class CSoldOutState : public IState
	{
	public:
		CSoldOutState(IGumballMachine& gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			std::cout << "You can't insert a quarter, the machine is sold out\n";
		}
		void EjectQuarter() override
		{
			if (m_gumballMachine.GetQuaterCount() > 0)
			{
				std::cout << "Quarter returned\n";
				m_gumballMachine.EjectAllQuarters();
			}
			else
			{
				std::cout << "You can't eject, you haven't inserted a quarter yet\n";
			}
		}
		void TurnCrank() override
		{
			std::cout << "You turned but there's no gumballs\n";
		}
		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}
		void Refill(unsigned numBalls)
		{
			std::cout << "You refilled machine\n";
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
		IGumballMachine& m_gumballMachine;
	};

	class CHasQuarterState : public IState
	{
	public:
		CHasQuarterState(IGumballMachine& gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			if (m_gumballMachine.IncrementQuaterCount())
			{
				std::cout << "You inserted a quarter\n";
			}
			else
			{
				std::cout << "You can't insert another quarter\n";
			}
		}
		void EjectQuarter() override
		{
			std::cout << "Quarter returned\n";
			m_gumballMachine.EjectAllQuarters();
			m_gumballMachine.SetNoQuarterState();
		}
		void TurnCrank() override
		{
			std::cout << "You turned...\n";
			m_gumballMachine.SetSoldState();
		}
		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}
		void Refill(unsigned numBalls)
		{
			std::cout << "You refilled machine\n";
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
		IGumballMachine& m_gumballMachine;
	};

	class CNoQuarterState : public IState
	{
	public:
		CNoQuarterState(IGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			std::cout << "You inserted a quarter\n";
			m_gumballMachine.SetHasQuarterState();
		}
		void EjectQuarter() override
		{
			std::cout << "You haven't inserted a quarter\n";
		}
		void TurnCrank() override
		{
			std::cout << "You turned but there's no quarter\n";
		}
		void Dispense() override
		{
			std::cout << "You need to pay first\n";
		}
		void Refill(unsigned numBalls)
		{
			std::cout << "You refilled machine\n";
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
		IGumballMachine& m_gumballMachine;
	};

	class CGumballMachine : private IGumballMachine
	{
	public:
		CGumballMachine(unsigned numBalls)
			: m_soldState(*this)
			, m_soldOutState(*this)
			, m_noQuarterState(*this)
			, m_hasQuarterState(*this)
			, m_state(&m_soldOutState)
			, m_count(numBalls)
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
	private:
		unsigned GetBallCount() const override
		{
			return m_count;
		}
		virtual void ReleaseBall() override
		{
			if (m_count != 0)
			{
				std::cout << "A gumball comes rolling out the slot...\n";
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
	private:
		const static unsigned MAX_QUARTER_COUNT = 5;
		unsigned m_quaterCount = 0;
		unsigned m_count = 0;
		CSoldState m_soldState;
		CSoldOutState m_soldOutState;
		CNoQuarterState m_noQuarterState;
		CHasQuarterState m_hasQuarterState;
		IState* m_state;
	};
}