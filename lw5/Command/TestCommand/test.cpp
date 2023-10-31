#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../Command/stdafx.h"
#include "../Command/Document.h"
#include "../Command/Image.h"
#include "../Command/Paragraph.h"
#include "../Command/ChangeStringCommand.h"
#include "../Command/InsertParagraphCommand.h"
#include "../Command/DeleteItemCommand.h"
#include "../Command/InsertImageCommand.h"
#include "../Command/ReplaceTextCommand.h"
#include "../Command/ResizeImageCommand.h"

class CMockDocument : public IDocument
{
public:

	std::shared_ptr<IParagraph>  InsertParagraph(const std::string& text,
		boost::optional<size_t> position)
	{
		std::shared_ptr<IParagraph> paragraphPtr = std::make_shared<CParagraph>(text);
		m_history.AddAndExecuteCommand(
			std::make_unique<CInsertParagraphCommand>(m_items, paragraphPtr, position)
		);
		return paragraphPtr;
	}

	void  ReplaceText(const std::string& text, size_t position)
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

	std::shared_ptr<IImage>  InsertImage(const Path& path, int width, int height,
		boost::optional<size_t> position)
	{
		std::shared_ptr<IImage> imagePtr = std::make_shared<CImage>(path, width, height);
		m_history.AddAndExecuteCommand(std::make_unique<CInsertImageCommand>(m_items, imagePtr, position));
		return imagePtr;
	}

	void  ResizeImage(size_t position, int width, int height)
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

	size_t  GetItemsCount()const
	{
		return m_items.size();
	}

	CConstDocumentItem  GetItem(size_t index)const
	{
		ValidateIndex(index);
		return m_items.at(index);
	}

	CDocumentItem  GetItem(size_t index)
	{
		ValidateIndex(index);
		return m_items.at(index);
	}

	void  DeleteItem(size_t index)
	{
		ValidateIndex(index);
		m_history.AddAndExecuteCommand(std::make_unique<CDeleteItemCommand>(m_items, index));
	}

	void  Save(const Path& path)const
	{
	}

	void  SetTitle(const std::string& title)
	{
		m_history.AddAndExecuteCommand(std::make_unique<CChangeStringCommand>(m_title, title));
	}

	std::string  GetTitle() const
	{
		return m_title;
	}

	bool  CanUndo() const
	{
		return m_history.CanUndo();
	}

	void  Undo()
	{
		m_history.Undo();
	}

	bool  CanRedo() const
	{
		return m_history.CanRedo();
	}

	void  Redo()
	{
		m_history.Redo();
	}

	void  ValidateIndex(size_t index) const
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

	std::vector<CDocumentItem> m_items;
	std::string m_title;
	CHistory m_history;
};

SCENARIO("Document operations")
{
	GIVEN("Document")
	{
		CMockDocument doc;
		WHEN("Given wrong type from input")
		{
			REQUIRE(doc.CanRedo() == false);
			REQUIRE(doc.CanUndo() == false);

			THEN("Throw error and catch it and handling it to get output mesage")
			{
			}
		}
	}
}

SCENARIO("Picture draft adding shapes and it can be walked in a loop")
{

}
