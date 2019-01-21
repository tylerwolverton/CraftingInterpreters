#pragma once
#include "Expr.h"
#include <vector>
#include <memory>

class AstPrinter : public Visitor
{
public:
	AstPrinter();
	~AstPrinter();

	std::string Print(const std::shared_ptr<Expr>& expr);

	void visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr, const std::shared_ptr<std::string>& returnStr) override;
	void visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr, const std::shared_ptr<std::string>& returnStr) override;
	void visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr, const std::shared_ptr<std::string>& returnStr) override;
	void visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr, const std::shared_ptr<std::string>& returnStr) override;

private:
	std::string parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs);
};

