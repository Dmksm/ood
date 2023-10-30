#pragma once
#include "IDocument.h"
#include "History.h"
#include "stdafx.h"

class CDocument : public IDocument
{
public:
	std::shared_ptr<IParagraph> InsertParagraph(const std::string& text,
		boost::optional<size_t> position = boost::none) override;

	void ReplaceText(const std::string& text, size_t position) override;

	std::shared_ptr<IImage> InsertImage(const Path& path, int width, int height,
		boost::optional<size_t> position = boost::none) override;

	size_t GetItemsCount()const override;

	CConstDocumentItem GetItem(size_t index)const override;
	CDocumentItem GetItem(size_t index) override;

	void DeleteItem(size_t index) override;

	void Save(const Path& path)const override;

	void SetTitle(const std::string& title) override;
	std::string GetTitle() const override;

	bool CanUndo() const override;
	void Undo() override;
	bool CanRedo() const override;
	void Redo() override;

private:
	std::vector<CDocumentItem> m_items;
	std::string m_title;
	CHistory m_history;

	void ValidateIndex(size_t index) const;
};