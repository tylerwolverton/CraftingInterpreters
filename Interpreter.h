#pragma once
#include "Expr.h"
class Interpreter : public Visitor
{
public:
	Interpreter();
	~Interpreter();

	void Interpret(const std::shared_ptr<Expr>& expr);

	std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) override;
	std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) override;
	std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) override;
	std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) override;

private:
	std::shared_ptr<void> evaluate(std::shared_ptr<Expr> expr);
	bool isTruthy(std::shared_ptr<Token> token);
	bool isEqual(std::shared_ptr<Token> left, std::shared_ptr<Token> right);

	std::shared_ptr<Token> createTruthToken(bool value, int lineNum);
	std::shared_ptr<Token> createNumberToken(double value, int lineNum);
	std::shared_ptr<Token> createStringToken(std::string value, int lineNum);
};

