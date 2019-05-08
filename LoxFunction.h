#pragma once
#include "LoxCallable.h"
#include "Stmt.h"
#include "Environment.h"

class Interpreter;
class LoxInstance;

class LoxFunction : LoxCallable
{
public:
	LoxFunction(std::shared_ptr<FunctionStmt> declaration, const std::shared_ptr<Environment>& closure, bool isInitializer)
		: m_declaration(declaration),
		  m_closure(closure),
		  m_isInitializer(isInitializer)
	{}

	std::shared_ptr<void> Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args) override;

	const int GetArity() override { return m_declaration->m_params.size(); }

	std::shared_ptr<LoxFunction> Bind(const std::shared_ptr<LoxInstance>& instance);

private:
	std::shared_ptr<FunctionStmt> m_declaration;
	std::shared_ptr<Environment> m_closure;
	bool m_isInitializer;
};