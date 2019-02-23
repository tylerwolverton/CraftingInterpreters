#include "Environment.h"
#include "Error.h"

Environment::Environment()
{
	//m_varToTokenMap = std::map<std::string, std::shared_ptr<Token>>();
}

Environment::~Environment()
{
	m_varToTokenMap.clear();
}

void Environment::Define(std::string name, std::shared_ptr<Token> value)
{
	m_varToTokenMap.insert(std::make_pair(name, value));
}

std::shared_ptr<Token> Environment::Get(Token name)
{
	auto mapIter = m_varToTokenMap.find(name.GetLexeme());
	if (mapIter != m_varToTokenMap.end())
	{
		return mapIter->second;
	}

	throw new RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
}