#include "AstPrinter.h"
#include <sstream>

AstPrinter::AstPrinter()
{
}

AstPrinter::~AstPrinter()
{
}

std::string AstPrinter::Print(Expr expr)
{
	return expr.accept(this);
}

std::string AstPrinter::visitBinaryExpr(Binary expr)
{
	return parenthesize(expr.m_op.GetLexeme(), expr.m_left, expr.m_right);
}

std::string AstPrinter::parenthesize(std::string name, std::vector<Expr> exprs) 
{
	std::stringstream buffer;

	buffer << "(" << name;
	for (Expr expr : exprs) {
		buffer << " ";
		buffer << expr.accept(this);
	}
	buffer << ")";

	return buffer.str();
}