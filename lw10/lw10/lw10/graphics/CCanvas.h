#pragma once
#include "Canvas.h"
#include "EllipseShape.h"
#include "Style.h"
#include "../stdafx.h"

using namespace std::string_literals;

class CCanvas : public ICanvas
{
public:
    CCanvas(sf::RenderWindow* window)
        : m_window(window)
    {
    }

    void BeginFill(RGBAColor color) override
    {
        m_fillColor = (sf::Color)color;
    }

    void MoveTo(double x, double y) override
    {
        m_position = sf::Vector2f((float)x, (float)y);
    }

    void SetLineColor(RGBAColor color) override
    {
        m_lineColor = (sf::Color)color;
    }
     
    void LineTo(double x, double y) override
    {
        m_lines.push_back(sf::Vector2f((float)x, (float)y));
        m_position = sf::Vector2f((float)x, (float)y);
    }

    void EndFill() override
    {
        if (!m_lines.empty())
        {
            sf::ConvexShape convex;

            convex.setPointCount(m_lines.size());
            convex.setFillColor(m_fillColor);

            convex.setOutlineColor(m_lineColor);
            convex.setOutlineThickness(m_thickness);
            
            for (int i = 0; i < m_lines.size(); i++)
            {
                convex.setPoint(i, m_lines[i].position);
            }
            m_window->draw(convex);
            m_lines.clear();
        }
        m_window->display();
    }

    void DrawEllipse(double cx, double cy, double rx, double ry) override
    {
        EllipseShape ellipse;
        ellipse.setPosition(sf::Vector2f((float)cx, (float)cy));
        ellipse.setRadius(sf::Vector2f((float)rx, (float)ry));
        ellipse.setOutlineThickness(m_thickness);
        ellipse.setOutlineColor(m_lineColor);
        ellipse.setFillColor(m_fillColor);
        m_window->draw(ellipse);
    }

    void SetThickness(double thickness) override
    {
        m_thickness = (float)thickness;
    }

private:
    const float DEFAULT_OUTLINE_THICKNESS = 1.0f;

    float m_thickness = DEFAULT_OUTLINE_THICKNESS;
    sf::Color m_lineColor;
    sf::Color m_fillColor;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::vector<sf::Vertex> m_lines;
};