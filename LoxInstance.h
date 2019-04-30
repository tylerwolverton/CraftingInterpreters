#pragma once
#include "LoxClass.h"
#include "Types.h"

#include <string>
#include <iostream>
#include <map>

class LoxInstance
{
public:
	LoxInstance(LoxClass klass)
		: m_klass(klass)
	{
		//std::cout << klass.ToString();
	}
	virtual ~LoxInstance() {}

	std::string ToString()
	{
		return m_klass.ToString() + " instance";
	}

	std::shared_ptr<void> GetField(Token fieldName);
	void SetField(Token fieldName, std::shared_ptr<void> value);

private:
	LoxClass m_klass;
	std::map<std::string, std::shared_ptr<void>> m_fields;
};

