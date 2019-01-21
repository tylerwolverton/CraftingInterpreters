#include "AstPrinter.h"
#include <sstream>
#include <iostream>

AstPrinter::AstPrinter()
{
}

AstPrinter::~AstPrinter()
{
}

std::string AstPrinter::Print(const std::shared_ptr<Expr>& expr)
{
	std::shared_ptr<std::string> returnStr = std::make_shared<std::string>("");
	expr->accept(*this, returnStr);
	return *returnStr;
}

void AstPrinter::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr, const std::shared_ptr<std::string>& returnStr)
{
	*returnStr = parenthesize(expr->m_op.GetLexeme(), std::vector<std::shared_ptr<Expr>>{expr->m_left, expr->m_right});
}

void AstPrinter::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr, const std::shared_ptr<std::string>& returnStr)
{
	*returnStr = parenthesize("group", std::vector<std::shared_ptr<Expr>>{expr->m_expr});
}

void AstPrinter::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr, const std::shared_ptr<std::string>& returnStr)
{
	if (expr != nullptr)
	{
		*returnStr = expr->m_literal.GetLexeme();
	}
}

void AstPrinter::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr, const std::shared_ptr<std::string>& returnStr)
{
	*returnStr = parenthesize(expr->m_op.GetLexeme(), std::vector<std::shared_ptr<Expr>>{expr->m_right});
}

std::string AstPrinter::parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs)
{
	std::stringstream buffer;

	buffer << "(" << name;
	for (auto& expr : exprs) 
	{
		buffer << " ";
		std::shared_ptr<std::string> returnStr = std::make_shared<std::string>("");
		expr->accept(*this, returnStr);
		buffer << *returnStr;
	}
	buffer << ")";
	
	return buffer.str();
}