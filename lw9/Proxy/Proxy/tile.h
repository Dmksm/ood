#pragma once
#include "stdafx.h"

#include "geom.h"

class Tile
{
public:
	// Размер тайла 8*8 пикселей.
	constexpr static int SIZE = 8;

	// Конструктор по умолчанию. Заполняет тайл указанным цветом.
	Tile(char color = ' ') noexcept
	{
		for (unsigned y = 0; y < SIZE; y++)
		{
			std::vector<char> row;
			for (unsigned x = 0; x < SIZE; x++)
			{
				row.push_back(color);
			}
			m_pixels.push_back(row);
		}
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	Tile(const Tile& other)
	{
		/* Реализуйте недостающий код самостоятельно. */
		for (unsigned y = 0; y < SIZE; y++)
		{
			std::vector<char> row;
			for (unsigned x = 0; x < SIZE; x++)
			{
				char color = other.GetPixel({ (int)x, (int)y });
				row.push_back(color);
			}
			m_pixels.push_back(row);
		}
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	~Tile()
	{
		// -------------- не удалять ------------
		--m_instanceCount; // Уменьшаем счётчик тайлов.
		assert(m_instanceCount >= 0);
		// -------------- не удалять ------------
	}

	/**
	 * Изменяет цвет пикселя тайла.
	 * Если координаты выходят за пределы тайла, метод ничего не делает.
	 */
	void SetPixel(Point p, char color) noexcept
	{
		if (!IsPointInSize(p, { SIZE, SIZE }))
		{
			return;
		}
		try
		{
			m_pixels.at(p.y).at(p.x) = color;
		}
		catch (std::out_of_range& e)
		{
			return;
		}
	}

	/**
	 * Возвращает цвет пикселя. Если координаты выходят за пределы тайла, возвращается пробел.
	 */
	char GetPixel(Point p) const noexcept
	{
		if (!IsPointInSize(p, { SIZE, SIZE }))
		{
			return ' ';
		}
		try
		{
			return m_pixels.at(p.y).at(p.x);
		}
		catch (std::out_of_range& e)
		{
			return ' ';
		}
	}

	// Возвращает количество экземпляра класса Tile в программе.
	static int GetInstanceCount() noexcept
	{
		// -------------- не удалять ------------
		return m_instanceCount;
		// -------------- не удалять ------------
	}

private:
	// -------------- не удалять ------------
	inline static int m_instanceCount = 0;
	// -------------- не удалять ------------
	std::vector<std::vector<char>> m_pixels;
	/* Разместите здесь поля для хранения пикселей тайла. */
};