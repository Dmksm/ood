#include "stdafx.h"
#include "Image.h"

void ValidateSize(int width, int height);

CImage::CImage(const Path& path, int width, int height)
	: m_path(path)
{
	Resize(width, height);
};

Path CImage::GetPath()const
{
	return m_path;
};

int CImage::GetWidth()const 
{
	return m_width;
};

int CImage::GetHeight()const
{
	return m_height;
};

void CImage::Resize(int width, int height)
{
	ValidateSize(width, height);
	m_width = width;
	m_height = height;
};

void ValidateSize(int width, int height)
{
	if (width > 10000 || width < 1 || height > 10000 || height < 1)
	{
		std::stringstream ss;
		ss << "image size must be at range from 1 to 10000. Given width and height: "
			<< width << " and " << height;
		throw std::logic_error(ss.str());
	}
}