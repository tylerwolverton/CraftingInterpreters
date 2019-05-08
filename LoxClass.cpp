#include "LoxClass.h"
#include "LoxInstance.h"

std::shared_ptr<void> LoxClass::Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args)
{
	std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(*this);
	std::shared_ptr<LoxFunction> initializer = FindMethod(std::string("init"));
	if (initializer != nullptr)
	{
		initializer->Bind(instance)->Call(interpreter, args);
	}
	return instance;
}

std::shared_ptr<LoxFunction> LoxClass::FindMethod(std::string name)
{
	auto iter = m_methods.find(name);

	if (iter == m_methods.end())
	{
		return nullptr;
	}

	return iter->second;
}

const int LoxClass::GetArity()
{
	std::shared_ptr<LoxFunction> initializer = FindMethod(std::string("init"));
	if (initializer != nullptr)
	{
		return initializer->GetArity();
	}

	return 0;
}