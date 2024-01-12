#pragma once
#include "../stdafx.h"
#include "ICanvas.h"
#include "EllipseShape.h"
#include "Color.h"
#include "CommonTypes.h"

using namespace std::string_literals;

class CCanvas : public ICanvas
{
public:
    CCanvas(sf::RenderWindow* window)
        : m_window(window)
        , m_color(sf::Color())
        , m_position(sf::Vector2((float)0, (float)0))
    {
    }

    void MoveTo(double x, double y) override
    {
        m_position = sf::Vector2f((float)x, (float)y);
    }

    void SetColor(Color color) override
    {
        m_color = (sf::Color)(color.GetRGBFormat());
    }
    
    void LineTo(double x, double y) override
    {
        DrawLine(m_position.x, m_position.y, x, y);
        m_position = sf::Vector2f((float)x, (float)y);
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
        EllipseShape ellipse(sf::Vector2f((float)rx, (float)ry));
        ellipse.setPosition(m_position);
        ellipse.move(sf::Vector2f((float)cx - (float)rx, (float)cy - (float)ry));
        ellipse.setFillColor(m_color);
        ellipse.setOutlineColor(m_outlineColor);
        ellipse.setOutlineThickness(m_outlineThickness);
        m_window->setActive(true);
        m_window->draw(ellipse);
    }

    void DrawRectangle(double left, double top, double width, double height) override
    {
        sf::ConvexShape rectangle(4);
        rectangle.setPoint(0, sf::Vector2f((float)left, (float)top));
        rectangle.setPoint(1, sf::Vector2f((float)left + (float)width, (float)top));
        rectangle.setPoint(2, sf::Vector2f((float)left + (float)width, (float)top + (float)height));
        rectangle.setPoint(3, sf::Vector2f((float)left, (float)top + (float)height));

        rectangle.setPosition(m_position);
        rectangle.setFillColor(m_color);
        rectangle.setOutlineColor(m_outlineColor);
        rectangle.setOutlineThickness(m_outlineThickness);
        m_window->draw(rectangle);
    }

    void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) override
    {
        sf::ConvexShape triangle(3);
        triangle.setPoint(0, sf::Vector2f((float)x1, (float)y1));
        triangle.setPoint(1, sf::Vector2f((float)x2, (float)y2));
        triangle.setPoint(2, sf::Vector2f((float)x3, (float)y3));

        triangle.setPosition(m_position);
        triangle.setFillColor(m_color);
        triangle.setOutlineColor(m_outlineColor);
        triangle.setOutlineThickness(m_outlineThickness);
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
        drawingText.move(sf::Vector2f((float)left, (float)top));
        drawingText.setString(text);
        drawingText.setCharacterSize((unsigned)fontSize);
        drawingText.setFillColor(m_color);
        drawingText.setOutlineColor(m_outlineColor);
        drawingText.setOutlineThickness(m_outlineThickness);
        m_window->draw(drawingText);
    }

    void DrawLine(double x1, double y1, double x2, double y2) override
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f((float)x1, (float)y1)),
            sf::Vertex(sf::Vector2f((float)x2, (float)y2))
        };
        line->color = m_color;
        m_window->draw(line, 2, sf::Lines);
    }

private:
    float m_outlineThickness = 1;
    sf::Color m_outlineColor = sf::Color(0, 0, 0);
    sf::Color m_color = sf::Color(0, 0, 0);
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;
};