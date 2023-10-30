#include "stdafx.h"
#include "ResizeImageCommand.h"

CResizeImageCommand::CResizeImageCommand(
	CDocumentItem& target,
	int newWidth,
	int newHeight
)
	: m_target(target)
	, m_newWidth(newWidth)
	, m_newHeight(newHeight)
{
}

void Swap(CDocumentItem& target, int& newWidth, int& newHeight);

void CResizeImageCommand::DoExecute()
{
	Swap(m_target, m_newWidth, m_newHeight);
}

void CResizeImageCommand::DoUnexecute()
{
	Swap(m_target, m_newWidth, m_newHeight);
}

void Swap(CDocumentItem& target, int& newWidth, int& newHeight)
{
	int heightValue = target.GetImage()->GetHeight();
	int widthValue = target.GetImage()->GetWidth();
	target.GetImage()->Resize(newWidth, newHeight);
	newWidth = widthValue;
	newHeight = heightValue;
}