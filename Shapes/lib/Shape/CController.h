#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include "Picture.h"

class CController
{
public:
	CController(Picture& picture, std::istream& input, std::ostream& output)
		: m_picture(std::move(picture))
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
	bool DrawShape(std::istream& args)
	{
		std::string id;
		args >> id;
		if (!m_picture.IsShapeExist(id))
		{
			std::cout << "Undefined shape with id = " << id << ". ";
			return false;
		}

		m_picture.DrawShape(id);

		return true;
	}

	bool AddShape(std::istream& args)
	{
		std::string id, hexColor, type;
		args >> id >> hexColor >> type;
		if (m_picture.IsShapeExist(id))
		{
			std::cout << "Shape with id = " << id << " already exist! ";
			return false;
		}

		m_picture.AddShape(id, hexColor, type, args);

		return true;
	};

	bool CommandInfo(std::istream& args)
	{
		m_output << "Available commands:" << std::endl;
		for (auto& it : m_actionMap)
		{
			m_output << it.first << std::endl;
		}

		return true;
	}

	using Handler = std::function<bool(std::istream& args)>;
	using ActionMap = std::map<std::string, Handler>;

	Picture m_picture;
	std::istream& m_input;
	std::ostream& m_output;

	const ActionMap m_actionMap = {
		  { "AddShape", bind(&CController::AddShape, this, std::placeholders::_1) },
		  { "Help", bind(&CController::CommandInfo, this, std::placeholders::_1) },
		  { "DrawShape", bind(&CController::DrawShape, this, std::placeholders::_1) },
	};
};