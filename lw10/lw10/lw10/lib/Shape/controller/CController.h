#pragma once
#include "../stdafx.h"
#include "../shapes/IShapeBehaviourFactory.h"
#include "../gfx/IView.h"
#include "../gfx/IView.h"
#include "../gfx/CView.h"
#include "../shapes/CShape.h"

class CController
{
public:
	CController(
		std::unique_ptr<IShapeBehaviourFactory>&& shapeFactory,
		std::unique_ptr<IPicture>&& picture,
		sf::RenderWindow* renderWindow,
		std::istream& input, 
		std::ostream& output
	)
		: m_picture(std::move(picture))
		, m_shapeFactory(std::move(shapeFactory))
		, m_window(renderWindow)
		, m_view(std::move(std::make_shared<CView>(renderWindow)))
		, m_input(input)
		, m_output(output)
	{
	}

	void Execute()
	{
		DrawWidgetPanel();
		PointD prevMousePos = { 0, 0 };
		PointD differencePoint = { 0, 0 };
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
						if (event.key.code == sf::Keyboard::Delete && m_activeShapeID.has_value())
						{
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
			DrawPicture();
		}
	}

private:
	const RectD DEFAULT_FRAME = {600, 600, 200, 200};
	const std::string BASE_COLOR = "#ff00ff";

	std::optional<std::string> m_activeShapeID = std::nullopt;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::shared_ptr<IPicture> m_picture;
	std::shared_ptr<IView> m_view;
	std::istream& m_input;
	std::ostream& m_output;
	std::unique_ptr<IShapeBehaviourFactory> m_shapeFactory;

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
			RectD border = m_view->GetWorkSpaceFrameBorder();
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
				selectionFrames = m_view->GetSelectionMarkerFrame(frame);
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
		RectD rectFrame = m_view->GetWidgetFrame(IView::ShapeType::Rectangle);
		RectD triangleFrame = m_view->GetWidgetFrame(IView::ShapeType::Triangle);
		RectD ellipseFrame = m_view->GetWidgetFrame(IView::ShapeType::Ellipse);
		if (IsInFrame(pos, rectFrame))
		{
			std::string type = "rectangle";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_FRAME, std::nullopt, std::nullopt);
		}
		if (IsInFrame(pos, triangleFrame))
		{
			std::string type = "triangle";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_FRAME, std::nullopt, std::nullopt);
		}
		if (IsInFrame(pos, ellipseFrame))
		{
			std::string type = "ellipse";
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			AddShape(boost::uuids::to_string(uuid), BASE_COLOR, type, DEFAULT_FRAME, std::nullopt, std::nullopt);
		}
	}

	std::optional<std::string> GetShapeIDByCoords(PointD mousePos)
	{
		std::optional<std::string> result = std::nullopt;
		for (auto& it : m_picture->GetShapes())
		{
			RectD frame = it.second->GetFrame();
			std::optional<IBehaviourStrategy::ShapeType> type = it.second->GetType();
			if (IsInFrame(mousePos, frame, type))
			{
				if (!result.has_value())
				{
					result = it.second->GetId();
				}
				else
				{
					if (m_picture->GetSequenceNumber(it.second->GetId()) >
						m_picture->GetSequenceNumber(result.value())
						)
					{
						result = it.second->GetId();
					}
				}
			}
		}
		return result;
	}

	bool IsInFrame(PointD pos, RectD frame, 
		std::optional<IBehaviourStrategy::ShapeType> shapeType = std::nullopt)
	{
		if (shapeType.has_value())
		{
			switch (shapeType.value())
			{
				case IBehaviourStrategy::ShapeType::Ellipse:
				{
					PointD center = { frame.left + frame.width / 2, frame.top + frame.height / 2 };
					PointD axis = { frame.width / 2, frame.height / 2 };
					return IsPointInsideEllipse(pos, center, axis);
				}
				case IBehaviourStrategy::ShapeType::Triangle:
				{
					PointD pointA = { frame.left, frame.top + frame.height };
					PointD pointB = { frame.left + frame.width / 2, frame.top };
					PointD pointC = { frame.left + frame.width, frame.top + frame.height };
					return IsPointInsideTriangle(pos, pointA, pointB, pointC);
				}
				default:
				{
					break;
				}
			}
		}
		return pos.x >= frame.left && pos.x <= frame.left + frame.width
			&& pos.y >= frame.top && pos.y <= frame.top + frame.height;
	}

	double CrossProduct(PointD A, PointD B, PointD C) {
		return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
	}

	bool IsPointInsideTriangle(PointD pos, PointD A, PointD B, PointD C) {
		double d1 = CrossProduct(A, B, pos);
		double d2 = CrossProduct(B, C, pos);
		double d3 = CrossProduct(C, A, pos);

		bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(hasNeg && hasPos);
	}

	bool IsPointInsideEllipse(PointD pos, PointD center, PointD axis) {
		double term1 = std::pow((pos.x - center.x) / axis.x, 2);
		double term2 = std::pow((pos.y - center.y) / axis.y, 2);

		return term1 + term2 <= 1;
	}

	void DrawWidgetPanel()
	{
		m_view->DrawWidgetPanel();
	}

	void ChangeShape(const std::string& id, const std::string& type, RectD frame,
		std::optional<unsigned> framefontSize,
		std::optional<std::string> text
	)
	{
		try
		{
			m_picture->ChangeShape(id, m_shapeFactory->MakeBehaviourStrategy(type, frame, framefontSize, text));
		}
		catch (std::exception& e)
		{
			m_output << e.what() << std::endl;
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
			m_output << e.what() << std::endl;
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
			m_output << e.what() << std::endl;
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
			m_output << e.what() << std::endl;
		}
	}

	void DrawShape(const std::string& id)
	{
		try
		{
			m_view->SetColor(m_picture->GetShape(id)->GetColor());
			RectD frame = m_picture->GetShape(id)->GetFrame();
			std::optional<IBehaviourStrategy::ShapeType> type = m_picture->GetShape(id)->GetType();
			if (type.has_value())
			{
				if (type.value() == IBehaviourStrategy::ShapeType::Ellipse)
				{
					m_view->DrawEllipse(
						frame.left + frame.width / 2,
						frame.top + frame.height / 2,
						frame.width / 2,
						frame.height / 2
					);
				}
				if (type.value() == IBehaviourStrategy::ShapeType::Rectangle)
				{
					m_view->DrawRectangle(
						frame.left, frame.top,
						frame.width, frame.height
					);
				}
				if (type.value() == IBehaviourStrategy::ShapeType::Triangle)
				{
					m_view->DrawTriangle(
						frame.left, frame.top + frame.height,
						frame.left + frame.width / 2, frame.top,
						frame.left + frame.width, frame.top + frame.height
					);
				}
				if (type.value() == IBehaviourStrategy::ShapeType::Line)
				{
					m_view->DrawLine(
						frame.left, frame.top,
						frame.left + frame.width, frame.top + frame.height
					);
				}

			}
			m_view->Display();
		}
		catch (std::exception& e)
		{
			m_output << e.what() << std::endl;
		}
	}

	void DrawPicture()
	{
		try
		{
			DrawWidgetPanel();
			unsigned positionNumber = 0;

			std::map<unsigned, std::string> orderList;
			for (auto& it : m_picture->GetShapes())
			{
				std::string ID = it.second->GetId();
				orderList[m_picture->GetSequenceNumber(ID)] = it.second->GetId();
			}

			for (auto& it : orderList)
			{
				std::string ID = it.second;
				
				m_view->SetColor(m_picture->GetShape(ID)->GetColor());
				RectD frame = m_picture->GetShape(ID)->GetFrame(); 
				std::optional<IBehaviourStrategy::ShapeType> type = m_picture->GetShape(ID)->GetType();
				if (type.has_value())
				{
					if (type.value() == IBehaviourStrategy::ShapeType::Ellipse)
					{
						m_view->DrawEllipse(
							frame.left + frame.width / 2,
							frame.top + frame.height / 2,
							frame.width / 2,
							frame.height / 2
						);
					}
					if (type.value() == IBehaviourStrategy::ShapeType::Rectangle)
					{
						m_view->DrawRectangle(
							frame.left, frame.top,
							frame.width, frame.height
						);
					}
					if (type.value() == IBehaviourStrategy::ShapeType::Triangle)
					{
						m_view->DrawTriangle(
							frame.left, frame.top + frame.height,
							frame.left + frame.width / 2, frame.top,
							frame.left + frame.width, frame.top + frame.height
						);
					}
					if (type.value() == IBehaviourStrategy::ShapeType::Line)
					{
						m_view->DrawLine(
							frame.left, frame.top,
							frame.left + frame.width, frame.top + frame.height
						);
					}

					if (ID == m_activeShapeID)
					{
						RectD frame = m_picture->GetShape(ID)->GetFrame();
						m_view->DrawFrame(frame);
					}
				}
			}

			m_view->Display();
		}
		catch (std::exception& e)
		{
			m_output << e.what() << std::endl;
		}
	}

	void AddShape(
		const std::string& id,
		const std::string& hexColor,
		const std::string& type,
		RectD frame,
		std::optional<unsigned> framefontSize,
		std::optional<std::string> text
	)
	{
		try
		{	
			m_picture->AddShape(std::make_unique<CShape>(m_shapeFactory->MakeBehaviourStrategy(type, frame, framefontSize, text), id, hexColor));
		}
		catch (std::exception& e)
		{
			m_output << e.what() << std::endl;
		}
	};
};