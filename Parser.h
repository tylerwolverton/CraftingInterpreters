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
	std::shared_ptr<Stmt> statement();
	std::shared_ptr<Stmt> ifStatement();
	std::shared_ptr<Stmt> printStatement();
	std::shared_ptr<Stmt> whileStatement();
	std::shared_ptr<Stmt> forStatement();
	std::vector<std::shared_ptr<Stmt>> block();
	std::shared_ptr<Stmt> expressionStatement();
	
	std::shared_ptr<Stmt> declaration();
	std::shared_ptr<Stmt> varDeclaration();

	std::shared_ptr<Expr> assignment();
	std::shared_ptr<Expr> logical_or();
	std::shared_ptr<Expr> logical_and();
	std::shared_ptr<Expr> equality();
	std::shared_ptr<Expr> comparison();
	std::shared_ptr<Expr> addition();
	std::shared_ptr<Expr> multiplication();
	std::shared_ptr<Expr> unary();
	std::shared_ptr<Expr> call();
	std::shared_ptr<Expr> primary();

	std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
	bool match(std::vector<ETokenType> types);
	bool check(const ETokenType& type);
	Token advance();
	Token peek();
	Token previous();
	Token consume(const ETokenType& type, const std::string& msg);
	void synchronize();
	bool isAtEnd();
};

