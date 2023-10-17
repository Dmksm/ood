#pragma once
#include "ICanvas.h"
#include "EllipseShape.h"
#include "Color.h"
#include <stdint.h>
#include <string>

using namespace std::string_literals;

class CCanvas : public ICanvas
{
public:
    CCanvas(sf::RenderWindow* window)
        : m_window(window)
    {
    }

    void MoveTo(double x, double y) override
    {
        m_position = sf::Vector2f(x, y);
    }

    void SetColor(Color color) override
    {
        m_color = GetColor(color);
    }
     
    void LineTo(double x, double y) override
    {
        DrawLine(m_position.x, m_position.y, x, y);
        m_position = sf::Vector2f(x, y);
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
        EllipseShape ellipse(sf::Vector2f(rx, ry));
        ellipse.setPosition(m_position);
        ellipse.move(sf::Vector2f(cx, cy));
        ellipse.setFillColor(m_color);
        m_window->draw(ellipse);
    }

    void DrawRegularPolygon(double radius, double vertexCount) override
    {
        sf::CircleShape regularPolygon(radius, vertexCount);
        regularPolygon.setPosition(m_position);
        regularPolygon.setFillColor(m_color);
        m_window->draw(regularPolygon);
    }

    void DrawRectangle(double left, double top, double width, double height) override
    {
        sf::ConvexShape rectangle(4);
        rectangle.setPoint(0, sf::Vector2f(left, top));
        rectangle.setPoint(1, sf::Vector2f(left + width, top));
        rectangle.setPoint(2, sf::Vector2f(left + width, top + height));
        rectangle.setPoint(3, sf::Vector2f(left, top + height));

        rectangle.setPosition(m_position);
        rectangle.setFillColor(m_color);
        m_window->draw(rectangle);
    }

    void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) override
    {
        sf::ConvexShape triangle(3);
        triangle.setPoint(0, sf::Vector2f(x1, y1));
        triangle.setPoint(1, sf::Vector2f(x2, y2));
        triangle.setPoint(2, sf::Vector2f(x3, y3));

        triangle.setPosition(m_position);
        triangle.setFillColor(m_color);
        m_window->draw(triangle);
    }

    void DrawText(double left, double top, double fontSize, const std::string& text) override
    {
        sf::Text drawingText;
        const std::string FONT_FILE_NAME = "ArialMT.ttf";
        sf::Font font;
        if (!font.loadFromFile(FONT_FILE_NAME))
        {
            throw std::logic_error("Can not open file "s + FONT_FILE_NAME);
        }
        drawingText.setFont(font);
        drawingText.setPosition(m_position);
        drawingText.move(sf::Vector2f(left, top));
        drawingText.setString(text);
        drawingText.setCharacterSize(fontSize);
        drawingText.setFillColor(m_color);
        m_window->draw(drawingText);
    }

    void DrawLine(double x1, double y1, double x2, double y2) override
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(x1, y1)),
            sf::Vertex(sf::Vector2f(x2, y2))
        };
        line->color = m_color;
        m_window->draw(line, 2, sf::Lines);
    }

    void Display() override
    {
        m_window->display();
    }

private:
    sf::Color m_color = sf::Color();
    sf::Vector2f m_position = sf::Vector2(0.0f, 0.0f);
    std::shared_ptr<sf::RenderWindow> m_window;

    sf::Color GetColor(Color color)
    {
        switch (color)
        {
            case Color::Green:
            {
                return { 0, 255, 0, 255 };
            }
            case Color::Red:
            {
                return { 255, 0, 0, 255 };
            }
            case Color::Blue:
            {
                return { 0, 0, 255, 255 };
            }
            case Color::Yellow:
            {
                return { 255, 255, 0, 255 };
            }
            case Color::Pink:
            {
                return { 255, 105, 180, 255 };
            }
            case Color::Black:
            {
                return { 0, 0, 0, 255 };
            }
        }
    }
};