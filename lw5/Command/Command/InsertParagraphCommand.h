#pragma once
#include "AbstractCommand.h"
#include "stdafx.h"
#include "DocumentItem.h"

class CInsertParagraphCommand :
	public CAbstractCommand
{
public:
	CInsertParagraphCommand(
		std::vector<CDocumentItem>& target,
		CDocumentItem newValue,
		boost::optional<size_t> position
	);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	std::vector<CDocumentItem>& m_target;
	CDocumentItem m_newValue;
	boost::optional<size_t> m_position;
};
