#include "LoxFunction.h"
#include "Interpreter.h"
#include "Error.h"

std::shared_ptr<void> LoxFunction::Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args)
{
	std::shared_ptr<Environment> environment = std::make_shared<Environment>(interpreter->GetGlobalEnv());
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