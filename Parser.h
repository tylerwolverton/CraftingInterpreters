#pragma once
#include "Types.h"
#include "Expr.h"
#include "Stmt.h"
#include <vector>
#include <memory>

class Parser
{
public:
	// TODO: Remove the local vector and create the return on the fly
	Parser(const std::shared_ptr<std::vector<Token>>& tokens);
	~Parser();

	std::vector<std::shared_ptr<Stmt>> Parse();

private:
	std::shared_ptr<std::vector<Token>> m_tokens;
	int m_curIdx = 0;

	std::shared_ptr<Expr> expression();
	std::shared_ptr<Expr> equality();
	std::shared_ptr<Expr> comparison();
	std::shared_ptr<Expr> addition();
	std::shared_ptr<Expr> multiplication();
	std::shared_ptr<Expr> unary();
	std::shared_ptr<Expr> primary();

	bool match(std::vector<ETokenType> types);
	bool check(const ETokenType& type);
	Token advance();
	Token peek();
	Token previous();
	Token consume(const ETokenType& type, const std::string& msg);
	void synchronize();
	bool isAtEnd();

	std::shared_ptr<Stmt> statement();
	std::shared_ptr<Stmt> printStatement();
	std::shared_ptr<Stmt> expressionStatement();
};

