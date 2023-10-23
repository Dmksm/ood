#pragma once
#include "IImage.h"
#include "stdafx.h"

typedef boost::filesystem::path Path;

class CImage: public IImage
{
public:
	CImage(const Path& path, int width, int height);

	Path GetPath()const override;

	int GetWidth()const override;
	int GetHeight()const override;

	void Resize(int width, int height) override;
private:
	Path m_path;
	int m_width;
	int m_height;
};