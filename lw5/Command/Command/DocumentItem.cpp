#include "stdafx.h"
#include "DocumentItem.h"

CDocumentItem::CDocumentItem(std::shared_ptr<IImage> image)
	: CConstDocumentItem(image)
	, m_image(image)
{
}

CDocumentItem::CDocumentItem(std::shared_ptr<IParagraph> paragraph)
	: CConstDocumentItem(paragraph)
	, m_paragraph(paragraph)
{
}

std::shared_ptr<IImage> CDocumentItem::GetImage() const
{
	return m_image;
}

std::shared_ptr<IParagraph> CDocumentItem::GetParagraph() const
{
	return m_paragraph;
}