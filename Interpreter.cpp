#include "Interpreter.h"
#include "Error.h"
#include "LoxCallable.h"
#include "LoxClass.h"
#include "LoxInstance.h"
#include "LoxFunction.h"

#include <iostream>

Interpreter::Interpreter()
{
	m_globalEnv = m_environment = std::make_shared<Environment>();

//	m_globalEnv->Define("clock", std::make_shared<Token>(CLASS, LoxTime(), 0));
}

Interpreter::~Interpreter()
{
}

void Interpreter::Interpret(const std::vector<std::shared_ptr<Stmt>>& statements)
{
	for (auto stmt : statements)
	{
		execute(stmt);
	}
}

std::shared_ptr<void> Interpreter::visitAssignExpr(const std::shared_ptr<AssignExpr>& expr)
{
	std::shared_ptr<Token> value = std::static_pointer_cast<Token>(evaluate(expr->m_value));

	auto iter = m_locals.find(value->GetLexeme());
	//auto iter = m_locals.find(expr);
	if (iter != m_locals.end())
	{
		m_environment->AssignAt(iter->second, expr->m_name, value);
	}
	else
	{
	    m_globalEnv->Assign(expr->m_name, value);
	}

	return value;
}

std::shared_ptr<void> Interpreter::visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr)
{
	std::shared_ptr<Token> left = std::static_pointer_cast<Token>(evaluate(expr->m_left));
	std::shared_ptr<Token> right = std::static_pointer_cast<Token>(evaluate(expr->m_right));

	switch (expr->m_op.GetType())
	{
		case ETokenType::BANG_EQUAL:
			return createTruthToken(!isEqual(left, right), expr->m_op.GetLineNum());
		case ETokenType::EQUAL_EQUAL:
			return createTruthToken(isEqual(left, right), expr->m_op.GetLineNum());
	}

	if (left->GetType() == ETokenType::NUMBER
		&& right->GetType() == ETokenType::NUMBER)
	{
		switch (expr->m_op.GetType())
		{
			case ETokenType::GREATER:
				return createTruthToken(left->GetLexemeAsDouble() > right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::GREATER_EQUAL:
				return createTruthToken(left->GetLexemeAsDouble() >= right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::LESS:
				return createTruthToken(left->GetLexemeAsDouble() < right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::LESS_EQUAL:
				return createTruthToken(left->GetLexemeAsDouble() <= right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::MINUS:
				return createNumberToken(left->GetLexemeAsDouble() - right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::SLASH:
				return createNumberToken(left->GetLexemeAsDouble() / right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::STAR:
				return createNumberToken(left->GetLexemeAsDouble() * right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
			case ETokenType::PLUS:
				return createNumberToken(left->GetLexemeAsDouble() + right->GetLexemeAsDouble(), expr->m_op.GetLineNum());
		}
	}

	if (left->GetType() == ETokenType::STRING
		&& right->GetType() == ETokenType::STRING
		&& expr->m_op.GetType() == ETokenType::PLUS)
	{
		return createStringToken(left->GetLexeme() + right->GetLexeme(), expr->m_op.GetLineNum());
	}

	// Unreachable
	return nullptr;
}

std::shared_ptr<void> Interpreter::visitGetExpr(const std::shared_ptr<GetExpr>& expr)
{
	std::shared_ptr<LoxInstance> object = std::static_pointer_cast<LoxInstance>(evaluate(expr->m_obj));

	if (object == nullptr)
	{
		throw RuntimeError(expr->m_name, "Only instances have properties.");
	}

	return object->GetField(expr->m_name);
}

std::shared_ptr<void> Interpreter::visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr)
{
	return evaluate(expr->m_expr);
}

std::shared_ptr<void> Interpreter::visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr)
{
	return std::make_shared<Token>(expr->m_literal);
}

std::shared_ptr<void> Interpreter::visitLogicalExpr(const std::shared_ptr<LogicalExpr>& expr)
{
	std::shared_ptr<Token> left = std::static_pointer_cast<Token>(evaluate(expr->m_left));
	std::shared_ptr<Token> right = std::static_pointer_cast<Token>(evaluate(expr->m_right));

	switch (expr->m_op.GetType())
	{
		case ETokenType::OR:
			if (isTruthy(left))
			{
				return createTruthToken(true, left->GetLineNum());
			}
		case ETokenType::AND:
			if (!isTruthy(left))
			{
				return createTruthToken(false, left->GetLineNum());
			}
	}

	return std::static_pointer_cast<Token>(evaluate(expr->m_right));
}

std::shared_ptr<void> Interpreter::visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr)
{
	std::shared_ptr<Token> right = std::static_pointer_cast<Token>(evaluate(expr->m_right));

	switch (expr->m_op.GetType())
	{
		case ETokenType::BANG:
			return createTruthToken(!isTruthy(right), right->GetLineNum());
		case ETokenType::MINUS:
			if (right->GetType() == ETokenType::NUMBER)
			{
				return createNumberToken(-(right->GetLexemeAsDouble()), right->GetLineNum());
			}
			else
			{
				// Throw type error
			}
	}

	// Unreachable
	return nullptr;
}

std::shared_ptr<void> Interpreter::visitCallExpr(const std::shared_ptr<CallExpr>& expr)
{
	std::shared_ptr<LoxCallable> function = std::static_pointer_cast<LoxCallable>(evaluate(expr->m_callee));

	//if (!(callee->GetType() == FUN || callee->GetType() == CLASS))
	//{
	//	//std::cout << expr->m_callee << std::endl;
	//	//throw RuntimeError(expr->m_paren, "Can only call functions and classes.");
	//}
	
	// TODO: Supprt ananymous functions by checking for functions or tokens
	// Or maybe give anonmous functios a randomly generated name and then look them up

	std::vector<std::shared_ptr<Token>> args;
	for (const auto& arg : expr->m_arguments)
	{
		std::shared_ptr<Token> evalArg = std::static_pointer_cast<Token>(evaluate(arg));
		args.push_back(evalArg);
	}

	if (args.size() != function->GetArity()) 
	{
		throw RuntimeError(expr->m_paren, "Expected " + function->GetArity());// +" arguments but got " + args.size() + ".");
	}

	return function->Call(std::make_shared<Interpreter>(*this), args);
}

std::shared_ptr<void> Interpreter::visitSetExpr(const std::shared_ptr<SetExpr>& expr)
{
	// TODO: Find a better way to do this
	std::shared_ptr<LoxInstance> object = std::static_pointer_cast<LoxInstance>(evaluate(expr->m_obj));
	if (object == nullptr)
	{
		throw RuntimeError(expr->m_name, "Only instances have fields.");
	}

	std::shared_ptr<void> value = evaluate(expr->m_value);
	object->SetField(expr->m_name, value);

	return value;
}

std::shared_ptr<void> Interpreter::visitSuperExpr(const std::shared_ptr<SuperExpr>& expr)
{
	auto distIter = m_locals.find("super");
	if (distIter == m_locals.end())
	{
		throw RuntimeError(expr->m_keyword, "Variable is not a local variable.");
	}

	int dist = distIter->second;
	std::shared_ptr<LoxClass> superclass = std::static_pointer_cast<LoxClass>(m_environment->GetAt(dist, expr->m_keyword));

	// "this" is always one level nearer than "super"'s environment.
	std::shared_ptr <LoxInstance> object = std::static_pointer_cast<LoxInstance>(m_environment->GetAt(dist - 1, Token(THIS, std::string("this"), expr->m_keyword.GetLineNum())));

	std::shared_ptr<LoxFunction> method = superclass->FindMethod(expr->m_method.GetLexeme());
	if (method == nullptr) 
	{
		throw RuntimeError(expr->m_method,	"Undefined property '" + expr->m_method.GetLexeme() + "'.");
	}

	return method->Bind(object);
}

std::shared_ptr<void> Interpreter::visitThisExpr(const std::shared_ptr<ThisExpr>& expr)
{
	return lookupVar(expr->m_keyword, expr);
}

std::shared_ptr<void> Interpreter::visitVariableExpr(const std::shared_ptr<VariableExpr>& expr)
{
	return lookupVar(expr->m_name, expr);
}

std::shared_ptr<void> Interpreter::lookupVar(Token name, const std::shared_ptr<Expr>& expr)
{
	//auto iter = m_locals.find(expr);
	auto iter = m_locals.find(name.GetLexeme());
	if (iter != m_locals.end())
	{
		return m_environment->GetAt(iter->second, name);
	}
	else
	{
		return m_globalEnv->Get(name);
	}
}

void Interpreter::visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt)
{
	executeBlock(stmt->m_statements, m_environment);
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> environment)
{
	std::shared_ptr<Environment> previous = m_environment;
	try 
	{
		m_environment = std::make_shared<Environment>(environment);

		for (const auto& statement : statements) 
		{
			execute(statement);
		}

		m_environment = previous;
	}
	catch (ReturnException retEx)
	{
		m_environment = previous;
		throw(retEx);
	}
	catch(...)
	{
		m_environment = previous;
	}
}

void Interpreter::visitClassStmt(const std::shared_ptr<ClassStmt>& stmt)
{
	std::shared_ptr<LoxClass> superclass = nullptr;
	if (stmt->m_superclass != nullptr)
	{
		superclass = std::static_pointer_cast<LoxClass>(evaluate(stmt->m_superclass));
		if (superclass == nullptr)
		{
			throw RuntimeError(stmt->m_superclass->m_name, "Superclass must be a class.");
		}
	}

	m_environment->Define(stmt->m_name.GetLexeme(), nullptr);

	if (stmt->m_superclass != nullptr)
	{
		m_environment = std::make_shared<Environment>(m_environment);
		m_environment->Define("super", superclass);
	}

	std::map<std::string, std::shared_ptr<LoxFunction>> methods;
	for (const auto& method : stmt->m_methods)
	{
		methods.insert(std::make_pair<std::string, std::shared_ptr<LoxFunction>>(method->m_name.GetLexeme(), std::make_shared<LoxFunction>(method, m_environment, method->m_name.GetLexeme() == std::string("init"))));
	}

	if (stmt->m_superclass != nullptr)
	{
		m_environment = m_environment->GetEnclosingEnv();
	}

	m_environment->Assign(stmt->m_name, std::make_shared<LoxClass>(stmt->m_name.GetLexeme(), superclass, methods));
}

void Interpreter::visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt)
{
	evaluate(stmt->m_expr);
}

void Interpreter::visitIfStmt(const std::shared_ptr<IfStmt>& stmt)
{
	if (isTruthy(std::static_pointer_cast<Token>(evaluate(stmt->m_condition))))
	{
		execute(stmt->m_thenBranch);
	}
	else if (stmt->m_elseBranch != nullptr)
	{
		execute(stmt->m_elseBranch);
	}
}

void Interpreter::visitFunctionStmt(const std::shared_ptr<FunctionStmt>& stmt)
{
	std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(stmt, m_environment, false);
	m_environment->Define(stmt->m_name.GetLexeme(), function);
}

void Interpreter::visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt)
{
	std::shared_ptr<Token> value = std::static_pointer_cast<Token>(evaluate(stmt->m_expr));
	std::cout << stringify(value) << std::endl;
}

void Interpreter::visitReturnStmt(const std::shared_ptr<ReturnStmt>& stmt)
{
	std::shared_ptr<Token> value = nullptr;
	if(stmt->m_value != nullptr)
	{
		value = std::static_pointer_cast<Token>(evaluate(stmt->m_value));
	}

	throw ReturnException(value);
}

void Interpreter::visitVarStmt(const std::shared_ptr<VarStmt>& stmt)
{
	std::shared_ptr<Token> value = nullptr;
	if (stmt->m_initializer != nullptr)
	{
		value = std::static_pointer_cast<Token>(evaluate(stmt->m_initializer));
	}

	m_environment->Define(stmt->m_name.GetLexeme(), value);
}

void Interpreter::visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt)
{
	while (isTruthy(std::static_pointer_cast<Token>(evaluate(stmt->m_condition))))
	{
		execute(stmt->m_body);
	}
}

std::shared_ptr<void> Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
	return expr->accept(*this);
}

bool Interpreter::isTruthy(std::shared_ptr<Token> token)
{
	switch (token->GetType())
	{
		case ETokenType::STRING:
		case ETokenType::NUMBER:
		case ETokenType::TRUE:
			return true;
	}

	return false;
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) 
{
	stmt->accept(*this);
}

void Interpreter::Resolve(Token name, int depth)
{
	m_locals[name.GetLexeme()] = depth;
}

bool Interpreter::isEqual(std::shared_ptr<Token> left, std::shared_ptr<Token> right)
{
	return (left->GetLexeme() == right->GetLexeme());
}

std::shared_ptr<Token> Interpreter::createTruthToken(bool value, int lineNum)
{
	ETokenType type = value ? ETokenType::TRUE : ETokenType::FALSE;
	std::string lexeme = value ? "true" : "false";

	return std::make_shared<Token>(type, lexeme, lineNum);
}

std::shared_ptr<Token> Interpreter::createNumberToken(double value, int lineNum)
{
	return std::make_shared<Token>(ETokenType::NUMBER, std::to_string(value), lineNum);
}

std::shared_ptr<Token> Interpreter::createStringToken(std::string value, int lineNum)
{
	return std::make_shared<Token>(ETokenType::STRING, value, lineNum);
}

std::string Interpreter::stringify(std::shared_ptr<Token> value)
{
	switch (value->GetType())
	{
	case ETokenType::NUMBER:
		return std::to_string(value->GetLexemeAsDouble());
	default:
		return value->GetLexeme();
	}
}