#pragma once
#include <memory>
#include "IParagraph.h"

class CParagraphImpl;

class CParagraph :public IParagraph
{
public:
	CParagraph(const std::string& text) : m_text(text)
	{
	};

	std::string GetText()const override;
	void SetText(const std::string& text) override;

private:
	std::string m_text = "";
};
