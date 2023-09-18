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
    void MoveTo(double x, double y) override
    {
        m_position = sf::Vector2f(x, y);
    }

    void SetColor(Color color) override
    {
        m_color = (sf::Color)HexToUint32(color);
    }
     
    void LineTo(double x, double y) override
    {
        sf::Vector2f nextPosition = sf::Vector2f(x, y);
        sf::Vertex line[] =
        {
            sf::Vertex(m_position),
            sf::Vertex(nextPosition)
        };
        line->color = m_color;
        m_window.draw(line, 2, sf::Lines);
        m_position = nextPosition;
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
        EllipseShape ellipse(sf::Vector2f(rx, ry));
        ellipse.setPosition(m_position);
        ellipse.move(sf::Vector2f(cx, cy));
        ellipse.setFillColor(m_color);
        m_window.draw(ellipse);
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
        drawingText.setPosition(sf::Vector2f(left, top));
        drawingText.setString(text);
        drawingText.setCharacterSize(fontSize);
        drawingText.setFillColor(m_color);
        m_window.draw(drawingText);
    }

    void Display() override
    {
        while (m_window.isOpen())
        {
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window.close();
            }
            m_window.setFramerateLimit(FRAME_RATE);
            m_window.display();
        }
    }

private:
    sf::Color m_color;
    sf::Vector2f m_position;

    const unsigned WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1080, FRAME_RATE = 145;
    sf::RenderWindow m_window = sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "");

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