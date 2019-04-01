#pragma once
#include "Types.h"

#include <vector>
#include <memory>
#include <time.h>

class Interpreter;

class LoxCallable
{
public:
	virtual ~LoxCallable() {};
	virtual std::shared_ptr<void> Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args) = 0;

	virtual const int GetArity() const = 0;

private:
	int m_arity;
};

//class LoxTime : public LoxCallable
//{
//public:
//	LoxTime() {}
//
//	std::shared_ptr<void> Call(const std::shared_ptr<Interpreter>& interpreter, std::vector<std::shared_ptr<Token>> args) override
//	{
//		return std::make_shared<Token>(ETokenType::NUMBER, time(NULL), args.front()->GetLineNum());
//	}
//
//	const int GetArity() const override { return 0; }
//};
