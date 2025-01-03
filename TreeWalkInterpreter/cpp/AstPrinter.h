#pragma once
#include "Expr.h"
#include <vector>
#include <memory>

class AstPrinter //: public ExprVisitor
{
public:
	AstPrinter();
	~AstPrinter();

	//std::string Print(const std::shared_ptr<Expr>& expr);

	/*std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) override;
	std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) override;
	std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) override;
	std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) override;
	std::shared_ptr<void> visitVariableExpr(const std::shared_ptr<VariableExpr>& expr) override;*/

private:
	//std::shared_ptr<void> parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs);
};

