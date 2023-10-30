#include "stdafx.h"
#include "InsertParagraphCommand.h"

CInsertParagraphCommand::CInsertParagraphCommand(
	std::vector<CDocumentItem>& target,
	CDocumentItem newValue,
	boost::optional<size_t> position
)
	: m_target(target)
	, m_newValue(newValue)
	, m_position(position)
{
}

void CInsertParagraphCommand::DoExecute()
{
	if (!m_position.has_value())
	{
		m_target.push_back(m_newValue);
	}
	else
	{
		m_target.insert(m_target.begin() + m_position.get(), m_newValue);
	}
}

void CInsertParagraphCommand::DoUnexecute()
{
	auto it = m_target.end();
	if (m_position.has_value())
	{
		it = m_target.begin() + m_position.get();
	}
	m_target.erase(it);
}