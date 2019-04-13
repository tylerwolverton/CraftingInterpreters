#pragma once
#include "LoxCallable.h"
#include "Stmt.h"
#include "Environment.h"

class Interpreter;

class LoxFunction : LoxCallable
{
public:
	LoxFunction(std::shared_ptr<FunctionStmt> declaration, const std::shared_ptr<Environment>& closure)
		: m_declaration(declaration),
		  m_closure(closure)
	{}

	std::shared_ptr<void> Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args) override;

	const int GetArity() const override { return m_declaration->m_params.size(); }

private:
	std::shared_ptr<FunctionStmt> m_declaration;
	std::shared_ptr<Environment> m_closure;
};