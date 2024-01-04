#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../Proxy/stdafx.h"
#include "../Proxy/drawer.h"

class MockImage
{
public:

	int DivUp(int x, int y)
	{
		return (x - 1) / y + 1;
	}

	MockImage(Size size, char color)
	{
		if (size.height < 0 || size.width < 0)
		{
			throw std::out_of_range("size must not be negative!");
		}

		int tileWidth = DivUp(size.width, Tile::SIZE);
		int tileHeight = DivUp(size.height, Tile::SIZE);
		std::shared_ptr<Tile> tilePtr = std::make_shared<Tile>(color);
		for (unsigned y = 0; y < tileHeight; y++)
		{
			std::vector<std::shared_ptr<Tile>> row;
			for (unsigned x = 0; x < tileWidth; x++)
			{
				row.push_back(tilePtr);
			}
			m_tiles.push_back(row);
		}
		m_size = size;
	}

	Size GetSize() const noexcept
	{
		return m_size;
	}

	char GetPixel(Point p) const noexcept
	{
		if (!IsPointInSize(p, m_size))
		{
			return ' ';
		}
		try
		{
			int tileX = p.x / Tile::SIZE;
			int tileY = p.y / Tile::SIZE;
			p.x %= Tile::SIZE;
			p.y %= Tile::SIZE;
			return m_tiles.at(tileY).at(tileX)->GetPixel(p);
		}
		catch (std::out_of_range& e)
		{
			return ' ';
		}
	}

	void SetPixel(Point p, char color)
	{
		if (!IsPointInSize(p, m_size))
		{
			return;
		}
		try
		{
			int tileX = p.x / Tile::SIZE;
			int tileY = p.y / Tile::SIZE;
			p.x %= Tile::SIZE;
			p.y %= Tile::SIZE;
			if (color != m_tiles.at(tileY).at(tileX)->GetPixel(p))
			{
				Tile tile = *m_tiles.at(tileY).at(tileX);
				std::shared_ptr<Tile> tilePtr = std::make_shared<Tile>(tile);
				tilePtr->SetPixel(p, color);
				m_tiles.at(tileY).at(tileX).swap(tilePtr);
			}
		}
		catch (std::out_of_range& e)
		{
			return;
		}
	}

    Size m_size;
    std::vector<std::vector<std::shared_ptr<Tile>>> m_tiles;
};

namespace
{

	int Sign(int value)
	{
		return (0 < value) - (value < 0);
	}

	void DrawSteepLine(MockImage& image, Point from, Point to, char color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaY >= deltaX);

		if (from.y > to.y)
		{
			std::swap(from, to);
		}

		const int stepX = Sign(to.x - from.x);
		const int errorThreshold = deltaY + 1; 
		const int deltaErr = deltaX + 1; 

		int error = deltaErr / 2;

		for (Point p = from; p.y <= to.y; ++p.y)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.y != to.y) || (p.x == to.x));

			error += deltaErr; 

			if (error >= errorThreshold)
			{
				p.x += stepX;
				error -= errorThreshold;
			}
		}
	}

	void DrawSlopeLine(MockImage& image, Point from, Point to, char color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaX >= deltaY);

		if (from.x > to.x)
		{
			std::swap(from, to);
		}

		const int stepY = Sign(to.y - from.y);
		const int errorThreshold = deltaX + 1;
		const int deltaErr = deltaY + 1;

		int error = deltaErr / 2;

		for (Point p = from; p.x <= to.x; ++p.x)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.x != to.x) || (p.y == to.y));

			error += deltaErr;

			if (error >= errorThreshold)
			{
				p.y += stepY;
				error -= errorThreshold;
			}
		}
	}

} 

void DrawLine(MockImage& image, Point from, Point to, char color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	if (deltaY > deltaX)
	{
		DrawSteepLine(image, from, to, color);
	}
	else
	{
		DrawSlopeLine(image, from, to, color);
	}
}

void DrawCircle(MockImage& image, Point center, int radius, char color)
{
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) 
	{
		image.SetPixel({ center.x + x, center.y + y }, color);
		image.SetPixel({center.x + x, center.y - y }, color);
		image.SetPixel({center.x - x, center.y + y }, color);
		image.SetPixel({center.x - x, center.y - y }, color);
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void FillCircle(MockImage& image, Point center, int radius, char color)
{
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0)
	{
		image.SetPixel({ center.x + x, center.y + y }, color);
		image.SetPixel({ center.x + x, center.y - y }, color);
		image.SetPixel({ center.x - x, center.y + y }, color);
		image.SetPixel({ center.x - x, center.y - y }, color);
		for (int currX = center.x - x + 1; currX < center.x + x; currX++)
		{
			image.SetPixel({ currX, center.y - y }, color);
			image.SetPixel({ currX, center.y + y }, color);
		}
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

SCENARIO("work with image")
{
	REQUIRE_THROWS(MockImage{ { -1, 20 }, '.' });
	REQUIRE_THROWS(MockImage{ { 70, -1 }, '.' });
	GIVEN("image")
	{
		MockImage img{ { 70, 20 }, '.' };
		WHEN("Change tile")
		{
			THEN("Creating new one")
			{
				REQUIRE_NOTHROW(img.SetPixel({ 100, 20 }, 'h'));
				REQUIRE_NOTHROW(img.SetPixel({ 70, 40 }, 'h'));
				REQUIRE_NOTHROW(img.SetPixel({ -1, 20 }, 'h'));
				REQUIRE_NOTHROW(img.SetPixel({ 70, -1 }, 'h'));

				REQUIRE(img.GetPixel({ 100, 20 }) == ' ');
				REQUIRE(img.GetPixel({ 70, 40 }) == ' ');
				REQUIRE(img.GetPixel({ -1, 20 }) == ' ');
				REQUIRE(img.GetPixel({ 70, -1 }) == ' ');

				REQUIRE(img.m_tiles.at(0).size() == 9);
				REQUIRE(img.m_tiles.size() == 3);


				REQUIRE(img.GetPixel({ 0, 0 }) == '.');
				REQUIRE_NOTHROW(img.SetPixel({ 0, 0 }, '4'));
				REQUIRE(img.GetPixel({ 0, 0 }) == '4');
			}
		}
	}
}

SCENARIO("Count tiles")
{
	GIVEN("image")
	{
		MockImage img{ { 70, 20 }, '.' };
		WHEN("Change tile")
		{
			for (int y = 0; y < img.m_tiles.size(); y++)
			{
				for (int x = 0; x < img.m_tiles.at(0).size(); x++)
				{
					REQUIRE(img.m_tiles.at(0).at(0) == img.m_tiles.at(y).at(x));
				}
			}
			REQUIRE(img.m_tiles.at(1) == img.m_tiles.at(2));
			DrawLine(img, { 1, 1 }, { 2, 2 }, '#');
			THEN("Creating new one")
			{
				REQUIRE(img.m_tiles.at(0).at(0) != img.m_tiles.at(0).at(1));
				for (int y = 0; y < img.m_tiles.size(); y++)
				{
					for (int x = 1; x < img.m_tiles.at(0).size(); x++)
					{
						REQUIRE(img.m_tiles.at(0).at(1) == img.m_tiles.at(y).at(x));
					}
				}
			}
		}
		MockImage img2{ { 70, 20 }, '.' };
		WHEN("Change tile")
		{
			for (int y = 0; y < img2.m_tiles.size(); y++)
			{
				for (int x = 0; x < img2.m_tiles.at(0).size(); x++)
				{
					REQUIRE(img2.m_tiles.at(0).at(0) == img2.m_tiles.at(y).at(x));
				}
			}
			REQUIRE(img2.m_tiles.at(1) == img2.m_tiles.at(2));
			DrawLine(img2, { 7, 1 }, { 8, 1 }, '#');
			THEN("Creating new one")
			{
				REQUIRE(img2.m_tiles.at(0).at(0) != img2.m_tiles.at(0).at(1));
				REQUIRE(img2.m_tiles.at(0).at(0) != img2.m_tiles.at(0).at(2));
				REQUIRE(img2.m_tiles.at(0).at(1) != img2.m_tiles.at(0).at(2));
				for (int y = 0; y < img2.m_tiles.size(); y++)
				{
					for (int x = 2; x < img2.m_tiles.at(0).size(); x++)
					{
						REQUIRE(img2.m_tiles.at(0).at(2) == img2.m_tiles.at(y).at(x));
					}
				}
			}
		}
	}
}

SCENARIO("work with tile")
{
	GIVEN("tile")
	{
		MockImage img{ { 70, 20 }, '.' };
		Tile tile = *img.m_tiles.at(0).at(0);
		WHEN("Change tile")
		{
			THEN("Creating new one")
			{
				REQUIRE_NOTHROW(tile.SetPixel({ 8, 0 }, 'h'));
				REQUIRE_NOTHROW(tile.SetPixel({ 0, 8 }, 'h'));
				REQUIRE_NOTHROW(tile.SetPixel({ -1, 0 }, 'h'));
				REQUIRE_NOTHROW(tile.SetPixel({ 0, -1 }, 'h'));

				REQUIRE(tile.GetPixel({ 8, 0 }) == ' ');
				REQUIRE(tile.GetPixel({ 0, 8 }) == ' ');
				REQUIRE(tile.GetPixel({ -1, 8 }) == ' ');
				REQUIRE(tile.GetPixel({ 0, -1 }) == ' ');

				REQUIRE(tile.GetPixel({ 0, 0 }) == '.');
				REQUIRE(tile.GetPixel({ 7, 7 }) == '.');
				REQUIRE_NOTHROW(tile.SetPixel({ 0, 0 }, '7'));
				REQUIRE_NOTHROW(tile.SetPixel({ 7, 7 }, '7'));
				REQUIRE(tile.GetPixel({ 0, 0 }) == '7');
				REQUIRE(tile.GetPixel({ 7, 7 }) == '7');
			}
		}
	}
}