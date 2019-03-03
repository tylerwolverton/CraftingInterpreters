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


std::shared_ptr<Stmt> Parser::statement()
{
	if (match(std::vector<ETokenType>{ IF })) return ifStatement();
	if (match(std::vector<ETokenType>{ PRINT })) return printStatement();
	if (match(std::vector<ETokenType>{ WHILE })) return whileStatement();
	if (match(std::vector<ETokenType>{ FOR })) return forStatement();
	if (match(std::vector<ETokenType>{ LEFT_BRACE })) return std::make_shared<BlockStmt>(block());

	return expressionStatement();
}

std::shared_ptr<Stmt> Parser::ifStatement()
{
	consume(LEFT_PAREN, "Expected '(' after 'if'.");
	std::shared_ptr<Expr> condition = expression();
	consume(RIGHT_PAREN, "Expected ')' after condition.");

	std::shared_ptr<Stmt> thenBranch = statement();
	std::shared_ptr<Stmt> elseBranch = nullptr;
	if (match(std::vector<ETokenType>{ ELSE }))
	{
		elseBranch = statement();
	}

	return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::printStatement()
{
	std::shared_ptr<Expr> value = expression();
	consume(SEMICOLON, "Expected ';' after value.");
	return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::whileStatement()
{
	consume(LEFT_PAREN, "Expected '(' after 'while'.");
	std::shared_ptr<Expr> condition = expression();
	consume(RIGHT_PAREN, "Expected ')' after condition.");

	std::shared_ptr<Stmt> body = statement();

	return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::forStatement()
{
	consume(LEFT_PAREN, "Expected '(' after 'for'.");


	// Initializer
	std::shared_ptr<Stmt> initializer = nullptr;
	if (match(std::vector<ETokenType>{ SEMICOLON }))
	{
		// No initializer, stay nullptr
	}
	else if (match(std::vector<ETokenType>{ VAR }))
	{
		initializer = varDeclaration();
	}
	else
	{
		initializer = expressionStatement();
	}

	// Condition
	std::shared_ptr<Expr> condition = nullptr;
	if (!check(SEMICOLON))
	{
		condition = expression();
	}
	else
	{
		condition = std::make_shared<LiteralExpr>(Token(TRUE, "true", 0));
	}
	consume(SEMICOLON, "Expected ';' after loop condition.");
	
	// Increment
	std::shared_ptr<Expr> increment = nullptr;
	if (!check(RIGHT_PAREN))
	{
		increment = expression();
	}	
	consume(RIGHT_PAREN, "Expected ')' after for clauses.");

	std::shared_ptr<Stmt> body = statement();

	// Build while loop from components
	std::vector<std::shared_ptr<Stmt>> blockBody;
	std::vector<std::shared_ptr<Stmt>> whileBody;

	if (initializer != nullptr)
	{
		blockBody.push_back(initializer);
	}

	if (condition == nullptr)
	{
		condition = std::make_shared<LiteralExpr>(Token(TRUE, "true", 0));
	}
	whileBody.push_back(body);

	if (increment != nullptr)
	{
		whileBody.push_back(std::make_shared<ExpressionStmt>(increment));
	}

	blockBody.push_back(std::make_shared<WhileStmt>(condition, std::make_shared<BlockStmt>(whileBody)));

	return std::make_shared<BlockStmt>(blockBody);
}

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
	std::vector<std::shared_ptr<Stmt>> statements;

	while (!check(RIGHT_BRACE) && !isAtEnd()) 
	{
		statements.push_back(declaration());
	}

	consume(RIGHT_BRACE, "Expect '}' after block.");
	return statements;
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

std::shared_ptr<Expr> Parser::expression()
{
	return assignment();
}

std::shared_ptr<Expr> Parser::assignment() 
{
	std::shared_ptr<Expr> expr = logical_or();

	if (match(std::vector<ETokenType>{ EQUAL }))
	{
		Token equals = previous();
		std::shared_ptr<Expr> value = assignment();

		if (std::dynamic_pointer_cast<VariableExpr>(expr) != nullptr) 
		{
			Token name = (std::static_pointer_cast<VariableExpr>(expr))->m_name;
			return std::make_shared<AssignExpr>(name, value);
		}

		ParseError(equals, "Invalid assignment target.");
	}

	return expr;
}

std::shared_ptr<Expr> Parser::logical_or()
{
	std::shared_ptr<Expr> expr = logical_and();

	while (match(std::vector<ETokenType>{ ETokenType::OR }))
	{
		Token op = previous();
		std::shared_ptr<Expr> right = logical_and();
		expr = std::make_shared<LogicalExpr>(expr, op, right);
	}

	return expr;
}

std::shared_ptr<Expr> Parser::logical_and()
{
	std::shared_ptr<Expr> expr = equality();

	while (match(std::vector<ETokenType>{ ETokenType::AND }))
	{
		Token op = previous();
		std::shared_ptr<Expr> right = equality();
		expr = std::make_shared<LogicalExpr>(expr, op, right);
	}

	return expr;
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