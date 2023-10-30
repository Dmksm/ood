#include "stdafx.h"
#include "DeleteItemCommand.h"

CDeleteItemCommand::CDeleteItemCommand(
	std::vector<CDocumentItem>& target,
	size_t position
)
	: m_target(target)
	, m_position(position)
{
}

void CDeleteItemCommand::DoExecute()
{
	auto it = m_target.begin() + m_position;
	m_deletedValue = *it;
	m_target.erase(it);
}

void CDeleteItemCommand::DoUnexecute()
{
	if (m_deletedValue.has_value())
	{
		m_target.insert(m_target.begin() + m_position, m_deletedValue.get());
	}
}