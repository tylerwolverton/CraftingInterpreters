#include "Parser.h"


Parser::Parser(const std::shared_ptr<std::vector<Token>>& tokens)
	: m_tokens(tokens)
{
}

Parser::~Parser()
{
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
	return equality();

}
//
//std::shared_ptr<Expr> Parser::addition()
//{
//
//}
//
//std::shared_ptr<Expr> Parser::multiplication()
//{
//
//}
//
//std::shared_ptr<Expr> Parser::unary()
//{
//
//}
//
//std::shared_ptr<Expr> Parser::primary()
//{
//
//}

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

bool Parser::isAtEnd()
{
	return peek().GetType() == ETokenType::END_OF_FILE;
}