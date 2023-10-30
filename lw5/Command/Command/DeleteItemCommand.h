#pragma once
#include "AbstractCommand.h"
#include "stdafx.h"
#include "DocumentItem.h"

class CDeleteItemCommand :
	public CAbstractCommand
{
public:
	CDeleteItemCommand(
		std::vector<CDocumentItem>& target,
		size_t position
	);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	std::vector<CDocumentItem>& m_target;
	size_t m_position;
	boost::optional<CDocumentItem> m_deletedValue;
};
