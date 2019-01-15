#pragma once
#include "Expr.h"
#include <vector>

class AstPrinter : public Visitor<std::string>
{
public:
	AstPrinter();
	~AstPrinter();

	std::string Print(Expr expr);

	std::string visitBinaryExpr(Binary expr);

private:
	std::string parenthesize(std::string name, std::vector<Expr> exprs);
};

