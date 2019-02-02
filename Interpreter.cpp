#include "Interpreter.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::Interpret(const std::shared_ptr<Expr>& expr)
{
	auto value = evaluate(expr);
}

std::shared_ptr<void> Interpreter::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr)
{
	std::shared_ptr<Token> left = std::static_pointer_cast<Token>(evaluate(expr->m_left));
	std::shared_ptr<Token> right = std::static_pointer_cast<Token>(evaluate(expr->m_right));

	switch (expr->m_op.GetType())
	{
		case ETokenType::BANG_EQUAL:
				return std::make_shared<bool>(!isEqual(left, right));
		case ETokenType::EQUAL_EQUAL:
			return std::make_shared<bool>(isEqual(left, right));
	}

	if (left->GetType() == ETokenType::NUMBER
		&& right->GetType() == ETokenType::NUMBER)
	{

		switch (expr->m_op.GetType())
		{
			case ETokenType::GREATER:
				return std::make_shared<double>(left->GetLexemeAsDouble() > right->GetLexemeAsDouble());
			case ETokenType::GREATER_EQUAL:
				return std::make_shared<double>(left->GetLexemeAsDouble() >= right->GetLexemeAsDouble());
			case ETokenType::LESS:
				return std::make_shared<double>(left->GetLexemeAsDouble() < right->GetLexemeAsDouble());
			case ETokenType::LESS_EQUAL:
				return std::make_shared<double>(left->GetLexemeAsDouble() <= right->GetLexemeAsDouble());
			case ETokenType::MINUS:
				return std::make_shared<double>(left->GetLexemeAsDouble() - right->GetLexemeAsDouble());
			case ETokenType::SLASH:
				return std::make_shared<double>(left->GetLexemeAsDouble() / right->GetLexemeAsDouble());
			case ETokenType::STAR:
				return std::make_shared<double>(left->GetLexemeAsDouble() * right->GetLexemeAsDouble());
			case ETokenType::PLUS:
				return std::make_shared<double>(left->GetLexemeAsDouble() + right->GetLexemeAsDouble());
		}
	}

	if (left->GetType() == ETokenType::STRING
		&& right->GetType() == ETokenType::STRING
		&& expr->m_op.GetType() == ETokenType::PLUS)
	{
		return std::make_shared<std::string>(left->GetLexemeAsString() + right->GetLexemeAsString());
	}

	// Unreachable
	return nullptr;
}

std::shared_ptr<void> Interpreter::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr)
{
	return evaluate(expr->m_expr);
}

std::shared_ptr<void> Interpreter::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr)
{
	return std::make_shared<Token>(expr->m_literal);
	//ETokenType type = expr->m_literal.GetType();
	//if (type == ETokenType::STRING)
	//{
	//	return std::make_shared<std::string>(expr->m_literal.GetLexeme());
	//}
	//else if (type == ETokenType::NUMBER)
	//{
	//	return std::make_shared<double>(atof(expr->m_literal.GetLexeme().c_str()));
	//}
	//else if (type == ETokenType::TRUE)
	//{
	//	return std::make_shared<bool>(true);
	//}
	//else if (type == ETokenType::FALSE)
	//{
	//	return std::make_shared<bool>(false);
	//}
	//
	//return nullptr;
}

std::shared_ptr<void> Interpreter::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr)
{
	std::shared_ptr<Token> right = std::static_pointer_cast<Token>(evaluate(expr->m_right));

	switch (expr->m_op.GetType())
	{
		case ETokenType::BANG:
			return std::make_shared<bool>(!isTruthy(right));
		case ETokenType::MINUS:
			return std::make_shared<double>(-(right->GetLexemeAsDouble()));
	}

	// Unreachable
	return nullptr;
}

std::shared_ptr<void> Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
	return expr->accept(*this);
}

bool Interpreter::isTruthy(std::shared_ptr<Token> token)
{
	switch (token->GetType())
	{
		case ETokenType::STRING:
		case ETokenType::NUMBER:
		case ETokenType::TRUE:
			return true;
	}

	return false;
}

bool Interpreter::isEqual(std::shared_ptr<Token> left, std::shared_ptr<Token> right)
{
	return false;
}