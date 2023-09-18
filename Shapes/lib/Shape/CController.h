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
	CController(Picture&& picture, std::istream& input, std::ostream& output)
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
	using Handler = std::function<bool(std::istream& args)>;
	using ActionMap = std::map<std::string, Handler>;

	Picture m_picture;
	std::istream& m_input;
	std::ostream& m_output;

	const ActionMap m_actionMap = {
		  { "Help", bind(&CController::CommandInfo, this, std::placeholders::_1) },
		  { "AddShape", bind(&CController::AddShape, this, std::placeholders::_1) },
		  { "List", bind(&CController::DisplayShapeList, this, std::placeholders::_1) },
		  { "DrawShape", bind(&CController::DrawShape, this, std::placeholders::_1) },
		  { "MoveShape", bind(&CController::MoveShape, this, std::placeholders::_1) },
		  { "DeleteShape", bind(&CController::DeleteShape, this, std::placeholders::_1) },
		  { "ChangeColor", bind(&CController::ChangeColor, this, std::placeholders::_1) },
		  { "ChangeShape", bind(&CController::ChangeShape, this, std::placeholders::_1) },
		  { "DrawPicture", bind(&CController::DrawPicture, this, std::placeholders::_1) },
		  { "MovePicture", bind(&CController::MovePicture, this, std::placeholders::_1) },
		  //{ "CloneShape", bind(&CController::CloneShape, this, std::placeholders::_1) },
	};

	/*bool CloneShape(std::istream& args)
	{
		try
		{
			std::string id, newId;
			args >> id >> newId;

			m_picture.CloneShape(id, newId);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}*/

	bool ChangeShape(std::istream& args)
	{
		try
		{
			std::string id, type;
			args >> id >> type;

			m_picture.ChangeShape(id, type, args);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool DeleteShape(std::istream& args)
	{
		try
		{
			std::string id;
			args >> id;
			m_picture.DeleteShape(id);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool ChangeColor(std::istream& args)
	{
		try
		{
			std::string id, color;
			args >> id >> color;
			m_picture.ChangeColor(id, color);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool MovePicture(std::istream& args)
	{
		try
		{
			double dx, dy;
			args >> dx >> dy;

			m_picture.Move(dx, dy);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool MoveShape(std::istream& args)
	{
		try
		{
			std::string id;
			double dx, dy;
			args >> id >> dx >> dy;

			m_picture.MoveShape(id, dx, dy);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool DisplayShapeList(std::istream& args)
	{
		try 
		{
			for (auto& it : m_picture.GetShapes())
			{
				m_output << it.first << " ";
				it.second.Display();
				m_output << std::endl;
			}
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool DrawShape(std::istream& args)
	{
		try
		{
			std::string id;
			args >> id;

			m_picture.DrawShape(id);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool DrawPicture(std::istream& args)
	{
		try
		{
			m_picture.Draw();
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	}

	bool AddShape(std::istream& args)
	{
		try
		{
			std::string id, hexColor, type;
			args >> id >> hexColor >> type;

			m_picture.AddShape(id, hexColor, type, args);
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
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