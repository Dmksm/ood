#pragma once
#include "ConstDocumentItem.h"

class CDocumentItem : public CConstDocumentItem
{
public:
	CDocumentItem(std::shared_ptr<IImage> image);
	CDocumentItem(std::shared_ptr<IParagraph> paragraph);

	std::shared_ptr<IImage> GetImage() const;
	std::shared_ptr<IParagraph> GetParagraph() const;

private:
	std::shared_ptr<IImage> m_image = nullptr;
	std::shared_ptr<IParagraph> m_paragraph = nullptr;
};