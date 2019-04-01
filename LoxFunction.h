#pragma once
#include "LoxCallable.h"
#include "Stmt.h"

class Interpreter;

class LoxFunction : LoxCallable
{
public:
	LoxFunction(std::shared_ptr<FunctionStmt> declaration)
		: m_declaration(declaration)
	{}

	std::shared_ptr<void> Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args) override;

	const int GetArity() const override { return m_declaration->m_params.size(); }

private:
	std::shared_ptr<FunctionStmt> m_declaration;
};