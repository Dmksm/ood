#pragma once
#include "stdafx.h"

namespace graphics_lib_pro
{
	// ����� ��� ���������
	class ICanvas
	{
	public:
		// ��������� ����� � ������� 0xRRGGBB
		virtual void SetColor(uint32_t rgbColor) = 0;
		virtual void MoveTo(int x, int y) = 0;
		virtual void LineTo(int x, int y) = 0;
		virtual ~ICanvas() = default;
	};

	// ���������� ������ ��� ���������
	class CCanvas : public ICanvas
	{
	public:
		void SetColor(uint32_t rgbColor) override
		{
			std::cout << "SetColor (" << rgbColor << ")" << std::endl;
		}
		void MoveTo(int x, int y) override
		{
			std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
		}
		void LineTo(int x, int y) override
		{
			std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
		}
	};
}