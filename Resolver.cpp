#include "Resolver.h"
#include "Interpreter.h"
#include "Error.h"

Resolver::Resolver(const std::shared_ptr<Interpreter>& interpreter)
	: m_interpreter(interpreter),
	  m_scopeStack(std::vector<std::map<std::string, bool>>()),
	  m_curFunctionType(EFunctionType::NONE_FUNC),
	  m_curClassType(EClassType::NONE_CLASS)
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

std::shared_ptr<void> Resolver::visitGetExpr(const std::shared_ptr<GetExpr>& expr)
{
	resolve(expr->m_obj);

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

std::shared_ptr<void> Resolver::visitSetExpr(const std::shared_ptr<SetExpr>& expr)
{
	resolve(expr->m_value);
	resolve(expr->m_obj);

	return nullptr;
}

std::shared_ptr<void> Resolver::visitSuperExpr(const std::shared_ptr<SuperExpr>& expr)
{
	if (m_curClassType == EClassType::NONE_CLASS)
	{
		throw ResolverError(expr->m_keyword, "Cannot use 'super' outside of a class.");
	}
	else if (m_curClassType == EClassType::CLASS)
	{
		throw ResolverError(expr->m_keyword, "Cannot use 'super' in a class with no superclass.");
	}

	resolveLocal(expr, expr->m_keyword);
	return nullptr;
}

std::shared_ptr<void> Resolver::visitThisExpr(const std::shared_ptr<ThisExpr>& expr)
{
	if (m_curClassType == EClassType::NONE_CLASS)
	{
		throw RuntimeError(expr->m_keyword, "Cannot use 'this' outside of a class.");
		return nullptr;
	}

	resolveLocal(expr, expr->m_keyword);

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

void Resolver::visitClassStmt(const std::shared_ptr<ClassStmt>& stmt)
{
	EClassType enclosingClass = m_curClassType;
	m_curClassType = EClassType::CLASS;
	declare(stmt->m_name);
	define(stmt->m_name);

	if (stmt->m_superclass != nullptr
		&& stmt->m_name.GetLexeme() == stmt->m_superclass->m_name.GetLexeme())
	{
		throw ResolverError(stmt->m_superclass->m_name, "A class cannot inherit from itself.");
	}

	if (stmt->m_superclass != nullptr)
	{
		m_curClassType = EClassType::SUBCLASS;
		resolve(stmt->m_superclass);
		
		beginScope();
		m_scopeStack.back().insert(std::make_pair<std::string, bool>("super", true));
	}

	beginScope();
	m_scopeStack.back().insert(std::make_pair<std::string, bool>("this", true));

	for (const auto& method : stmt->m_methods) 
	{
		EFunctionType declaration = EFunctionType::METHOD;
		if (method->m_name.GetLexeme() == std::string("init"))
		{
			declaration = EFunctionType::INITIALIZER;
		}
		resolveFunction(method, declaration);
	}

	endScope();

	if (stmt->m_superclass != nullptr)
	{
		endScope();
	}

	m_curClassType = enclosingClass;
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
	if (m_curFunctionType == EFunctionType::NONE_FUNC)
	{
		throw ResolverError(stmt->m_keyword, "Cannot return outside function.");
	}

	if (stmt->m_value != nullptr)
	{
		if (m_curFunctionType == EFunctionType::INITIALIZER) 
		{
			throw ResolverError(stmt->m_keyword, "Cannot return a value from an initializer.");
		}

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
			//m_interpreter->Resolve(expr, m_scopeStack.size() - 1 - i);
			m_interpreter->Resolve(name, m_scopeStack.size() - 1 - i);
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