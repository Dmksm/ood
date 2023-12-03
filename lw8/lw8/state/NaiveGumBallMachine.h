#pragma once
#include "stdafx.h"

namespace naive
{
	class CGumballMachine
	{
	public:
		enum class State
		{
			SoldOut,		// Жвачка закончилась
			NoQuarter,		// Нет монетки
			HasQuarter,		// Есть монетка
			Sold,			// Монетка выдана
		};

		CGumballMachine(unsigned count)
			: m_count(count)
			, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
			, m_quarterCount(0)
		{
		}

		void InsertQuarter()
		{
			switch (m_state)
			{
			case State::SoldOut:
				std::cout << "You can't insert a quarter, the machine is sold out\n";
				break;
			case State::NoQuarter:
				m_quarterCount++;
				std::cout << "You inserted a quarter\n";
				m_state = State::HasQuarter;
				break;
			case State::HasQuarter:
				if (m_quarterCount == MAX_QUARTER_COUNT)
				{
					std::cout << "You can't insert another quarter\n";
				} 
				else
				{
					m_quarterCount++;
					std::cout << "You inserted a quarter\n";
				}
				break;
			case State::Sold:
				if (m_quarterCount < MAX_QUARTER_COUNT)
				{
					++m_quarterCount;
					std::cout << "You inserted a quarter\n";
				}
				else
				{
					std::cout << "Please wait, we're already giving you a gumball\n";
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
				std::cout << "Quarter returned\n";
				m_state = State::NoQuarter;
				break;
			case State::NoQuarter:
				std::cout << "You haven't inserted a quarter\n";
				break;
			case State::Sold:
				std::cout << "Sorry you already turned the crank\n";
				break;
			case State::SoldOut:
				if (m_quarterCount > 0)
				{
					m_quarterCount = 0;
					std::cout << "Quarter returned\n";
				}
				else
				{
					std::cout << "You can't eject, you haven't inserted a quarter yet\n";
				}
				break;
			}
		}

		void TurnCrank()
		{
			switch (m_state)
			{
			case State::SoldOut:
				std::cout << "You turned but there's no gumballs\n";
				break;
			case State::NoQuarter:
				std::cout << "You turned but there's no quarter\n";
				break;
			case State::HasQuarter:
				std::cout << "You turned...\n";
				m_state = State::Sold;
				Dispense();
				break;
			case State::Sold:
				std::cout << "Turning twice doesn't get you another gumball\n";
				break;
			}
		}

		void Refill(unsigned numBalls)
		{
			switch (m_state)
			{
			case State::SoldOut:
				std::cout << "You refilled machine\n";
				m_count = numBalls;
				m_state = m_quarterCount > 0 ? State::HasQuarter : State::NoQuarter;
				break;
			case State::NoQuarter:
			case State::HasQuarter:
				std::cout << "You refilled machine\n";
				m_count = numBalls;
				break;
			case State::Sold:
				std::cout << "You can't refill, you haven't get a gumball yet\n";
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

	private:
		void Dispense()
		{
			switch (m_state)
			{
			case State::Sold:
				--m_quarterCount;
				std::cout << "A gumball comes rolling out the slot\n";
				--m_count;
				if (m_count == 0)
				{
					std::cout << "Oops, out of gumballs\n";
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
				std::cout << "You need to pay first\n";
				break;
			case State::SoldOut:
			case State::HasQuarter:
				std::cout << "No gumball dispensed\n";
				break;
			}
		}
		const unsigned MAX_QUARTER_COUNT = 5;

		unsigned m_count;	// Количество шариков
		unsigned m_quarterCount;	// Количество монеток
		State m_state = State::SoldOut;
	};
}