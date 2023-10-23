#pragma once
#include <memory>
#include <string>
#include <boost\optional.hpp>
#include <boost\none.hpp>
#include "IParagraph.h"
#include "IImage.h"
#include "ConstDocumentItem.h"
#include "DocumentItem.h"

class IDocument
{
public:


	virtual std::string GetTitle()const = 0;
	virtual void SetTitle(const std::string& title) = 0;

	virtual bool CanUndo()const = 0;
	virtual void Undo() = 0;

	virtual bool CanRedo()const = 0;
	virtual void Redo() = 0;

	virtual ~IDocument() = default;
};