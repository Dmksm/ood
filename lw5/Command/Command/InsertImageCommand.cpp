#include "stdafx.h"
#include "InsertImageCommand.h"

CInsertImageCommand::CInsertImageCommand(
	std::vector<CDocumentItem>& target,
	CDocumentItem newValue,
	boost::optional<size_t> position
)
	: m_target(target)
	, m_newValue(newValue)
	, m_position(position)
{
	const std::string SUB_FOLDER = "images";
	const char CATALOG_SEPARATOR = '/';
	const char FORMAT_SEPARATOR = '.';

	boost::uuids::uuid u = boost::uuids::random_generator()();
	std::string fileName = m_newValue.GetImage()->GetPath().string();
	std::string format = fileName.substr(fileName.find(FORMAT_SEPARATOR));
	std::string newFileName = SUB_FOLDER + CATALOG_SEPARATOR + boost::uuids::to_string(u)
		+ FORMAT_SEPARATOR + format;

	boost::filesystem::copy_file(fileName, newFileName);
	m_fileName = newFileName;
}

void CInsertImageCommand::DoExecute()
{
	if (!m_position.has_value())
	{
		m_target.push_back(m_newValue);
	}
	else
	{
		m_target.insert(m_target.begin() + m_position.get(), m_newValue);
	}

	m_isDeleted = false;
}

void CInsertImageCommand::DoUnexecute()
{
	auto it = m_target.end() - 1;
	if (m_position.has_value())
	{
		it = m_target.begin() + m_position.get();
	}
	m_target.erase(it);
	m_isDeleted = true;
}

CInsertImageCommand::~CInsertImageCommand()
{
	if (m_isDeleted)
	{
		boost::filesystem::remove(m_fileName);
	}
}