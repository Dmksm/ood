#pragma once
#include <memory>
#include <string>
#include "IParagraph.h"
#include "IImage.h"

class CConstDocumentItem
{
public:
	CConstDocumentItem(std::shared_ptr<const IImage> image);
	CConstDocumentItem(std::shared_ptr<const IParagraph> paragraph);
	std::shared_ptr<const IImage> GetImage()const;
	std::shared_ptr<const IParagraph> GetParagraph()const;
	virtual ~CConstDocumentItem() = default;
private:
	std::shared_ptr<const IImage> m_image = nullptr;
	std::shared_ptr<const IParagraph> m_paragraph = nullptr;
};