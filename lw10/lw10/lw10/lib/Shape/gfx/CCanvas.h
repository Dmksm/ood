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
    enum class ShapeType : int
    {
        Ellipse = 0,
        Triangle = 1,
        Rectangle = 2,
    };

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
        drawingText.setCharacterSize(fontSize);
        drawingText.setFillColor(m_color);
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

    void DrawWidgetPanel() override
    {
        m_window->clear(m_backgroundColor);

        sf::Vector2u size = m_window->getSize();
        unsigned int width = size.x;
        unsigned int height = size.y;

        m_color = m_widgetPanelColor;
        float m_widgetPanelWidth = width - 2 * m_widgetPanelLeft;
        DrawRectangle(m_widgetPanelLeft, m_widgetPanelTop, m_widgetPanelWidth, m_widgetPanelHeight);

        unsigned widgetPositionNumber = 0;
        DrawWidget(ShapeType::Rectangle, ++widgetPositionNumber);
        DrawWidget(ShapeType::Triangle, ++widgetPositionNumber);
        DrawWidget(ShapeType::Ellipse, ++widgetPositionNumber);

        m_color = m_workSpaceColor;
        float m_workSpacelWidth = m_widgetPanelWidth;
        DrawRectangle(m_workSpaceMarginLeft, m_widgetPanelTop + m_widgetPanelHeight + m_workSpaceMarginTop,
            m_workSpacelWidth, height - m_workSpaceMarginBottom);
    }

    void Display() override
    {
        m_window->display();
    }

    void GetWidgetFrame(ShapeType type) override
    {
        switch (type)
        {
            case CCanvas::ShapeType::Ellipse:
            {
                break;
            }
            case CCanvas::ShapeType::Triangle:
            {
                break;
            }
            case CCanvas::ShapeType::Rectangle:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }

private:
    const float m_widgetMarginTop = 2;
    const float m_widgetMarginLeft = 40;
    const float m_widgetSize = 50;

    const float m_widgetPanelLeft = 30;
    const float m_widgetPanelTop = 58;
    const float m_widgetButtom = m_widgetPanelTop + m_widgetMarginTop + m_widgetSize;
    const float m_widgetPanelHeight = 2 * m_widgetSize;

    const float m_textSize = 16;
    const float m_textMarginLeftFromShapePos = -5;
    const float m_textMarginTop = 5;

    const float m_workSpaceMarginLeft = m_widgetPanelLeft;
    const float m_workSpaceMarginTop = 15;
    const float m_workSpaceMarginBottom = 200;
    const float m_workSpaceMarginRight = 15;

    const sf::Color m_widgetPanelColor = sf::Color(165, 165, 165);
    const sf::Color m_backgroundColor = sf::Color(128, 128, 128);
    const sf::Color m_widgetShapeColor = sf::Color(17, 100, 180);
    const sf::Color m_widgetTextColor = sf::Color(0, 0, 0);
    const sf::Color m_workSpaceColor = sf::Color(255, 255, 255);

    sf::Color m_color;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;

    void DrawWidget(ShapeType shapeType, unsigned widgetPositionNumber)
    {
        float left = widgetPositionNumber * (m_widgetSize + m_widgetMarginLeft);
        std::string text;

        m_color = m_widgetShapeColor;
        switch (shapeType)
        {
            case ShapeType::Ellipse:
            {
                float radius = m_widgetSize / 2;
                DrawEllipse(left + radius, m_widgetButtom - radius, radius, radius);
                text = "Ellipse";
                break;
            }
            case ShapeType::Triangle:
            {
                DrawTriangle(
                    left, m_widgetButtom,
                    left + m_widgetSize / 2, m_widgetPanelTop + m_widgetMarginTop,
                    left + m_widgetSize, m_widgetButtom
                );
                text = "Triangle";
                break;
            }
            case ShapeType::Rectangle:
            {
                DrawRectangle(left, m_widgetPanelTop + m_widgetMarginTop, m_widgetSize, m_widgetSize);
                text = "Rectangle"; 
                break;
            }
            default:
            {
                throw std::logic_error("Undefined shape type");
            }
        }

        m_color = m_widgetTextColor;
        DrawText(left + m_textMarginLeftFromShapePos, m_widgetButtom + m_textMarginTop, m_textSize, text);
    }

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