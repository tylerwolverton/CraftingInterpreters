#include "Parser.h"
#include "Error.h"

Parser::Parser(const std::shared_ptr<std::vector<Token>>& tokens)
	: m_tokens(tokens)
{
}

Parser::~Parser()
{
}

std::shared_ptr<Expr> Parser::Parse()
{
	try
	{
		return expression();
	}
	catch (ParserError error)
	{
		return nullptr;
	}
}

std::shared_ptr<Expr> Parser::expression()
{
	return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
	std::shared_ptr<Expr> expr = comparison();

	while (match(std::vector<ETokenType>{ ETokenType::BANG_EQUAL, ETokenType::EQUAL_EQUAL }))
	{
		expr = std::make_shared<BinaryExpr>(expr, previous(), comparison());
	}

	return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
	std::shared_ptr<Expr> expr = addition();

	while (match(std::vector<ETokenType>{ ETokenType::GREATER, ETokenType::GREATER_EQUAL, ETokenType::LESS, ETokenType::LESS_EQUAL }))
	{
		expr = std::make_shared<BinaryExpr>(expr, previous(), addition());
	}

	return expr;
}

std::shared_ptr<Expr> Parser::addition()
{
	std::shared_ptr<Expr> expr = multiplication();

	while (match(std::vector<ETokenType>{ ETokenType::PLUS, ETokenType::MINUS }))
	{
		expr = std::make_shared<BinaryExpr>(expr, previous(), multiplication());
	}

	return expr;
}

std::shared_ptr<Expr> Parser::multiplication()
{
	std::shared_ptr<Expr> expr = unary();

	while (match(std::vector<ETokenType>{ ETokenType::SLASH, ETokenType::STAR }))
	{
		expr = std::make_shared<BinaryExpr>(expr, previous(), unary());
	}

	return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
	if (match(std::vector<ETokenType>{ ETokenType::BANG, ETokenType::MINUS }))
	{
		return std::make_shared<UnaryExpr>(previous(), unary());
	}

	return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
	if (match(std::vector<ETokenType>{ ETokenType::FALSE , ETokenType::TRUE, ETokenType::NIL, ETokenType::NUMBER, ETokenType::STRING}))
	{
		return std::make_shared<LiteralExpr>(previous());
	}

	if (match(std::vector<ETokenType>{ ETokenType::LEFT_PAREN }))
	{
		std::shared_ptr<Expr> expr = expression();
		consume(ETokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return std::make_shared<GroupingExpr>(expr);
	}

	throw ParserError(peek(), "Expected expression.");
}

bool Parser::match(std::vector<ETokenType> types)
{
	for (const auto& type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}

	return false;
}

bool Parser::check(const ETokenType& type)
{
	if (!isAtEnd()
		&& m_tokens->at(m_curIdx).GetType() == type)
	{
		return true;
	}

	return false;
}

Token Parser::advance()
{
	if (!isAtEnd())
	{
		m_curIdx++;
	}

	return previous();
}

Token Parser::previous()
{
	return m_tokens->at(m_curIdx - 1);
}

Token Parser::peek()
{
	return m_tokens->at(m_curIdx);
}

Token Parser::consume(const ETokenType& type, const std::string& msg)
{
	if (check(type))
	{
		return advance();
	}

	throw ParserError(peek(), msg);
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous().GetType() == ETokenType::SEMICOLON) return;

		switch (peek().GetType()) 
		{
			case ETokenType::CLASS:
			case ETokenType::FUN:
			case ETokenType::VAR:
			case ETokenType::FOR:
			case ETokenType::IF:
			case ETokenType::WHILE:
			case ETokenType::PRINT:
			case ETokenType::RETURN:
				return;
		}

		advance();
	}
}

bool Parser::isAtEnd()
{
	return peek().GetType() == ETokenType::END_OF_FILE;
}