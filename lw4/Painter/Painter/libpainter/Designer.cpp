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

	std::cout << "enter 'stop' to stop drafting" << std::endl;
	while (std::getline(inputData, line) && line != "stop")
	{
		try
		{
			draft.AddShape(m_factory.CreateShape(line));
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return draft;
}