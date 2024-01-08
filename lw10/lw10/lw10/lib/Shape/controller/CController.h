#pragma once
#include "../stdafx.h"
#include "../shapes/Picture.h"
#include "../gfx/ICanvas.h"

class CController
{
public:
	CController(std::unique_ptr<Picture>&& picture,
		sf::RenderWindow* renderWindow,
		std::istream& input, std::ostream& output
	)
		: m_picture(std::move(picture))
		, m_window(renderWindow)
		, m_canvas(std::move(std::make_shared<CCanvas>(renderWindow)))
		, m_input(input)
		, m_output(output)
	{
	}

	void Execute()
	{
		DrawWidgetPanel();
		RectD rectFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Rectangle);
		RectD triangleFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Triangle);
		RectD ellipseFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Ellipse);
		float prevMouseX = 0;
		float prevMouseY = 0;
		double dx = 0;
		double dy = 0;
		bool isShapeMoving = false;
		bool isShapeResizing = false;
		PointD staticPoint = { 0, 0 };
		while (m_window->isOpen())
		{
			bool isMouseOnShape = false;
			bool isMouseOnSelectionMarker = false;
			sf::Event event;
			while (m_window->pollEvent(event))
			{
				switch (event.type)
				{
					case sf::Event::MouseButtonPressed:
					{
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							float x = event.mouseButton.x;
							float y = event.mouseButton.y;

							auto shapeID = GetShapeIDByCoords(x, y);
							if (shapeID.has_value())
							{
								isMouseOnShape = true;
								m_activeShapeID = shapeID.value();
								isShapeMoving = true;
								prevMouseX = x;
								prevMouseY = y;
							}

							std::vector<RectD> selectionFrames;
							if (m_activeShapeID.has_value())
							{
								RectD frame = m_picture->GetShape(m_activeShapeID.value())->GetFrame();
								selectionFrames = m_canvas->GetSelectionMarkerFrame(frame);
							}

							int index = 0;
							for (RectD& frame : selectionFrames)
							{
								if (IsInFrame(x, y, frame))
								{
									isMouseOnSelectionMarker = true;
									isShapeResizing = true;
									index = (index + 2) % 4;
									RectD staticFrame = *(selectionFrames.begin() + index);
									staticPoint = {
										staticFrame.left + staticFrame.width / 2,
										staticFrame.top + staticFrame.height / 2
									};
									prevMouseX = x;
									prevMouseY = y;
									break;
								}
								++index;
							}

							if (IsInFrame(x, y, rectFrame))
							{
								std::string type = "rectangle";
								boost::uuids::uuid uuid = boost::uuids::random_generator()();
								AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_RECTANGLE_ARGS);
							}
							if (IsInFrame(x, y, triangleFrame))
							{
								std::string type = "triangle";
								boost::uuids::uuid uuid = boost::uuids::random_generator()();
								AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_TRIANGLE_ARGS);
							}
							if (IsInFrame(x, y, ellipseFrame))
							{
								std::string type = "circle";
								boost::uuids::uuid uuid = boost::uuids::random_generator()();
								AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_ELLIPSE_ARGS);
							}

							if (!(isMouseOnShape || isMouseOnSelectionMarker))
							{
								m_activeShapeID = std::nullopt;
								isShapeMoving = false;
							}
						}
						break;
					case sf::Event::MouseMoved:
					{
						if ((isShapeMoving || isShapeResizing) && m_activeShapeID.has_value())
						{
							RectD border = m_canvas->GetWorkSpaceFrameBorder();
							double currX = event.mouseMove.x;
							double currY = event.mouseMove.y;
							RectD activeFrame = m_picture->GetShape(m_activeShapeID.value())->GetFrame();
							dx = currX - prevMouseX;
							dy = currY - prevMouseY;
							if (activeFrame.left + dx < border.left)
							{
								dx = border.left - activeFrame.left;
							}
							if (activeFrame.left + activeFrame.width + dx > border.left + border.width)
							{
								dx = (border.left + border.width) - (activeFrame.left + activeFrame.width);
							}
							if (activeFrame.top + dy < border.top)
							{
								dy = border.top - activeFrame.top;
							}
							if (activeFrame.top + activeFrame.height + dy > border.top + border.height)
							{
								dy = (border.top + border.height) - (activeFrame.top + activeFrame.height);
							}

							prevMouseX = currX;
							prevMouseY = currY;
							if (isShapeResizing)
							{
								double left = std::min(staticPoint.x, (double)event.mouseMove.x);
								double top = std::min(staticPoint.y, (double)event.mouseMove.y);
								double width = std::abs(left - std::max(staticPoint.x, (double)event.mouseMove.x));
								double height = std::abs(top - std::max(staticPoint.y, (double)event.mouseMove.y));
								m_picture->GetShape(m_activeShapeID.value())->SetFrame({ left, top, width, height });

								RectD frame = m_picture->GetShape(m_activeShapeID.value())->GetFrame();
								if (frame.left < border.left)
								{
									frame.left = border.left;
								}
								if (frame.top < border.top)
								{
									frame.top = border.top;
								}
								if (frame.left + frame.width > border.left + border.width)
								{
									frame.width = (border.left + border.width) - frame.left;
								}
								if (frame.top + frame.height > border.top + border.height)
								{
									frame.height = (border.top + border.height) - frame.top;
								}
								m_picture->GetShape(m_activeShapeID.value())->SetFrame(frame);
							} 
							else
							{
								if (isShapeMoving)
								{
									MoveShape(m_activeShapeID.value(), dx, dy);
								}
							}
						}
						break;
					}
					case sf::Event::MouseButtonReleased:
					{
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							isShapeMoving = false;
							isShapeResizing = false;
						}
						break;
					}
					case sf::Event::Closed:
					{
						m_window->close();
						break;
					}
					case sf::Event::KeyPressed:
					{
						if (event.key.code == sf::Keyboard::Delete && m_activeShapeID.has_value())
						{
							DeleteShape(m_activeShapeID.value());
							m_activeShapeID = std::nullopt;
						}
					}
					default:
						break;
					}
				}
			}
			DrawPicture();
		}
	}

private:
	const std::string DEFAULT_ELLIPSE_ARGS = "600 600 75";
	const std::string DEFAULT_RECTANGLE_ARGS = "600 600 100 100";
	const std::string DEFAULT_TRIANGLE_ARGS = "400 600 500 400 600 600";
	const std::string BASE_COLOR = "#ff00ff";

	std::optional<std::string> m_activeShapeID = std::nullopt;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::shared_ptr<Picture> m_picture;
	std::shared_ptr<ICanvas> m_canvas;
	std::istream& m_input;
	std::ostream& m_output;

	std::optional<std::string> GetShapeIDByCoords(double x, double y)
	{
		for (auto& it : m_picture->GetShapes())
		{
			RectD frame = it.second->GetFrame();
			if (IsInFrame(x, y, frame))
			{
				return it.second->GetId();
			}
		}
		return std::nullopt;
	}

	bool IsInFrame(double x, double y, RectD frame)
	{
		return x >= frame.left && x <= frame.left + frame.width
			&& y >= frame.top && y <= frame.top + frame.height;
	}

	void DrawWidgetPanel()
	{
		m_canvas->DrawWidgetPanel();
	}

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

	void DeleteShape(const std::string& id)
	{
		m_picture->DeleteShape(id);
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

	bool MoveShape(const std::string& id, double dx, double dy)
	{
		try
		{
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
			std::map<unsigned, std::string> outputMessage;
			for (auto& it : m_picture->GetShapes())
			{
				std::string strategyParams = it.second->GetStrategyParams();
				std::string	shapeType = strategyParams.substr(0, strategyParams.find(' '));
				std::string id = it.second->GetId();

				std::stringstream ss;
				ss << m_picture->GetSequenceNumber(id) << " " << shapeType << " " 
					<< id << " " << it.second->GetColor().GetRGBFormat()
					<< strategyParams.substr(strategyParams.find(' ')) << std::endl;
				outputMessage.insert({ m_picture->GetSequenceNumber(id), ss.str()});
			}
			for (auto& it : outputMessage)
			{
				m_output << it.second;
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

	void DrawPicture()
	{
		DrawWidgetPanel();
		for (auto& it : m_picture->GetShapes())
		{
			it.second->Draw(*m_canvas);
			if (it.second->GetId() == m_activeShapeID)
			{
				RectD frame = it.second->GetFrame();
				m_canvas->DrawFrame(frame);
			}
		}
		m_canvas->Display();
	}

	bool AddShape(std::string id, std::string hexColor, std::string type, std::string args)
	{
		try
		{	
			std::stringstream ss;
			ss << args;
			m_picture->AddShape(std::make_unique<Shape>(MakeDrawingStrategy(type, ss), id, hexColor));
		}
		catch (std::exception e)
		{
			m_output << e.what() << std::endl;
			return false;
		}

		return true;
	};

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