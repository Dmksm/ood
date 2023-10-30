#include "stdafx.h"
#include "Document.h"
#include "Image.h"
#include "Paragraph.h"
#include "ChangeStringCommand.h"
#include "InsertParagraphCommand.h"
#include "DeleteItemCommand.h"
#include "InsertImageCommand.h"

std::shared_ptr<IParagraph> CDocument::InsertParagraph(const std::string& text,
	boost::optional<size_t> position) 
{
	std::shared_ptr<IParagraph> paragraphPtr = std::make_shared<CParagraph>(text);
	m_history.AddAndExecuteCommand(
		std::make_unique<CInsertParagraphCommand>(m_items, paragraphPtr, position)
	);
	return paragraphPtr;
}

std::shared_ptr<IImage> CDocument::InsertImage(const Path& path, int width, int height,
	boost::optional<size_t> position)
{
	std::shared_ptr<IImage> imagePtr = std::make_shared<CImage>(path, width, height);
	m_history.AddAndExecuteCommand(std::make_unique<CInsertImageCommand>(m_items, imagePtr, position));
	return imagePtr;
}

size_t CDocument::GetItemsCount()const
{
	return m_items.size();
}

CConstDocumentItem CDocument::GetItem(size_t index)const
{
	ValidateIndex(index);
	return m_items.at(index);
}

CDocumentItem CDocument::GetItem(size_t index)
{
	ValidateIndex(index);
	return m_items.at(index);
}

void CDocument::DeleteItem(size_t index)
{
	ValidateIndex(index);
	m_history.AddAndExecuteCommand(make_unique<CDeleteItemCommand>(m_items, index));
}

void CDocument::Save(const Path& path)const
{

}

void CDocument::SetTitle(const std::string& title)
{
	m_history.AddAndExecuteCommand(make_unique<CChangeStringCommand>(m_title, title));
}

std::string CDocument::GetTitle() const
{
	return m_title;
}

bool CDocument::CanUndo() const
{
	return m_history.CanUndo();
}

void CDocument::Undo()
{
	m_history.Undo();
}

bool CDocument::CanRedo() const
{
	return m_history.CanRedo();
}

void CDocument::Redo()
{
	m_history.Redo();
}

void CDocument::ValidateIndex(size_t index) const
{
	if (index > m_items.size() || (index == m_items.size() && m_items.size() > 0))
	{
		std::stringstream msg;
		msg << "Index out of range! Given " << index;
		throw std::logic_error(msg.str());
	}
}