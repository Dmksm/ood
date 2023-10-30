#pragma once
#include "AbstractCommand.h"
#include "stdafx.h"
#include "DocumentItem.h"

class CResizeImageCommand :
	public CAbstractCommand
{
public:
	CResizeImageCommand(
		CDocumentItem& target,
		int newWidth,
		int newHeight
	);

protected:
	void DoExecute() override;
	void DoUnexecute() override;

private:
	CDocumentItem& m_target;
	int m_newWidth;
	int m_newHeight;
};
