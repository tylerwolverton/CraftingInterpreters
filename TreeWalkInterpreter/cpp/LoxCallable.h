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

	virtual const int GetArity() = 0;

private:
	int m_arity;
};