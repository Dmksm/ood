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

    void DrawFrame(RectD frame) override
    {
        m_color = m_frameColor;
        DrawLine(frame.left, frame.top, frame.left + frame.width, frame.top);
        DrawLine(frame.left + frame.width, frame.top, frame.left + frame.width, frame.top + frame.height);
        DrawLine(frame.left + frame.width, frame.top + frame.height, frame.left, frame.top + frame.height);
        DrawLine(frame.left, frame.top + frame.height, frame.left, frame.top);
        m_color = m_selectionMarkerColor;
        DrawEllipse(frame.left, frame.top, m_selectionMarkerRadius, m_selectionMarkerRadius);
        DrawEllipse(frame.left + frame.width, frame.top, m_selectionMarkerRadius, m_selectionMarkerRadius);
        DrawEllipse(frame.left + frame.width, frame.top + frame.height, m_selectionMarkerRadius, m_selectionMarkerRadius);
        DrawEllipse(frame.left, frame.top + frame.height, m_selectionMarkerRadius, m_selectionMarkerRadius);
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

    void DrawWidgetPanel() override
    {
        m_window->clear(m_backgroundColor);

        sf::Vector2u size = m_window->getSize();
        unsigned int width = size.x;
        unsigned int height = size.y;

        m_color = m_widgetPanelColor;
        float m_widgetPanelWidth = width - 2 * m_widgetPanelLeft;
        DrawRectangle(m_widgetPanelLeft, m_widgetPanelTop, m_widgetPanelWidth, m_widgetPanelHeight);

        RectD widgetFrame = GetWidgetFrame(ICanvas::ShapeType::Rectangle);
        m_color = m_widgetBackgroundColor;
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Rectangle);

        widgetFrame = GetWidgetFrame(ICanvas::ShapeType::Triangle);
        m_color = m_widgetBackgroundColor;
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Triangle);
        
        widgetFrame = GetWidgetFrame(ICanvas::ShapeType::Ellipse);
        m_color = m_widgetBackgroundColor;
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Ellipse);

        m_color = m_workSpaceColor;
        float m_workSpacelWidth = m_widgetPanelWidth;
        DrawRectangle(m_workSpaceMarginLeft, m_widgetPanelTop + m_widgetPanelHeight + m_workSpaceMarginTop,
            m_workSpacelWidth, height - m_workSpaceMarginBottom);
        m_workSpaceBorder = { m_workSpaceMarginLeft, m_widgetPanelTop + m_widgetPanelHeight + m_workSpaceMarginTop,
            m_workSpacelWidth, height - m_workSpaceMarginBottom };
    }

    void Display() override
    {
        m_window->display();
    }

    RectD GetWidgetFrame(ShapeType shapeType) override
    {
        float left = (int)shapeType * (m_widgetSize + m_widgetMarginLeft);
        float top = m_widgetPanelTop + m_widgetMarginTop;
        float width = m_widgetSize;
        float height = m_widgetSize;

        return RectD(left, top, width, height);
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
    const sf::Color m_frameColor = sf::Color(0, 0, 255);
    const sf::Color m_selectionMarkerColor = sf::Color(0, 0, 255);
    const float m_selectionMarkerRadius = 3;

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
    const sf::Color m_widgetBackgroundColor = sf::Color(181, 184, 177);

    RectD m_workSpaceBorder = { 0, 0, 0, 0 };
    float m_outlineThickness = 1;
    sf::Color m_outlineColor = sf::Color(0, 0, 0);
    sf::Color m_color;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;

    void DrawWidget(ShapeType shapeType)
    {
        float left = (int)shapeType * (m_widgetSize + m_widgetMarginLeft);
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