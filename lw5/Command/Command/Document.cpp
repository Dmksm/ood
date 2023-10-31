#include "stdafx.h"
#include "Document.h"
#include "Image.h"
#include "Paragraph.h"
#include "ChangeStringCommand.h"
#include "InsertParagraphCommand.h"
#include "DeleteItemCommand.h"
#include "InsertImageCommand.h"
#include "ReplaceTextCommand.h"
#include "ResizeImageCommand.h"

std::string ToHTML(CConstDocumentItem item);

std::shared_ptr<IParagraph> CDocument::InsertParagraph(const std::string& text,
	boost::optional<size_t> position) 
{
	std::shared_ptr<IParagraph> paragraphPtr = std::make_shared<CParagraph>(text);
	m_history.AddAndExecuteCommand(
		std::make_unique<CInsertParagraphCommand>(m_items, paragraphPtr, position)
	);
	return paragraphPtr;
}

void CDocument::ReplaceText(const std::string& text, size_t position)
{
	if (!m_items.at(position).GetParagraph())
	{
		std::stringstream ss;
		ss << "Element in position " << position << " is not a text!" << std::endl;
		throw std::logic_error(ss.str());
	}

	m_history.AddAndExecuteCommand(
		std::make_unique<CReplaceTextCommand>(m_items.at(position), text)
	);
}

std::shared_ptr<IImage> CDocument::InsertImage(const Path& path, int width, int height,
	boost::optional<size_t> position)
{
	std::shared_ptr<IImage> imagePtr = std::make_shared<CImage>(path, width, height);
	m_history.AddAndExecuteCommand(std::make_unique<CInsertImageCommand>(m_items, imagePtr, position));
	return imagePtr;
}

void CDocument::ResizeImage(size_t position, int width, int height)
{
	if (!m_items.at(position).GetImage())
	{
		std::stringstream ss;
		ss << "Element in position " << position << " is not a image!" << std::endl;
		throw std::logic_error(ss.str());
	}

	m_history.AddAndExecuteCommand(
		std::make_unique<CResizeImageCommand>(m_items.at(position), width, height)
	);
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
	m_history.AddAndExecuteCommand(std::make_unique<CDeleteItemCommand>(m_items, index));
}

void CDocument::Save(const Path& path)const
{
	std::ofstream file(path.string());
	file << "<!DOCTYPE html><html><head><title>" << m_title << "</title></head><body>";

	file << "<ol>";
	for (auto item : m_items)
	{
		file << "<li>" << ToHTML(item) << "</li>";
	}
	file << "</ol>";

	file << "</body></html>";
	file.close();
}

void CDocument::SetTitle(const std::string& title)
{
	m_history.AddAndExecuteCommand(std::make_unique<CChangeStringCommand>(m_title, title));
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

std::string ToHTML(CConstDocumentItem item)
{
	std::stringstream ss;
	if (item.GetImage())
	{
		ss << "<img src=" << item.GetImage()->GetPath() << " width = " <<
			item.GetImage()->GetWidth() << " height = " << item.GetImage()->GetHeight()
			<< " alt = 'image' >";
	}
	else
	{
		ss << item.GetParagraph()->GetText();
	}
	return ss.str();
}