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
        m_position = sf::Vector2f(x, y);
    }

    void SetLineColor(RGBAColor color) override
    {
        m_lineColor = (sf::Color)color;
    }
     
    void LineTo(double x, double y) override
    {
        m_lines.push_back((sf::Vector2f(x, y)));
        m_position = sf::Vector2f(x, y);
    }

    void EndFill() override
    {
        if (!m_lines.empty())
        {
            int vertexCount = m_lines.size();
            sf::Vertex* vertices = &m_lines[0];
            vertices->color = m_fillColor;
            m_window->draw(vertices, vertexCount, sf::Lines);

            m_lines.clear();
        }
        m_window->display();
    }

    void DrawEllipse(double left, double top, double width, double height) override
    {
        EllipseShape ellipse(sf::Vector2f(left, top));
        ellipse.setPosition(m_position);
        ellipse.move(sf::Vector2f(left + width, top + height));
        ellipse.setOutlineThickness(OUTLINE_THICKNESS);
        ellipse.setOutlineColor(m_lineColor);
        ellipse.setFillColor(m_fillColor);
        m_window->setActive(true);
        m_window->draw(ellipse);
    }

    void EndDraw() override
    {
        m_window->display();
    }

private:
    const float OUTLINE_THICKNESS = 1.0f;

    sf::Color m_lineColor;
    sf::Color m_fillColor;
    sf::Vector2f m_position;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::vector<sf::Vertex> m_lines;
};