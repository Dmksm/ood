#pragma once
#include "AbstractCommand.h"
#include "stdafx.h"
#include "DocumentItem.h"

class CReplaceTextCommand :
	public CAbstractCommand
{
public:
	CReplaceTextCommand(
		CDocumentItem& target,
		std::string newValue
	);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	CDocumentItem& m_target;
	std::string m_newValue;
};
