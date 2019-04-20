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

	void Define(std::string name, std::shared_ptr<void> value);
	void Assign(Token name, std::shared_ptr<void> value);
	void AssignAt(int dist, Token name, std::shared_ptr<void> value);
	std::shared_ptr<void> Get(Token name);
	std::shared_ptr<void> GetAt(int dist, Token name);

private:
	std::map<std::string, std::shared_ptr<void>> m_varToTokenMap;
	std::shared_ptr<Environment> m_enclosingEnv;

	std::shared_ptr<Environment> ancestor(int dist);
};

