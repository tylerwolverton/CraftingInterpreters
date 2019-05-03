#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Interpreter.h"
#include "Error.h"

std::shared_ptr<void> LoxFunction::Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args)
{
	std::shared_ptr<Environment> environment = std::make_shared<Environment>(m_closure);
	for (int i = 0; i < m_declaration->m_params.size(); i++) 
	{
		environment->Define(m_declaration->m_params[i]->GetLexeme(), args[i]);
	}

	try
	{
		interpreter->executeBlock(m_declaration->m_body->m_statements, environment);
	}
	catch (ReturnException retEx)
	{
		return retEx.GetValue();
	}

	return nullptr;
}

std::shared_ptr<LoxFunction> LoxFunction::Bind(const std::shared_ptr<LoxInstance>& instance)
{
	std::shared_ptr<Environment> env = std::make_shared<Environment>(m_closure);
	env->Define("this", instance);
	return std::make_shared<LoxFunction>(m_declaration, env);
}