// ISpringWord.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Menu.h"
#include "Document.h"

using namespace std;
using namespace std::placeholders;

namespace
{

	class CEditor
	{
	public:
		CEditor() 
			:m_document(make_unique<CDocument>())
		{
			m_menu.AddItem("help", "Help", [this](istream&) { m_menu.ShowInstructions(); });
			m_menu.AddItem("exit", "Exit", [this](istream&) { m_menu.Exit(); });
			AddMenuItem("setTitle", "Changes title. Args: <new title>", &CEditor::SetTitle);
			AddMenuItem("deleteItem", "Delete item. Args: <position>", &CEditor::DeleteItem);
			AddMenuItem("insertParagraph", "Insert paragraph. Args: <position>|end <text>", &CEditor::InsertParagraph);
			AddMenuItem("insertImage", "Insert image. Args: <position>|end <width> <height> <path>", &CEditor::InsertImage);
			AddMenuItem("resizeImage", "Resize image. Args: <position> <width> <height>", &CEditor::ResizeImage);
			AddMenuItem("replaceText", "Replace text. Args: <position> <text>", &CEditor::ReplaceText);
			AddMenuItem("save", "Save document to html file. Args: <path>", &CEditor::Save);
			m_menu.AddItem("list", "Show document", bind(&CEditor::List, this, _1));
			AddMenuItem("undo", "Undo command", &CEditor::Undo);
			AddMenuItem("redo", "Redo undone command", &CEditor::Redo);
		}

		void Start()
		{
			m_menu.Run();
		}

	private:
		// Указатель на метод класса CEditor, принимающий istream& и возвращающий void
		typedef void (CEditor::* MenuHandler)(istream& in);

		void AddMenuItem(const string& shortcut, const string& description, MenuHandler handler)
		{
			m_menu.AddItem(shortcut, description, bind(handler, this, _1));
		}

		void Save(istream& in)
		{
			string path;

			in >> path;
			try
			{
				m_document->Save(path);
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void DeleteItem(istream& in)
		{
			string text;
			string position;

			in >> position >> text;
			try
			{
				m_document->DeleteItem(stoi(position) - 1);
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void ReplaceText(istream& in)
		{
			string text;
			string position;

			in >> position >> text;
			try
			{
				m_document->ReplaceText(text, stoull(position) - 1);
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void ResizeImage(istream& in)
		{
			string width;
			string height;
			string position;

			in >> position >> width >> height;
			try
			{
				m_document->ResizeImage(stoull(position) - 1, stoi(width), stoi(height));
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void InsertImage(istream& in)
		{
			string width;
			string height;
			string path;
			string position;

			in >> position >> width >> height >> path; 
			try
			{
				if (position == "end")
				{
					m_document->InsertImage(path, stoi(width), stoi(height));
				}
				else
				{
					m_document->InsertImage(path, stoi(width), stoi(height), stoull(position) - 1);
				}
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void InsertParagraph(istream& in)
		{
			string text;
			string position;

			in >> position >> text;
			try
			{
				if (position == "end")
				{
					m_document->InsertParagraph(text);
				}
				else
				{
					m_document->InsertParagraph(text, stoull(position) - 1);
				}
			}
			catch (std::exception& e)
			{
				cout << e.what() << endl;
			}
		}

		void SetTitle(istream& in)
		{
			string head;
			string tail;

			if (in >> head)
			{
				getline(in, tail);
			}
			string title = head + tail;

			m_document->SetTitle(title);
		}

		void List(istream&)
		{
			cout << "-------------" << endl;
			cout << m_document->GetTitle() << endl;
			cout << "-------------" << endl;
			for (size_t position = 1; position <= m_document->GetItemsCount(); ++position)
			{
				CConstDocumentItem item = m_document->GetItem(position - 1);
				cout << position << ". ";
				if (item.GetImage())
				{
					cout << item.GetImage()->GetWidth() << " " 
						<< item.GetImage()->GetHeight() << " "
						<< item.GetImage()->GetPath() << endl;
				}
				if (item.GetParagraph())
				{
					cout << item.GetParagraph()->GetText() << endl;
				}
			}
		}

		void Undo(istream&)
		{
			if (m_document->CanUndo())
			{
				m_document->Undo();
			}
			else
			{
				cout << "Can't undo" << endl;
			}
		}

		void Redo(istream&)
		{
			if (m_document->CanRedo())
			{
				m_document->Redo();
			}
			else
			{
				cout << "Can't redo" << endl;
			}
		}

		CMenu m_menu;
		unique_ptr<IDocument> m_document;
	};

}

int main()
{
	setlocale(LC_ALL, "Russian");
	CEditor editor;
	editor.Start();
	return 0;
}
