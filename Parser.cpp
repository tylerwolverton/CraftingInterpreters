#include "Parser.h"
#include "Error.h"

Parser::Parser(const std::shared_ptr<std::vector<Token>>& tokens)
	: m_tokens(tokens)
{
}

Parser::~Parser()
{
}

std::vector<std::shared_ptr<Stmt>> Parser::Parse()
{
	std::vector<std::shared_ptr<Stmt>> statements;
	while (!isAtEnd())
	{
		statements.push_back(declaration());
	}
	
	return statements;
}

std::shared_ptr<Expr> Parser::expression()
{
	return equality();
}

std::shared_ptr<Stmt> Parser::statement()
{
	if (match(std::vector<ETokenType>{ ETokenType::PRINT })) return printStatement();

	return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement()
{
	std::shared_ptr<Expr> value = expression();
	consume(SEMICOLON, "Expected ';' after value.");
	return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::expressionStatement()
{
	std::shared_ptr<Expr> expr = expression();
	consume(SEMICOLON, "Expected ';' after expression.");
	return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::declaration() 
{
	try 
	{
		if (match(std::vector<ETokenType>{ VAR }))
		{
			return varDeclaration();
		}

		return statement();
	}
	catch (ParseError error) 
	{
		synchronize();
		return nullptr;
	}
}

std::shared_ptr<Stmt> Parser::varDeclaration() 
{
	Token name = consume(IDENTIFIER, "Expect variable name.");

	std::shared_ptr<Expr> initializer = nullptr;
	if (match(std::vector<ETokenType>{ EQUAL })) 
	{
		initializer = expression();
	}

	consume(SEMICOLON, "Expect ';' after variable declaration.");
	return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Expr> Parser::equality()
{
	std::shared_ptr<Expr> expr = comparison();

	while (match(std::vector<ETokenType>{ ETokenType::BANG_EQUAL, ETokenType::EQUAL_EQUAL }))
	{
		Token token = previous();
		std::shared_ptr<Expr> right = comparison();
		expr = std::make_shared<BinaryExpr>(expr, token, right);
	}

	return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
	std::shared_ptr<Expr> expr = addition();

	while (match(std::vector<ETokenType>{ ETokenType::GREATER, ETokenType::GREATER_EQUAL, ETokenType::LESS, ETokenType::LESS_EQUAL }))
	{
		Token token = previous();
		std::shared_ptr<Expr> right = addition();
		expr = std::make_shared<BinaryExpr>(expr, token, right);
	}

	return expr;
}

std::shared_ptr<Expr> Parser::addition()
{
	std::shared_ptr<Expr> expr = multiplication();

	while (match(std::vector<ETokenType>{ ETokenType::PLUS, ETokenType::MINUS }))
	{
		Token token = previous();
		std::shared_ptr<Expr> right = multiplication();
		expr = std::make_shared<BinaryExpr>(expr, token, right);
	}

	return expr;
}

std::shared_ptr<Expr> Parser::multiplication()
{
	std::shared_ptr<Expr> expr = unary();

	while (match(std::vector<ETokenType>{ ETokenType::SLASH, ETokenType::STAR }))
	{
		Token token = previous();
		std::shared_ptr<Expr> right = unary();
		expr = std::make_shared<BinaryExpr>(expr, token, right);
	}

	return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
	if (match(std::vector<ETokenType>{ ETokenType::BANG, ETokenType::MINUS }))
	{
		Token token = previous();
		std::shared_ptr<Expr> right = unary();
		return std::make_shared<UnaryExpr>(token, right);
	}

	return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
	if (match(std::vector<ETokenType>{ ETokenType::FALSE , ETokenType::TRUE, ETokenType::NIL, ETokenType::NUMBER, ETokenType::STRING}))
	{
		return std::make_shared<LiteralExpr>(previous());
	}

	if (match(std::vector<ETokenType> {IDENTIFIER}))
	{
		return std::make_shared<VariableExpr>(previous());
	}

	if (match(std::vector<ETokenType>{ ETokenType::LEFT_PAREN }))
	{
		std::shared_ptr<Expr> expr = expression();
		consume(ETokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return std::make_shared<GroupingExpr>(expr);
	}

	throw ParseError(peek(), "Expected expression.");
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

	throw ParseError(peek(), msg);
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