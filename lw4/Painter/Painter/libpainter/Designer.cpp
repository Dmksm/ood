#include "Designer.h"
#include "PictureDraft.h"
#include "IShapeFactory.h"

CDesigner::CDesigner(IShapeFactory& factory)
	:m_factory(factory)
{
}


CDesigner::~CDesigner()
{
}

CPictureDraft CDesigner::CreateDraft(std::istream& inputData)
{
	CPictureDraft draft;
	std::string line;
	while (std::getline(inputData, line))
	{
		draft.AddShape(m_factory.CreateShape(line));
	}
	return draft;
}