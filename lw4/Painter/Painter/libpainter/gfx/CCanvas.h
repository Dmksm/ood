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
        , m_color(sf::Color())
        , m_position(sf::Vector2(0, 0))
    {
    }

    void MoveTo(double x, double y) override
    {
        m_position = sf::Vector2f(x, y);
    }

    void SetColor(Color color) override
    {
        m_color = (sf::Color)(color.GetRGBFormat());
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
        m_window->setActive(true);
        m_window->draw(ellipse);
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
    sf::Color m_color;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;

    uint32_t HexToUint32(const std::string& hexColor) const
    {
        if (!IsValidHexCode(hexColor))
        {
            throw std::logic_error("Undefined hex color code = "s + hexColor + ". "s);
        }
        std::string hexCode = hexColor;
        if (hexCode.at(0) == '#')
        {
            hexCode.erase(0, 1);
        }
        while (hexCode.length() != 6)
        {
            hexCode += "0";
        }
        while (hexCode.length() != 8)
        {
            hexCode += "F";
        }

        return std::stoul(hexCode, nullptr, 16);
    }

    bool IsValidHexCode(const std::string& hexColor) const
    {
        const unsigned AVAILABLE_HEX_SIZE = 7;
        const char FIRST_HEX_SYMBOL = '#';
        if ((hexColor[0] != FIRST_HEX_SYMBOL) || (hexColor.length() != AVAILABLE_HEX_SIZE))
        {
            return false;
        }

        for (int position = 1; position < hexColor.length(); position++)
        {
            if (!((hexColor[position] >= '0' && hexColor[position] <= '9')
                || (hexColor[position] >= 'a' && hexColor[position] <= 'f')
                || (hexColor[position] >= 'A' && hexColor[position] <= 'F')))
            {
                return false;
            }
        }

        return true;
    }
};