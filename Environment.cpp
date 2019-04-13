#include "Environment.h"
#include "Error.h"

Environment::Environment()
{
	m_enclosingEnv = nullptr;
}

Environment::Environment(const std::shared_ptr<Environment>& env)
	: m_enclosingEnv(env)
{}

Environment::~Environment()
{
	m_varToTokenMap.clear();
}

void Environment::Define(std::string name, std::shared_ptr<void> value)
{
	auto iter = m_varToTokenMap.insert(std::make_pair(name, value));
	if (!iter.second)
	{
		// Variable already exists, need to assign instead
		auto mapIter = m_varToTokenMap.find(name);
		mapIter->second = value;
	}
}

void Environment::Assign(Token name, std::shared_ptr<void> value)
{
	auto mapIter = m_varToTokenMap.find(name.GetLexeme());
	if (mapIter != m_varToTokenMap.end())
	{
		mapIter->second = value;
		return;
	}
	
	if (m_enclosingEnv != nullptr)
	{
		m_enclosingEnv->Assign(name, value);
		return;
	}

	throw RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
}

std::shared_ptr<void> Environment::Get(Token name)
{
	auto mapIter = m_varToTokenMap.find(name.GetLexeme());
	if (mapIter != m_varToTokenMap.end())
	{
		return mapIter->second;
	}

	if (m_enclosingEnv != nullptr)
	{
		return m_enclosingEnv->Get(name);
	}

	throw RuntimeError(name, "Undefined variable '" + name.GetLexeme() + "'.");
}