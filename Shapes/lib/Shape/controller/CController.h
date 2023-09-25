#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include "../shapes/Picture.h"
#include "../gfx/ICanvas.h"

class CController
{
public:
	CController(std::unique_ptr<Picture>&& picture,
		std::unique_ptr<ICanvas>&& canvas,
		std::istream& input, std::ostream& output
	)
		: m_picture(std::move(picture))
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

	std::unique_ptr<Picture> m_picture;
	std::unique_ptr<ICanvas> m_canvas;
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

			m_picture->ChangeShape(id, MakeDrawingStrategy(type, args));
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
			m_picture->DeleteShape(id);
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
			m_picture->GetShape(id)->SetColor(color);
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

			for (auto& it : m_picture->GetShapes())
			{
				it.second->Move(dx, dy);
			}
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

			m_picture->GetShape(id)->Move(dx, dy);
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
			unsigned positionNumber = 0;
			for (auto& it : m_picture->GetShapes())
			{
				m_output << ++positionNumber << " ";
				it.second->Display();
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

			m_picture->GetShape(id)->Draw(*m_canvas);
			m_canvas->Display();
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
			for (auto& it : m_picture->GetShapes())
			{
				it.second->Draw(*m_canvas);
			}
			m_canvas->Display();
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
			
			m_picture->AddShape(std::make_unique<Shape>(MakeDrawingStrategy(type, args), id, hexColor));
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

	std::unique_ptr<EllipseDrawingStrategy> MakeEllipseStrategy(std::istream& args)
	{
		double x, y, radius;
		args >> x >> y >> radius;
		if (radius < 0)
		{
			throw std::logic_error("Radius can not be negative!");
		}
		return std::make_unique<EllipseDrawingStrategy>(x, y, radius, radius);
	}

	std::unique_ptr<RectangleDrawingStrategy> MakeRectangleStrategy(std::istream& args)
	{
		double x, y, width, heigth;
		args >> x >> y >> width >> heigth;
		if (width < 0 || heigth < 0)
		{
			throw std::logic_error("width and heigth can not be negative!");
		}

		return std::make_unique<RectangleDrawingStrategy>(x, y, width, heigth);
	}

	std::unique_ptr<LineDrawingStrategy> MakeLineStrategy(std::istream& args)
	{
		double x1, y1, x2, y2;
		args >> x1 >> y1 >> x2 >> y2;

		return std::make_unique<LineDrawingStrategy>(x1, y1, x2, y2);
	}

	std::unique_ptr<TriangleDrawingStrategy> MakeTriangleStrategy(std::istream& args)
	{
		double x1, y1, x2, y2, x3, y3;
		args >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

		return std::make_unique<TriangleDrawingStrategy>(x1, y1, x2, y2, x3, y3);
	}

	std::unique_ptr<TextDrawingStrategy> MakeTextStrategy(std::istream& args)
	{
		double width, heigth, fontSize;
		std::string text;
		args >> width >> heigth >> fontSize;
		getline(args, text);
		if (width < 0 || heigth < 0 || fontSize < 0)
		{
			throw std::logic_error("width and heigth and font size can not be negative!");
		}

		return std::make_unique<TextDrawingStrategy>(width, heigth, fontSize, text);
	}

	std::unique_ptr<IDrawingStrategy> MakeDrawingStrategy(
		const std::string& type,
		std::istream& args
	)
	{
		if (type == "circle")
		{
			return MakeEllipseStrategy(args);
		}
		else if (type == "rectangle")
		{
			return MakeRectangleStrategy(args);
		}
		else if (type == "line")
		{
			return MakeLineStrategy(args);
		}
		else if (type == "triangle")
		{
			return MakeTriangleStrategy(args);
		}
		else if (type == "text")
		{
			return MakeTextStrategy(args);
		}
		else
		{
			throw std::logic_error("Undefined type " + type);
		}
	}
};