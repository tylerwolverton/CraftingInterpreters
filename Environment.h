#pragma once
#include "Types.h"
#include <map>
#include <memory>

class Environment
{
public:
	Environment();
	Environment(const std::shared_ptr<Environment>& env);
	~Environment();

	void Define(std::string name, std::shared_ptr<Token> value);
	void Assign(Token name, std::shared_ptr<Token> value);
	std::shared_ptr<Token> Get(Token name);

private:
	std::map<std::string, std::shared_ptr<Token>> m_varToTokenMap;
	std::shared_ptr<Environment> m_enclosingEnv;
};

