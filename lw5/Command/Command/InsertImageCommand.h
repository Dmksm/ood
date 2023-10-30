#pragma once
#include "AbstractCommand.h"
#include "stdafx.h"
#include "DocumentItem.h"

class CInsertImageCommand :
	public CAbstractCommand
{
public:
	CInsertImageCommand(
		std::vector<CDocumentItem>& target,
		CDocumentItem newValue,
		boost::optional<size_t> position
	);

	~CInsertImageCommand();

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	std::vector<CDocumentItem>& m_target;
	CDocumentItem m_newValue;
	boost::optional<size_t> m_position;
	std::string m_fileName;
	bool m_isDeleted = true;
};