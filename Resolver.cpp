#include "Resolver.h"
#include "Error.h"

Resolver::Resolver(const Interpreter& interpreter)
	: m_interpreter(interpreter),
	  m_scopeStack(std::vector<std::map<std::string, bool>>()),
	  m_curFunctionType(EFunctionType::NONE)
{
}

Resolver::~Resolver()
{
}

void Resolver::Resolve(const std::vector<std::shared_ptr<Stmt>>& statements)
{
	for (const auto& stmt : statements)
	{
		resolve(stmt);
	}
}

std::shared_ptr<void> Resolver::visitAssignExpr(const std::shared_ptr<AssignExpr>& expr)
{
	resolve(expr->m_value);
	resolveLocal(expr, expr->m_name);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr)
{
	resolve(expr->m_left);
	resolve(expr->m_right);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr)
{
	resolve(expr->m_expr);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr)
{
	return nullptr;
}

std::shared_ptr<void> Resolver::visitLogicalExpr(const std::shared_ptr<LogicalExpr>& expr)
{
	resolve(expr->m_left);
	resolve(expr->m_right);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr)
{
	resolve(expr->m_right);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitCallExpr(const std::shared_ptr<CallExpr>& expr)
{
	resolve(expr->m_callee);

	for (const auto& arg : expr->m_arguments)
	{
		resolve(arg);
	}

	return nullptr;
}

std::shared_ptr<void> Resolver::visitVariableExpr(const std::shared_ptr<VariableExpr>& expr)
{
	if (!m_scopeStack.empty())
	{
		auto iter = m_scopeStack[m_scopeStack.size() - 1].find(expr->m_name.GetLexeme());
		if (iter != m_scopeStack[m_scopeStack.size() - 1].end()
			&& iter->second == false)
		{
			throw ResolverError(expr->m_name, "Cannot read local variable in its own initializer.");
		}
	}

	resolveLocal(expr, expr->m_name);

	return nullptr;
}

void Resolver::visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt)
{
	beginScope();
	Resolve(stmt->m_statements);
	endScope();
}

void Resolver::beginScope()
{
	m_scopeStack.push_back(std::map<std::string, bool>());
}

void Resolver::endScope()
{
	m_scopeStack.pop_back();
}

void Resolver::visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt)
{
	resolve(stmt->m_expr);
}

void Resolver::visitIfStmt(const std::shared_ptr<IfStmt>& stmt)
{
	resolve(stmt->m_condition);
	resolve(stmt->m_thenBranch);
	if (stmt->m_elseBranch != nullptr) resolve(stmt->m_elseBranch);
}

void Resolver::visitFunctionStmt(const std::shared_ptr<FunctionStmt>& stmt)
{
	declare(stmt->m_name);
	define(stmt->m_name);

	resolveFunction(stmt, EFunctionType::FUNCTION);
}

void Resolver::visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt)
{
	resolve(stmt->m_expr);
}

void Resolver::visitReturnStmt(const std::shared_ptr<ReturnStmt>& stmt)
{
	if (m_curFunctionType == EFunctionType::NONE)
	{
		throw ResolverError(stmt->m_keyword, "Cannot return outside function.");
	}

	if (stmt->m_value != nullptr)
	{
		resolve(stmt->m_value);
	}
}

void Resolver::visitVarStmt(const std::shared_ptr<VarStmt>& stmt)
{
	declare(stmt->m_name);
	if (stmt->m_initializer != nullptr)
	{
		resolve(stmt->m_initializer);
	}
	define(stmt->m_name);
}

void Resolver::declare(Token name)
{
	if (m_scopeStack.size() == 0) return;
	if (m_scopeStack.back().find(name.GetLexeme()) != m_scopeStack.back().end())
	{
		throw ResolverError(name, "Variable with this name already declared in this scope.");
	}

	(m_scopeStack.back())[name.GetLexeme()] = false;
}

void Resolver::define(Token name)
{
	if (m_scopeStack.size() == 0) return;

	(m_scopeStack.back())[name.GetLexeme()] = true;
}

void Resolver::visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt)
{
	resolve(stmt->m_condition);
	resolve(stmt->m_body);
}

void Resolver::resolve(const std::shared_ptr<Stmt>& stmt)
{
	stmt->accept(*this);
}

void Resolver::resolve(const std::shared_ptr<Expr>& expr)
{
	expr->accept(*this);
}

void Resolver::resolveLocal(const std::shared_ptr<Expr>& expr, Token name)
{
	for (int i = m_scopeStack.size() - 1; i >= 0; i--) 
	{
		auto iter = m_scopeStack[i].find(name.GetLexeme());
		if (iter != m_scopeStack[i].end())
		{
			m_interpreter.Resolve(expr, m_scopeStack.size() - 1 - i);
			return;
		}
	}
}

void Resolver::resolveFunction(const std::shared_ptr<FunctionStmt>& function, Resolver::EFunctionType type)
{
	EFunctionType enclosingFunctionType = m_curFunctionType;
	m_curFunctionType = type;

	beginScope();
	for (const auto& param : function->m_params)
	{
		declare(*param);
		define(*param);
	}

	resolve(function->m_body);
	endScope();

	m_curFunctionType = enclosingFunctionType;
}