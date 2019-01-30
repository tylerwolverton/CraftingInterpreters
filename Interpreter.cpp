#include "Interpreter.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::Interpret(const std::shared_ptr<Expr>& expr)
{

}

std::shared_ptr<void> Interpreter::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr)
{

}

std::shared_ptr<void> Interpreter::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr)
{
	return evaluate(expr->m_expr);
}

std::shared_ptr<void> Interpreter::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr)
{
	ETokenType type = expr->m_literal.GetType();
	if (type == ETokenType::STRING)
	{
		return std::make_shared<std::string>(expr->m_literal.GetLexeme());
	}
	else if (type == ETokenType::NUMBER)
	{
		return std::make_shared<double>(atof(expr->m_literal.GetLexeme().c_str()));
	}
	else if (type == ETokenType::TRUE)
	{
		return std::make_shared<bool>(true);
	}
	else if (type == ETokenType::FALSE)
	{
		return std::make_shared<bool>(false);
	}
	
	return nullptr;
}

std::shared_ptr<void> Interpreter::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr)
{

}

std::shared_ptr<void> Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
	return expr->accept(*this);
}