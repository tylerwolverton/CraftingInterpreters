#include "Interpreter.h"

#include <iostream>

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::Interpret(const std::shared_ptr<Expr>& expr)
{
	std::shared_ptr<Token> value = std::static_pointer_cast<Token>(evaluate(expr));

	switch (value->GetType())
	{
		case ETokenType::NUMBER:
			std::cout << value->GetLexemeAsDouble() << "\n";
			break;
		default:
			std::cout << value->GetLexeme() << "\n";
			break;
	}
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
				return createTruthToken(left->GetLexemeAsDouble() > right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::GREATER_EQUAL:
				return createTruthToken(left->GetLexemeAsDouble() >= right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::LESS:
				return createTruthToken(left->GetLexemeAsDouble() < right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::LESS_EQUAL:
				return createTruthToken(left->GetLexemeAsDouble() <= right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::MINUS:
				return createNumberToken(left->GetLexemeAsDouble() - right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::SLASH:
				return createNumberToken(left->GetLexemeAsDouble() / right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::STAR:
				return createNumberToken(left->GetLexemeAsDouble() * right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::PLUS:
				return createNumberToken(left->GetLexemeAsDouble() + right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
		}
	}

	if (left->GetType() == ETokenType::STRING
		&& right->GetType() == ETokenType::STRING
		&& expr->m_op.GetType() == ETokenType::PLUS)
	{
		return createStringToken(left->GetLexeme() + right->GetLexeme(), expr->m_op.GetLineNum());
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
			return createTruthToken(!isTruthy(right), right->GetLineNum());
		case ETokenType::MINUS:
			if (right->GetType() == ETokenType::NUMBER)
			{
				return createNumberToken(-(right->GetLexemeAsDouble()), right->GetLineNum());
			}
			else
			{
				// Throw type error
			}
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

std::shared_ptr<Token> Interpreter::createTruthToken(bool value, int lineNum)
{
	ETokenType type = value ? ETokenType::TRUE : ETokenType::FALSE;
	std::string lexeme = value ? "true" : "false";

	return std::make_shared<Token>(type, lexeme, lineNum);
}

std::shared_ptr<Token> Interpreter::createNumberToken(double value, int lineNum)
{
	return std::make_shared<Token>(ETokenType::NUMBER, std::to_string(value), lineNum);
}

std::shared_ptr<Token> Interpreter::createStringToken(std::string value, int lineNum)
{
	return std::make_shared<Token>(ETokenType::STRING, value, lineNum);
}