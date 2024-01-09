#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../lw10/lib/Shape/controller/CController.h"
#include "../lw10/lib/Shape/shapes/CPicture.h"
#include "../lw10/lib/Shape/shapes/CShape.h"
#include "../lw10/lib/Shape/shapes/CShapeFabric.h"
#include "../lw10/lib/Shape/gfx/CCanvas.h"
#include "../lw10/lib/Shape/stdafx.h"

using namespace std::string_literals;

class CMockCanvas : public ICanvas
{
public:
	CMockCanvas(
		std::stringstream& out,
		RectD workSpaceBorder = { 100, 100, 100, 100 },
		RectD widgetFrame = { 20, 20, 20, 20 }
	)
		: m_workSpaceBorder(workSpaceBorder)
		, m_widgetFrame(widgetFrame)
		, m_out(out)
	{
	}

	void MoveTo(double x, double y) override
	{
	}

	void SetColor(Color color) override
	{
	}

	void LineTo(double x, double y) override
	{
	}

	void DrawEllipse(double cx, double cy, double rx, double ry) override
	{
	}

	void DrawFrame(RectD frame) override
	{
	}

	void DrawRectangle(double left, double top, double width, double height) override
	{
	}

	void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) override
	{
	}

	void DrawText(double left, double top, double fontSize, const std::string& text) override
	{
	}

	void DrawLine(double x1, double y1, double x2, double y2) override
	{
	}

	void DrawWidgetPanel() override
	{
	}

	void Display() override
	{
	}

	RectD GetWidgetFrame(ShapeType shapeType) override
	{
		return m_widgetFrame;
	}

	std::vector<RectD> GetSelectionMarkerFrame(RectD frame) override
	{
		std::vector<RectD> result;
		result.push_back({
			frame.left - m_selectionMarkerRadius,
			frame.top - m_selectionMarkerRadius,
			2 * m_selectionMarkerRadius,
			2 * m_selectionMarkerRadius,
			});
		result.push_back({
		   frame.left + frame.width - m_selectionMarkerRadius,
		   frame.top - m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
			});
		result.push_back({
		   frame.left + frame.width - m_selectionMarkerRadius,
		   frame.top + frame.height - m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
			});
		result.push_back({
		   frame.left - m_selectionMarkerRadius,
		   frame.top + frame.height - m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
		   2 * m_selectionMarkerRadius,
			});
		return result;
	}

	RectD GetWorkSpaceFrameBorder() override
	{
		return m_workSpaceBorder;
	}

private:
	const float m_selectionMarkerRadius = 3;

	RectD m_workSpaceBorder;
	RectD m_widgetFrame;
	std::stringstream& m_out;
};


class CMockController
{
public:
	CMockController(
		std::unique_ptr<IShapeFabric>&& shapeFabric,
		std::unique_ptr<IPicture>&& picture,
		std::istream& input,
		std::stringstream& out
	)
		: m_picture(std::move(picture))
		, m_shapeFabric(std::move(shapeFabric))
		, m_canvas(std::move(std::make_shared<CMockCanvas>(out)))
		, m_input(input)
		, m_out(out)
	{
	}

	void Execute(sf::Event event)
	{
		PointD prevMousePos = { 0, 0 };
		PointD differencePoint = { 0, 0 };
		bool isShapeMoving = false;
		bool isShapeResizing = false;
		PointD staticPoint = { 0, 0 };
		bool isMouseOnShape = false;
		bool isMouseOnSelectionMarker = false;
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
		{
			HandleMousePressedEvent(event, isMouseOnShape, isShapeMoving, prevMousePos,
				isMouseOnSelectionMarker, isShapeResizing, staticPoint);
			break;
		}
		case sf::Event::MouseMoved:
		{
			HandleMouseMovedEvent(event, isShapeMoving, isShapeResizing,
				staticPoint, differencePoint, prevMousePos);
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
			m_out << "norm";
			if (event.key.code == sf::Keyboard::Delete && m_activeShapeID.has_value())
			{
				m_out << "norm2";
				DeleteShape(m_activeShapeID.value());
				m_activeShapeID = std::nullopt;
			}
			break;
		}
		default:
		{
		}
		}
	}

	const std::string DEFAULT_ELLIPSE_ARGS = "600 600 75";
	const std::string DEFAULT_RECTANGLE_ARGS = "600 600 100 100";
	const std::string DEFAULT_TRIANGLE_ARGS = "400 600 500 400 600 600";
	const std::string BASE_COLOR = "#ff00ff";

	std::stringstream& m_out;
	std::optional<std::string> m_activeShapeID = std::nullopt;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::shared_ptr<IPicture> m_picture;
	std::shared_ptr<ICanvas> m_canvas;
	std::istream& m_input;
	std::unique_ptr<IShapeFabric> m_shapeFabric;

	void HandleMouseMovedEvent(
		sf::Event event,
		bool isShapeMoving,
		bool isShapeResizing,
		PointD staticPoint,
		PointD& differencePoint,
		PointD& prevMousePos
	)
	{
		if ((isShapeMoving || isShapeResizing) && m_activeShapeID.has_value())
		{
			RectD border = m_canvas->GetWorkSpaceFrameBorder();
			double currX = event.mouseMove.x;
			double currY = event.mouseMove.y;
			RectD activeFrame = m_picture->GetShape(m_activeShapeID.value())->GetFrame();
			differencePoint.x = currX - prevMousePos.x;
			differencePoint.y = currY - prevMousePos.y;
			ChangePointIfOutOfBorder(differencePoint, activeFrame, border);

			prevMousePos.x = currX;
			prevMousePos.y = currY;
			if (isShapeResizing)
			{
				double left = std::min(staticPoint.x, (double)event.mouseMove.x);
				double top = std::min(staticPoint.y, (double)event.mouseMove.y);
				double width = std::abs(left - std::max(staticPoint.x, (double)event.mouseMove.x));
				double height = std::abs(top - std::max(staticPoint.y, (double)event.mouseMove.y));
				m_picture->GetShape(m_activeShapeID.value())->SetFrame({ left, top, width, height });

				RectD frame = m_picture->GetShape(m_activeShapeID.value())->GetFrame();
				ChangeFrameIfOutOfBorder(frame, border);
				m_picture->GetShape(m_activeShapeID.value())->SetFrame(frame);
			}
			else
			{
				if (isShapeMoving)
				{
					MoveShape(m_activeShapeID.value(), differencePoint.x, differencePoint.y);
				}
			}
		}
	}

	void HandleMousePressedEvent(
		sf::Event event,
		bool& isMouseOnShape,
		bool& isShapeMoving,
		PointD& prevMousePos,
		bool& isMouseOnSelectionMarker,
		bool& isShapeResizing,
		PointD& staticPoint
	)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			PointD mousePos = { (double)event.mouseButton.x, (double)event.mouseButton.y };

			auto shapeID = GetShapeIDByCoords(mousePos);
			if (shapeID.has_value())
			{
				isMouseOnShape = true;
				m_activeShapeID = shapeID.value();
				isShapeMoving = true;
				prevMousePos = mousePos;
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
				if (IsInFrame(mousePos, frame))
				{
					isMouseOnSelectionMarker = true;
					isShapeResizing = true;
					index = (index + 2) % 4;
					RectD staticFrame = *(selectionFrames.begin() + index);
					staticPoint = {
						staticFrame.left + staticFrame.width / 2,
						staticFrame.top + staticFrame.height / 2
					};
					prevMousePos = mousePos;
					break;
				}
				++index;
			}

			IsInWidget(mousePos);

			if (!(isMouseOnShape || isMouseOnSelectionMarker))
			{
				m_activeShapeID = std::nullopt;
				isShapeMoving = false;
			}
		}
	}

	void ChangeFrameIfOutOfBorder(RectD& frame, RectD border)
	{
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
	}

	void ChangePointIfOutOfBorder(PointD& differencePoint, RectD activeFrame, RectD border)
	{
		if (activeFrame.left + differencePoint.x < border.left)
		{
			differencePoint.x = border.left - activeFrame.left;
		}
		if (activeFrame.left + activeFrame.width + differencePoint.x > border.left + border.width)
		{
			differencePoint.x = (border.left + border.width) - (activeFrame.left + activeFrame.width);
		}
		if (activeFrame.top + differencePoint.y < border.top)
		{
			differencePoint.y = border.top - activeFrame.top;
		}
		if (activeFrame.top + activeFrame.height + differencePoint.y > border.top + border.height)
		{
			differencePoint.y = (border.top + border.height) - (activeFrame.top + activeFrame.height);
		}
	}

	void IsInWidget(PointD pos)
	{
		RectD rectFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Rectangle);
		RectD triangleFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Triangle);
		RectD ellipseFrame = m_canvas->GetWidgetFrame(ICanvas::ShapeType::Ellipse);
		if (IsInFrame(pos, rectFrame))
		{
			std::string type = "rectangle";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_RECTANGLE_ARGS);
		}
		if (IsInFrame(pos, triangleFrame))
		{
			std::string type = "triangle";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_TRIANGLE_ARGS);
		}
		if (IsInFrame(pos, ellipseFrame))
		{
			std::string type = "circle";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_ELLIPSE_ARGS);
		}
	}

	std::optional<std::string> GetShapeIDByCoords(PointD mousePos)
	{
		for (auto& it : m_picture->GetShapes())
		{
			RectD frame = it.second->GetFrame();
			if (IsInFrame(mousePos, frame))
			{
				return it.second->GetId();
			}
		}
		return std::nullopt;
	}

	bool IsInFrame(PointD pos, RectD frame)
	{
		return pos.x >= frame.left && pos.x <= frame.left + frame.width
			&& pos.y >= frame.top && pos.y <= frame.top + frame.height;
	}

	void DrawWidgetPanel()
	{
		m_canvas->DrawWidgetPanel();
	}

	void ChangeShape(const std::string& id, const std::string& type, std::istream& args)
	{
		try
		{
			m_picture->ChangeShape(id, m_shapeFabric->MakeDrawingStrategy(type, args));
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void DeleteShape(const std::string& id)
	{
		m_picture->DeleteShape(id);
	}

	void ChangeColor(
		const std::string& id,
		const std::string& color
	)
	{
		try
		{
			m_picture->GetShape(id)->SetColor(color);
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void MovePicture(double dx, double dy)
	{
		try
		{
			for (auto& it : m_picture->GetShapes())
			{
				it.second->Move(dx, dy);
			}
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void MoveShape(const std::string& id, double dx, double dy)
	{
		try
		{
			m_picture->GetShape(id)->Move(dx, dy);
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void DisplayShapeList(std::istream& args)
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
				outputMessage.insert({ m_picture->GetSequenceNumber(id), ss.str() });
			}
			for (auto& it : outputMessage)
			{
				m_out << it.second;
			}
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void DrawShape(const std::string& id)
	{
		try
		{
			m_picture->GetShape(id)->Draw(*m_canvas);
			m_canvas->Display();
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void DrawPicture()
	{
		try
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
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	}

	void AddShape(
		const std::string& id,
		const std::string& hexColor,
		const std::string& type,
		const std::string& args
	)
	{
		try
		{
			std::stringstream ss;
			ss << args;
			m_picture->AddShape(std::make_unique<CShape>(m_shapeFabric->MakeDrawingStrategy(type, ss), id, hexColor));
		}
		catch (std::exception& e)
		{
			m_out << e.what() << std::endl;
		}
	};
};

SCENARIO("test controller")
{
	GIVEN("picture and controller out and mock data")
	{
		std::stringstream out;
		CPicture picture;
		CShapeFabric shapeFabric;
		CMockController controller(std::make_unique<CShapeFabric>(std::move(shapeFabric)),
			std::make_unique<CPicture>(std::move(picture)),
			std::cin, out);
		const std::string BASE_COLOR = "#ff00ff";
		const std::string DEFAULT_ID = "1";
		const std::string DEFAULT_ID_2 = "2";
		const std::string DEFAULT_ID_3 = "3";
		WHEN("Add shape")
		{
			controller.AddShape(DEFAULT_ID, BASE_COLOR, "rectangle", "10 20 30 40");
			THEN("Correct added shape")
			{
				controller.AddShape(DEFAULT_ID, BASE_COLOR, "ellipse2", "10 20 30 40");
				REQUIRE(out.str() == "Undefined type ellipse2\n");
				REQUIRE(controller.m_picture->GetShape(DEFAULT_ID)->GetStrategyParams() == "rectangle 10 20 30 40");
				REQUIRE(controller.m_picture->GetShapes().size() == 1);
			}
		}
		WHEN("size shape frame is out of border")
		{
			RectD frame = { 0, 0, 10, 11 };
			RectD border = { 0, 0, 19, 5};
			controller.ChangeFrameIfOutOfBorder(frame, border);
			THEN("correct frame")
			{
				RectD expectedFrame = { 0, 0, 10, 5 };
				REQUIRE(frame.height == expectedFrame.height);
				REQUIRE(frame.left == expectedFrame.left);
				REQUIRE(frame.width == expectedFrame.width);
				REQUIRE(frame.height == expectedFrame.height);
			}
		}
		WHEN("move shape on out of border")
		{
			PointD differencePoint = {-1, 0};
			RectD frame = { 0, 0, 10, 5 };
			RectD border = { 0, 0, 20, 10 };
			controller.ChangePointIfOutOfBorder(differencePoint, frame, border);
			THEN("correct frame")
			{
				RectD expectedFrame = { 0, 0, 10, 5 };
				REQUIRE(frame.height == expectedFrame.height);
				REQUIRE(frame.left == expectedFrame.left);
				REQUIRE(frame.width == expectedFrame.width);
				REQUIRE(frame.height == expectedFrame.height);
			}
		}
		WHEN("click on shape by mouse")
		{
			controller.AddShape(DEFAULT_ID, BASE_COLOR, "rectangle", "10 20 30 40");
			PointD mousePos = { 10, 20 };
			auto id = controller.GetShapeIDByCoords(mousePos);
			THEN("get selected shape")
			{
				REQUIRE(id.has_value());
				REQUIRE(id.value() == DEFAULT_ID);
			}
		}
		WHEN("delete shape by Delete key")
		{
			controller.AddShape(DEFAULT_ID, BASE_COLOR, "rectangle", "10 20 30 40");
			sf::Event event;
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Delete;
			controller.m_activeShapeID = DEFAULT_ID;

			REQUIRE(controller.m_picture->GetShape(DEFAULT_ID)->GetStrategyParams() == "rectangle 10 20 30 40");
			REQUIRE(controller.m_picture->GetShapes().size() == 1);

			controller.Execute(event);
			THEN("get selected shape")
			{
				REQUIRE(!controller.m_activeShapeID.has_value());
				REQUIRE(controller.m_picture->GetShapes().size() == 0);
			}
		}
		WHEN("move shapes")
		{
			controller.AddShape(DEFAULT_ID, BASE_COLOR, "rectangle", "10 20 30 40");
			REQUIRE(controller.m_picture->GetShape(DEFAULT_ID)->GetStrategyParams() == "rectangle 10 20 30 40");
			controller.AddShape(DEFAULT_ID_2, BASE_COLOR, "triangle", "10 10 20 5 30 10");
			REQUIRE(controller.m_picture->GetShape(DEFAULT_ID_2)->GetStrategyParams() == "triangle 10 10 20 5 30 10");
			controller.AddShape(DEFAULT_ID_3, BASE_COLOR, "ellipse", "50 60 30 20");
			REQUIRE(controller.m_picture->GetShape(DEFAULT_ID_3)->GetStrategyParams() == "ellipse 50 60 30 20");
			controller.MoveShape(DEFAULT_ID, 10, 5);
			controller.MoveShape(DEFAULT_ID_2, 5, 5);
			controller.MoveShape(DEFAULT_ID_3, 10, 10);
			THEN("get moved shapes")
			{
				REQUIRE(controller.m_picture->GetShape(DEFAULT_ID)->GetStrategyParams() == "rectangle 20 25 30 40");
				REQUIRE(controller.m_picture->GetShape(DEFAULT_ID_2)->GetStrategyParams() == "triangle 15 15 25 10 35 15");
				REQUIRE(controller.m_picture->GetShape(DEFAULT_ID_3)->GetStrategyParams() == "ellipse 60 70 30 20");
			}
		}
	}
}

SCENARIO("test picture")
{
	GIVEN("picture and out and mock data")
	{
		std::stringstream out;
		CShapeFabric shapeFabric;
		CPicture picture;
		const std::string BASE_COLOR = "#ff00ff";
		const std::string TYPE = "ellipse";
		const std::string TYPE_2 = "rectangle";
		const std::string TYPE_3 = "triangle";
		const std::string DEFAULT_ID = "1";
		const std::string DEFAULT_ID_2 = "2";
		const std::string DEFAULT_ID_3 = "3";
		std::stringstream ss;
		ss << "10 12 13 16";
		WHEN("Add shape")
		{
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE, ss), DEFAULT_ID, BASE_COLOR)
			));
			THEN("Added new one")
			{
				REQUIRE(picture.GetShapes().size() == 1);
				REQUIRE(picture.GetShape(DEFAULT_ID)->GetStrategyParams() == "ellipse 10 12 13 16");
			}
		}
		WHEN("Delete shape")
		{
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE, ss), DEFAULT_ID, BASE_COLOR)
			));
			picture.DeleteShape(DEFAULT_ID);
			THEN("Added new one")
			{
				REQUIRE(picture.GetShapes().size() == 0);
			}
		}
		WHEN("Get z-index of shape")
		{
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE, ss), DEFAULT_ID, BASE_COLOR)
			));
			ss.clear();
			ss << "20 40 15 50";
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE_2, ss), DEFAULT_ID_2, BASE_COLOR)
			));
			ss.clear();
			ss << "20 40 15 50 12 21";
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE_3, ss), DEFAULT_ID_3, BASE_COLOR)
			));
			THEN("Added new one")
			{
				REQUIRE(picture.GetSequenceNumber(DEFAULT_ID) == 1);
				REQUIRE(picture.GetSequenceNumber(DEFAULT_ID_2) == 2);
				REQUIRE(picture.GetSequenceNumber(DEFAULT_ID_3) == 3);
			}
		}
	}
}

SCENARIO("test shape")
{
	GIVEN("picture and out and mock data")
	{
		std::stringstream out;
		CShapeFabric shapeFabric;
		CPicture picture;
		const std::string BASE_COLOR = "#ff00ff";
		const std::string TYPE = "ellipse";
		const std::string TYPE_2 = "rectangle";
		const std::string TYPE_3 = "triangle";
		const std::string DEFAULT_ID = "1";
		const std::string DEFAULT_ID_2 = "2";
		const std::string DEFAULT_ID_3 = "3";
		std::stringstream ss;
		ss << "10 12 13 16";
		WHEN("Get shape frame")
		{
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE, ss), DEFAULT_ID, BASE_COLOR)
			));
			ss.clear();
			ss << "20 40 15 50";
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE_2, ss), DEFAULT_ID_2, BASE_COLOR)
			));
			ss.clear();
			ss << "20 40 15 50 12 21";
			REQUIRE_NOTHROW(picture.AddShape(std::make_unique<CShape>(
				shapeFabric.MakeDrawingStrategy(TYPE_3, ss), DEFAULT_ID_3, BASE_COLOR)
			));
			THEN("got correct shape frame")
			{
				REQUIRE(picture.GetShape(DEFAULT_ID)->GetFrame().left == -3);
				REQUIRE(picture.GetShape(DEFAULT_ID)->GetFrame().top == -4);
				REQUIRE(picture.GetShape(DEFAULT_ID)->GetFrame().width == 26);
				REQUIRE(picture.GetShape(DEFAULT_ID)->GetFrame().height == 32);

				REQUIRE(picture.GetShape(DEFAULT_ID_2)->GetFrame().left == 20);
				REQUIRE(picture.GetShape(DEFAULT_ID_2)->GetFrame().top == 40);
				REQUIRE(picture.GetShape(DEFAULT_ID_2)->GetFrame().width == 15);
				REQUIRE(picture.GetShape(DEFAULT_ID_2)->GetFrame().height == 50);

				REQUIRE(picture.GetShape(DEFAULT_ID_3)->GetFrame().left == 12);
				REQUIRE(picture.GetShape(DEFAULT_ID_3)->GetFrame().top == 21);
				REQUIRE(picture.GetShape(DEFAULT_ID_3)->GetFrame().width == 8);
				REQUIRE(picture.GetShape(DEFAULT_ID_3)->GetFrame().height == 29);
			}
		}

	}
}