#pragma once
#include "NaiveGumBallMachine.h"
#include "GumBallMachineWithState.h"
#include "GumBallMachineWithDynamicallyCreatedState.h"
#include "stdafx.h"
class CController
{
public:
	CController(std::unique_ptr<with_state::CGumballMachine>&& gumballMachine,
		std::istream& input, std::ostream& output
	)
		: m_gumballMachine(std::move(gumballMachine))
		, m_input(input)
		, m_output(output)
	{
	}

	bool HandleCommand()
	{
		std::string commandLine;
		std::getline(m_input, commandLine);
		std::istringstream strm(commandLine);

		std::string action;
		strm >> action;

		auto it = m_actionMap.find(action);
		if (it != m_actionMap.end())
		{
			return it->second(strm);
		}

		return false;
	}

private:
	using Handler = std::function<bool(std::istream& args)>;
	using ActionMap = std::map<std::string, Handler>;

	std::unique_ptr<with_state::CGumballMachine> m_gumballMachine;
	std::istream& m_input;
	std::ostream& m_output;

	const ActionMap m_actionMap = {
		  { "Help", bind(&CController::CommandInfo, this, std::placeholders::_1) },
		  { "Refill", bind(&CController::Refill, this, std::placeholders::_1) },
		  { "InsertQuarter", bind(&CController::InsertQuarter, this, std::placeholders::_1) },
		  { "EjectQuarter", bind(&CController::EjectQuarter, this, std::placeholders::_1) },
		  { "TurnCrank", bind(&CController::TurnCrank, this, std::placeholders::_1) },
		  { "ToString", bind(&CController::ToString, this, std::placeholders::_1) },
	};

	bool Refill(std::istream& args)
	{
		try
		{
			std::string gumball;
			args >> gumball;

			m_gumballMachine->Refill(std::stoul(gumball));
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool InsertQuarter(std::istream& args)
	{
		try
		{
			m_gumballMachine->InsertQuarter();
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool EjectQuarter(std::istream& args)
	{
		try
		{
			m_gumballMachine->EjectQuarter();
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool TurnCrank(std::istream& args)
	{
		try
		{
			m_gumballMachine->TurnCrank();
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool ToString(std::istream& args)
	{
		try
		{
			m_output << m_gumballMachine->ToString();
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool CommandInfo(std::istream& args)
	{
		try
		{
			m_output << "Available commands:" << std::endl;
			for (auto& it : m_actionMap)
			{
				m_output << it.first << std::endl;
			}
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}
};