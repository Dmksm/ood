#include "stdafx.h"
#include "Image.h"

CImage::CImage(const Path& path, int width, int height)
	: m_path(path)
	, m_width(width)
	, m_height(height)
{
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
	m_width = width;
	m_height = height;
};