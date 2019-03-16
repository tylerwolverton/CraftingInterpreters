#pragma once
#include "Types.h"

#include <vector>
#include <memory>
#include <time.h>

class LoxCallable
{
public:
	LoxCallable(int arity)
		: m_arity(arity)
	{}

	virtual std::shared_ptr<void> Call(Interpreter interpreter, std::vector<std::shared_ptr<Token>> args) = 0;

	int GetArity() { return m_arity; }

private:
	int m_arity;
};

class LoxTime : public LoxCallable
{
public:
	LoxTime() : LoxCallable(0)
	{}

	std::shared_ptr<void> Call(Interpreter interpreter, std::vector<std::shared_ptr<Token>> args) override
	{
		return std::make_shared<Token>(ETokenType::NUMBER, time(NULL), args.front()->GetLineNum());
	}
};
