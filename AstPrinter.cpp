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
	return *(std::static_pointer_cast<std::string>(expr->accept(*this)));
}

std::shared_ptr<void> AstPrinter::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr)
{
	return parenthesize(expr->m_op.GetLexeme(), std::vector<std::shared_ptr<Expr>>{expr->m_left, expr->m_right});
}

std::shared_ptr<void> AstPrinter::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr)
{
	return parenthesize("group", std::vector<std::shared_ptr<Expr>>{expr->m_expr});
}

std::shared_ptr<void> AstPrinter::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr)
{
	if (expr != nullptr)
	{
		return std::make_shared<std::string>(expr->m_literal.GetLexeme());
	}

	return nullptr;
}

std::shared_ptr<void> AstPrinter::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr)
{
	return parenthesize(expr->m_op.GetLexeme(), std::vector<std::shared_ptr<Expr>>{expr->m_right});
}

std::shared_ptr<void> AstPrinter::parenthesize(std::string name, std::vector<std::shared_ptr<Expr>> exprs)
{
	std::stringstream buffer;

	buffer << "(" << name;
	for (auto& expr : exprs) 
	{
		buffer << " ";
		buffer << *(std::static_pointer_cast<std::string>(expr->accept(*this)));
	}
	buffer << ")";
	
	return std::make_shared<std::string>(buffer.str());
}