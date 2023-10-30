#include "stdafx.h"
#include "ReplaceTextCommand.h"

CReplaceTextCommand::CReplaceTextCommand(
	CDocumentItem& target,
	std::string newValue
)
	: m_target(target)
	, m_newValue(newValue)
{
}

void Swap(CDocumentItem& target, std::string& newValue);

void CReplaceTextCommand::DoExecute()
{
	Swap(m_target, m_newValue);
}

void CReplaceTextCommand::DoUnexecute()
{
	Swap(m_target, m_newValue);
}

void Swap(CDocumentItem& target, std::string& newValue)
{
	std::string value = target.GetParagraph()->GetText();
	target.GetParagraph()->SetText(newValue);
	newValue = value;
}