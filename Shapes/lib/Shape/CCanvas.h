#pragma once
#include "ICanvas.h"
#include "EllipseShape.h"

class CCanvas : public ICanvas
{
public:
	void DrawElipse(
        IVisualObjectInfo::Point position,
        IVisualObjectInfo::Color color, 
        IVisualObjectInfo::Point bounds
    )
	{
        EllipseShape ellipse(sf::Vector2f(bounds.x / 2, bounds.y / 2));
        ellipse.setPosition(sf::Vector2f(position.x, position.y));
        ellipse.setFillColor((sf::Color)color);
        Draw(ellipse);
	}

	void DrawLine(
        IVisualObjectInfo::Point position,
        IVisualObjectInfo::Color color
    )
	{
	};
	
	void DrawRectangle(
        IVisualObjectInfo::Point position,
        IVisualObjectInfo::Color color,
        IVisualObjectInfo::Point bounds
    )
	{
        sf::RectangleShape rectangle(sf::Vector2f(bounds.x, bounds.y));
        rectangle.setPosition(sf::Vector2f(position.x, position.y));
        rectangle.setFillColor((sf::Color)color);
        Draw(rectangle);
	};

    void DrawTriangle(
        IVisualObjectInfo::Point position,
        IVisualObjectInfo::Color color,
        IVisualObjectInfo::Point secondVertex,
        IVisualObjectInfo::Point thirdVertex
    )
    {
        sf::ConvexShape triangle;

        triangle.setPointCount(3);

        triangle.setPoint(0, sf::Vector2f(position.x, position.y));
        triangle.setPoint(1, sf::Vector2f(secondVertex.x, secondVertex.y));
        triangle.setPoint(2, sf::Vector2f(thirdVertex.x, thirdVertex.y));

        triangle.setFillColor((sf::Color)color);
        Draw(triangle);
    }

private:
	void Draw(const sf::Shape & shape)
	{
        unsigned modeWidth = 1920, modeHeight = 1080;
        sf::RenderWindow window(sf::VideoMode(modeWidth, modeHeight), "Project Shapes");
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
            window.display();
        }
	};
};