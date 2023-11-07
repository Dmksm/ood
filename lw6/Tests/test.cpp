#define CATCH_CONFIG_MAIN
#include "../../catch2/catch.hpp"
#include "../lw6/stdafx.h"
#include "../lw6/ShapeDrawingLib.h"
#include "../lw6/GraphicsLib.h"
#include "../lw6/ModernGraphicsLib.h"
#include "../lw6/ModernGraphicsLibPro.h"
#include "../lw6/ShapeDrawingLibPro.h"
#include "../lw6/GraphicsLibPro.h"
#include "../lw6/App.h"
#include "../lw6/AppProEdition.h"

namespace
{
	class CMockCanvas : public graphics_lib::ICanvas
	{
	public:
		CMockCanvas(std::ostream& strm) : m_out(strm)
		{
		};

		void MoveTo(int x, int y) override
		{
			m_out << "MoveTo (" << x << ", " << y << ")" << std::endl;
		}
		void LineTo(int x, int y) override
		{
			m_out << "LineTo (" << x << ", " << y << ")" << std::endl;
		}
	private:
		std::ostream& m_out;
	};

	class CMockCanvasPro : public graphics_lib_pro::ICanvas
	{
	public:
		CMockCanvasPro(std::ostream& strm): m_out(strm)
		{
		};

		void SetColor(uint32_t rgbColor) override
		{
			m_out << "SetColor (" << rgbColor << ")" << std::endl;
		}
		void MoveTo(int x, int y) override
		{
			m_out << "MoveTo (" << x << ", " << y << ")" << std::endl;
		}
		void LineTo(int x, int y) override
		{
			m_out << "LineTo (" << x << ", " << y << ")" << std::endl;
		}
	private:
		std::ostream& m_out;
	};
}

SCENARIO("Adapter operations")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss;
		std::stringstream ss2;
		WHEN("Draw on canvas and canvas adapter")
		{
			int x = 10;
			int y = 15;
			int x2 = 20;
			int y2 = 35;
			{
				modern_graphics_lib::CModernGraphicsRenderer renderer(ss);
				CMockCanvas canvas(ss2);
				app::CCanvasAdapter adapter(renderer);
				REQUIRE_THROWS(adapter.LineTo(x2, y2));
				renderer.BeginDraw();
				adapter.MoveTo(x, y);
				adapter.LineTo(x2, y2);
				renderer.EndDraw();

				canvas.MoveTo(x, y);
				canvas.LineTo(x2, y2);
			}
			THEN("get old graphic output and modern graphic output")
			{
				std::stringstream outStream;
				outStream << "<draw>" << std::endl
					<< "  <line fromX=\"" << x << "\" fromY=\"" << y 
					<< "\" toX=\"" << x2 << "\" toY=\"" << y2 << "\"/>" << std::endl
					<< "</draw>" << std::endl;
				REQUIRE(ss.str() == outStream.str());

				std::stringstream outStream2;
				outStream2 << "MoveTo (" << x << ", " << y << ")" << std::endl
					<< "LineTo (" << x2 << ", " << y2 << ")" << std::endl;
				REQUIRE(ss2.str() == outStream2.str());
			}
		}
	}
}

SCENARIO("Class adapter operations")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss;
		std::stringstream ss2;

		WHEN("Draw on canvas and canvas adapter")
		{
			int x = 10;
			int y = 15;
			int x2 = 20;
			int y2 = 35;
			{
				app::CCanvasClassAdapter adapter(ss);
				CMockCanvas canvas(ss2);
				adapter.MoveTo(x, y);
				adapter.LineTo(x2, y2);
				canvas.MoveTo(x, y);
				canvas.LineTo(x2, y2);
			}
			THEN("get old graphic output and modern graphic output")
			{
				std::stringstream outStream;
				outStream << "<draw>" << std::endl
					<< "  <line fromX=\"" << x << "\" fromY=\"" << y
					<< "\" toX=\"" << x2 << "\" toY=\"" << y2 << "\"/>" << std::endl
					<< "</draw>" << std::endl;
				REQUIRE(ss.str() == outStream.str());

				std::stringstream outStream2;
				outStream2 << "MoveTo (" << x << ", " << y << ")" << std::endl
					<< "LineTo (" << x2 << ", " << y2 << ")" << std::endl;
				REQUIRE(ss2.str() == outStream2.str());
			}
		}
	}
}

SCENARIO("Pro adapter operations")
{
	GIVEN("Streams and line points and color")
	{
		std::stringstream ss;
		std::stringstream ss2;
		WHEN("Draw on canvas and canvas adapter")
		{
			int x = 10;
			int y = 15;
			int x2 = 30;
			int y2 = 65;
			uint32_t color = 256 * 256 * 256 - 1;
			{
				modern_graphics_lib_pro::CModernGraphicsRenderer renderer(ss);
				CMockCanvasPro canvas(ss2);
				app_pro::CCanvasAdapter adapter(renderer);
				adapter.SetColor(color);
				REQUIRE_THROWS(adapter.LineTo(x, y));
				renderer.BeginDraw();
				adapter.MoveTo(x, y);
				adapter.LineTo(x2, y2);
				renderer.EndDraw();

				canvas.SetColor(color);
				canvas.MoveTo(x, y);
				canvas.LineTo(x2, y2);
			}
			THEN("get old graphic output and modern graphic output")
			{
				std::stringstream outStream;
				outStream << "<draw>" << std::endl
					<< "  <line fromX=\"" << x << "\" fromY=\"" << y
					<< "\" toX=\"" << x2 << "\" toY=\"" << y2 << "\">"
					<< "<color r=\"" << 1 << "\" g=\"" << 1 << "\" b=\"" << 1 
					<< "\" a=\"" << 1 << "\"/><line/>" << std::endl
					<< "</draw>" << std::endl;
				REQUIRE(ss.str() == outStream.str());

				std::stringstream outStream2;
				outStream2 << "SetColor (" << color << ")" << std::endl
					<< "MoveTo (" << x << ", " << y << ")" << std::endl
					<< "LineTo (" << x2 << ", " << y2 << ")" << std::endl;
				REQUIRE(ss2.str() == outStream2.str());
			}
		}
	}
}

SCENARIO("Pro class adapter operations")
{
	GIVEN("Streams and line points")
	{
		std::stringstream ss;
		std::stringstream ss2;
		WHEN("Draw on canvas and canvas adapter")
		{
			int x = 1;
			int y = 5;
			int x2 = 10;
			int y2 = 15;
			uint32_t color = 256 * 256 * 256 - 1;
			{
				modern_graphics_lib_pro::CModernGraphicsRenderer renderer(ss);
				app_pro::CCanvasClassAdapter adapter(ss);

				CMockCanvasPro canvas(ss2);
				adapter.SetColor(color);
				adapter.MoveTo(x, y);
				adapter.LineTo(x2, y2);

				canvas.SetColor(color);
				canvas.MoveTo(x, y);
				canvas.LineTo(x2, y2);
			}
			THEN("get old graphic output and modern graphic output")
			{

				std::stringstream outStream;
				outStream << "<draw>" << std::endl
					<< "  <line fromX=\"" << x << "\" fromY=\"" << y
					<< "\" toX=\"" << x2 << "\" toY=\"" << y2 << "\">"
					<< "<color r=\"" << 1 << "\" g=\"" << 1 << "\" b=\"" << 1
					<< "\" a=\"" << 1 << "\"/><line/>" << std::endl
					<< "</draw>" << std::endl;
				REQUIRE(ss.str() == outStream.str());

				std::stringstream outStream2;
				outStream2 << "SetColor (" << color << ")" << std::endl
					<< "MoveTo (" << x << ", " << y << ")" << std::endl
					<< "LineTo (" << x2 << ", " << y2 << ")" << std::endl;
				REQUIRE(ss2.str() == outStream2.str());
			}
		}
	}
}