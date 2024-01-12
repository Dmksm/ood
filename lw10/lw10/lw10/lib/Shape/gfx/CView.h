#pragma once
#include "../stdafx.h"
#include "IView.h"
#include "EllipseShape.h"
#include "Color.h"
#include "CommonTypes.h"
#include "ICanvas.h"

using namespace std::string_literals;

class CView : public IView
{
public:
    CView(sf::RenderWindow* window)
        : m_window(window)
        , m_position(sf::Vector2((float)0, (float)0))
        , m_canvas(std::move(std::make_shared<CCanvas>(window)))
    {
    }

    void MoveTo(double x, double y) override
    {
        m_canvas->MoveTo(x, y);
    }

    void SetColor(Color color) override
    {
        m_canvas->SetColor(color);
    }

    void LineTo(double x, double y) override
    {
        m_canvas->LineTo(x, y);
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
        m_canvas->DrawEllipse(cx, cy, rx, ry);
    }

    void DrawFrame(RectD frame) override
    {
        m_canvas->SetColor(m_frameColor);
        m_canvas->DrawLine(frame.left, frame.top, frame.left + frame.width, frame.top);
        m_canvas->DrawLine(frame.left + frame.width, frame.top, frame.left + frame.width, frame.top + frame.height);
        m_canvas->DrawLine(frame.left + frame.width, frame.top + frame.height, frame.left, frame.top + frame.height);
        m_canvas->DrawLine(frame.left, frame.top + frame.height, frame.left, frame.top);
        m_canvas->SetColor(m_selectionMarkerColor);
        m_canvas->DrawEllipse(frame.left, frame.top, m_selectionMarkerRadius, m_selectionMarkerRadius);
        m_canvas->DrawEllipse(frame.left + frame.width, frame.top, m_selectionMarkerRadius, m_selectionMarkerRadius);
        m_canvas->DrawEllipse(frame.left + frame.width, frame.top + frame.height, m_selectionMarkerRadius, m_selectionMarkerRadius);
        m_canvas->DrawEllipse(frame.left, frame.top + frame.height, m_selectionMarkerRadius, m_selectionMarkerRadius);
    }

    void DrawRectangle(double left, double top, double width, double height) override
    {
        m_canvas->DrawRectangle(left, top, width, height);
    }

    void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) override
    {
        m_canvas->DrawTriangle(x1, y1, x2, y2, x3, y3);
    }

    void DrawText(double left, double top, double fontSize, const std::string& text) override
    {
        m_canvas->DrawText(left, top, fontSize, text);
    }

    void DrawLine(double x1, double y1, double x2, double y2) override
    {
        m_canvas->DrawLine(x1, y1, x2, y2);
    }

    void DrawWidgetPanel() override
    {
        m_window->clear(sf::Color(m_backgroundColor.GetRGBFormat()));

        sf::Vector2u size = m_window->getSize();
        unsigned int width = size.x;
        unsigned int height = size.y;

        m_canvas->SetColor(m_widgetPanelColor);
        float m_widgetPanelWidth = width - 2 * m_widgetPanelLeft;
        DrawRectangle(m_widgetPanelLeft, m_widgetPanelTop, m_widgetPanelWidth, m_widgetPanelHeight);

        RectD widgetFrame = GetWidgetFrame(ShapeType::Rectangle);
        m_canvas->SetColor(m_widgetBackgroundColor);
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Rectangle);

        widgetFrame = GetWidgetFrame(ShapeType::Triangle);
        m_canvas->SetColor(m_widgetBackgroundColor);
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Triangle);

        widgetFrame = GetWidgetFrame(ShapeType::Ellipse);
        m_canvas->SetColor(m_widgetBackgroundColor);
        DrawRectangle(widgetFrame.left, widgetFrame.top, widgetFrame.width, widgetFrame.height);
        DrawWidget(ShapeType::Ellipse);

        m_canvas->SetColor(m_workSpaceColor);
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
    Color m_frameColor = Color("#0000ff");
    Color m_selectionMarkerColor = Color("#0000ff");
    float m_selectionMarkerRadius = 3;

    float m_widgetMarginTop = 2;
    float m_widgetMarginLeft = 40;
    float m_widgetSize = 50;

    float m_widgetPanelLeft = 30;
    float m_widgetPanelTop = 58;
    float m_widgetButtom = m_widgetPanelTop + m_widgetMarginTop + m_widgetSize;
    float m_widgetPanelHeight = 2 * m_widgetSize;

    float m_textSize = 16;
    float m_textMarginLeftFromShapePos = -5;
    float m_textMarginTop = 5;

    float m_workSpaceMarginLeft = m_widgetPanelLeft;
    float m_workSpaceMarginTop = 15;
    float m_workSpaceMarginBottom = 200;
    float m_workSpaceMarginRight = 15;

    Color m_widgetPanelColor = Color("#a5a5a5");
    Color m_backgroundColor = Color("#808080");
    Color m_widgetShapeColor = Color("#1164b4");
    Color m_widgetTextColor = Color("#000000");
    Color m_workSpaceColor = Color("#ffffff");
    Color m_widgetBackgroundColor = Color("#b5b8b1");

    std::shared_ptr<ICanvas> m_canvas;
    RectD m_workSpaceBorder = { 0, 0, 0, 0 };
    float m_outlineThickness = 1;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;

    void DrawWidget(ShapeType shapeType)
    {
        float left = (int)shapeType * (m_widgetSize + m_widgetMarginLeft);
        std::string text;

        m_canvas->SetColor(m_widgetShapeColor);
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

        m_canvas->SetColor(m_widgetTextColor);
        DrawText(left + m_textMarginLeftFromShapePos, m_widgetButtom + m_textMarginTop, m_textSize, text);
    }
};