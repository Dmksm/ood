#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include "../Designer.h"
#include "../gfx/ICanvas.h"

class CController
{
public:
	CController(std::unique_ptr<CDesigner>&& designer,
		std::unique_ptr<ICanvas>&& canvas,
		std::istream& input, std::ostream& output
	)
		: m_designer(std::move(designer))
		, m_canvas(std::move(canvas))
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

	std::unique_ptr<CDesigner> m_designer;
	std::unique_ptr<ICanvas> m_canvas;
	std::istream& m_input;
	std::ostream& m_output;

	const ActionMap m_actionMap = {
		  { "Help", bind(&CController::CommandInfo, this, std::placeholders::_1) },
	};

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