#pragma once
#include "Expr.h"
#include "Stmt.h"
#include <vector>
class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
	Interpreter();
	~Interpreter();

	void Interpret(const std::vector<std::shared_ptr<Stmt>>& statements);

	std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) override;
	std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) override;
	std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) override;
	std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) override;
	void visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) override;
	void visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt) override;

private:
	std::shared_ptr<void> evaluate(std::shared_ptr<Expr> expr);
	void execute(std::shared_ptr<Stmt> stmt);
	bool isTruthy(std::shared_ptr<Token> token);
	bool isEqual(std::shared_ptr<Token> left, std::shared_ptr<Token> right);

	std::shared_ptr<Token> createTruthToken(bool value, int lineNum);
	std::shared_ptr<Token> createNumberToken(double value, int lineNum);
	std::shared_ptr<Token> createStringToken(std::string value, int lineNum);

	std::string stringify(std::shared_ptr<Token> value);
};

