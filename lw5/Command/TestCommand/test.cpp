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
		if (position.has_value() && !m_items.at(position.get()).GetParagraph())
		{
			std::stringstream ss;
			ss << "Element in position " << position << " is not a text!" << std::endl;
			throw std::logic_error(ss.str());
		}
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
		if (position.has_value() && !m_items.at(position.get()).GetImage())
		{
			std::stringstream ss;
			ss << "Element in position " << position << " is not a image!" << std::endl;
			throw std::logic_error(ss.str());
		}
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
		WHEN("Initialized document")
		{
			REQUIRE(doc.CanRedo() == false);
			REQUIRE(doc.CanUndo() == false);
			REQUIRE_THROWS_AS(doc.GetItem(1), std::logic_error);
			REQUIRE(doc.GetItemsCount() == 0);
			REQUIRE(doc.GetTitle() == "");
			REQUIRE_NOTHROW(doc.Redo());
			REQUIRE_NOTHROW(doc.Undo());

			REQUIRE(doc.m_history.CanUndo() == false);
			REQUIRE(doc.m_history.CanRedo() == false);
			REQUIRE_NOTHROW(doc.m_history.Redo());
			REQUIRE_NOTHROW(doc.m_history.Undo());

			THEN("Can use methods without errors")
			{
				REQUIRE_THROWS(doc.InsertImage("testUnexisted.jpg", 100, 50, boost::none));
				REQUIRE_THROWS(doc.InsertImage("test.jpg", 100, 50, 2));
				REQUIRE_THROWS(doc.InsertImage("test.jpg", 0, 50, boost::none));
				REQUIRE_THROWS(doc.InsertImage("test.jpg", 100, 0, boost::none));
				REQUIRE_THROWS(doc.InsertImage("test.jpg", 10001, 50, boost::none));
				REQUIRE_THROWS(doc.InsertImage("test.jpg", 100, 10001, boost::none));

				REQUIRE_NOTHROW(doc.InsertImage("test.jpg", 10000, 1, boost::none));

				REQUIRE(doc.m_items.front().GetImage()->GetWidth() == 10000);
				REQUIRE(doc.m_items.front().GetImage()->GetHeight() == 1);
				REQUIRE(doc.m_items.front().GetImage()->GetPath() == "test.jpg");

				REQUIRE(doc.m_history.CanUndo() == true);
				REQUIRE(doc.m_history.CanRedo() == false);

				REQUIRE_THROWS(doc.InsertParagraph("TEXT", -1));
				REQUIRE_THROWS(doc.InsertParagraph("TEXT", 3));

				REQUIRE(doc.m_items.size() == 1);
				REQUIRE_NOTHROW(doc.InsertParagraph("TEXT", boost::none));
				REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "TEXT");

				REQUIRE(doc.m_items.size() == 2);
				REQUIRE_THROWS(doc.DeleteItem(2));
				REQUIRE_NOTHROW(doc.DeleteItem(1));
				REQUIRE(doc.m_items.size() == 1);
				
				REQUIRE(doc.m_title == "");
				REQUIRE_NOTHROW(doc.SetTitle("MyTitle"));
				REQUIRE(doc.m_title == "MyTitle");
			}
		}
	}
}

SCENARIO("History limit operations")
{
	GIVEN("Document")
	{
		CMockDocument doc;
		WHEN("Initialized document")
		{
			REQUIRE(doc.m_history.CanUndo() == false);
			REQUIRE(doc.m_history.CanRedo() == false);
			REQUIRE_NOTHROW(doc.m_history.Redo());
			REQUIRE_NOTHROW(doc.m_history.Undo());
			REQUIRE_NOTHROW(doc.m_history.Redo());

			THEN("Throw error and catch it and handling it to get output mesage")
			{
				REQUIRE_NOTHROW(doc.InsertParagraph("1", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("2", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("3", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("4", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("5", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("6", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("7", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("8", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("9", boost::none));
				REQUIRE_NOTHROW(doc.InsertParagraph("10", boost::none));

				unsigned position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 10);

				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE(doc.CanUndo() == true);
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE(doc.CanUndo() == false);

				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 0);

				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE(doc.CanRedo() == true);
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE(doc.CanRedo() == false);

				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 10);

				REQUIRE_NOTHROW(doc.InsertParagraph("11", boost::none));

				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 11);


				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE(doc.CanUndo() == true);
				REQUIRE_NOTHROW(doc.Undo());
				REQUIRE(doc.CanUndo() == false);

				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 1);

				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE(doc.CanRedo() == true);
				REQUIRE_NOTHROW(doc.Redo());
				REQUIRE(doc.CanRedo() == false);

				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
				}
				REQUIRE(position == 11);
			}
		}
	}
}

SCENARIO("History delete old operations in redo state")
{
	GIVEN("Some operations in document")
	{
		CMockDocument doc;
		REQUIRE_NOTHROW(doc.InsertParagraph("1", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("2", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("3", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("4", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("5", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("6", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("7", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("8", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("9", boost::none));
		REQUIRE_NOTHROW(doc.InsertParagraph("10", boost::none));

		WHEN("Undo operation and add new")
		{
			REQUIRE_NOTHROW(doc.Undo());
			REQUIRE_NOTHROW(doc.Undo());
			REQUIRE_NOTHROW(doc.Undo());

			unsigned position = 0;
			for (auto it : doc.m_items)
			{
				REQUIRE(it.GetImage() == nullptr);
				REQUIRE(it.GetParagraph() != nullptr);
				REQUIRE(it.GetParagraph()->GetText() == std::to_string(++position));
			}
			REQUIRE(position == 7);

			REQUIRE_NOTHROW(doc.InsertParagraph("11", boost::none));
			REQUIRE(doc.m_items.size() == 8);
			THEN("New operation replace olds")
			{
				position = 0;
				for (auto it : doc.m_items)
				{
					REQUIRE(it.GetImage() == nullptr);
					REQUIRE(it.GetParagraph() != nullptr);
					++position;
					if (position == 8)
					{
						REQUIRE(it.GetParagraph()->GetText() == "11");
					}
					else
					{
						REQUIRE(it.GetParagraph()->GetText() == std::to_string(position));
					}
				}
				REQUIRE(position == 8);
			}
		}
	}
}
