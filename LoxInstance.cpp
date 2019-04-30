#include "LoxInstance.h"
#include "LoxFunction.h"
#include "Error.h"

std::shared_ptr<void> LoxInstance::GetField(Token fieldName)
{
	auto iter = m_fields.find(fieldName.GetLexeme());
	if (iter != m_fields.end())
	{
		return iter->second;
	}

	std::shared_ptr<LoxFunction> method = m_klass.FindMethod(fieldName.GetLexeme());
	if (method != nullptr)
	{
		return method->Bind(std::make_shared<LoxInstance>(*this));
	}

	throw RuntimeError(fieldName, "Undefined property '" + fieldName.GetLexeme() + "'.");
}

void LoxInstance::SetField(Token fieldName, std::shared_ptr<void> value)
{
	m_fields[fieldName.GetLexeme()] = value;
}
