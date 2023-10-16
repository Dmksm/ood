#pragma once
#include "IShapeFactory.h"

class CShapeFactory : public IShapeFactory
{
public:
	CShape CreateShape(const std::string& description);
private:
	std::string m_memberName;
};