#pragma once
#include "Types.h"
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include <vector>
#include <map>

class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
	Interpreter();
	~Interpreter();

	void Interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
	//void Resolve(const std::shared_ptr<Expr>& expr, int depth);
	void Resolve(Token name, int depth);

	std::shared_ptr<void> visitAssignExpr(const std::shared_ptr<AssignExpr>& expr) override;
	std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) override;
	std::shared_ptr<void> visitGetExpr(const std::shared_ptr<GetExpr>& expr) override;
	std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) override;
	std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) override;
	std::shared_ptr<void> visitLogicalExpr(const std::shared_ptr<LogicalExpr>& expr) override;
	std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) override;
	std::shared_ptr<void> visitCallExpr(const std::shared_ptr<CallExpr>& expr) override;
	std::shared_ptr<void> visitSetExpr(const std::shared_ptr<SetExpr>& expr) override;
	std::shared_ptr<void> visitSuperExpr(const std::shared_ptr<SuperExpr>& expr) override;
	std::shared_ptr<void> visitThisExpr(const std::shared_ptr<ThisExpr>& expr) override;
	std::shared_ptr<void> visitVariableExpr(const std::shared_ptr<VariableExpr>& expr) override;

	void visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) override;
	void visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) override;
	void visitClassStmt(const std::shared_ptr<ClassStmt>& stmt) override;
	void visitIfStmt(const std::shared_ptr<IfStmt>& stmt) override;
	void visitFunctionStmt(const std::shared_ptr<FunctionStmt>& stmt) override;
	void visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt) override;
	void visitReturnStmt(const std::shared_ptr<ReturnStmt>& stmt) override;
	void visitVarStmt(const std::shared_ptr<VarStmt>& stmt) override;
	void visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt) override;

	void executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> environment);

	std::shared_ptr<Environment> GetGlobalEnv() { return m_globalEnv; }

private:
	std::shared_ptr<Environment> m_globalEnv;
	std::shared_ptr<Environment> m_environment;
	//std::map<std::shared_ptr<Expr>, int> m_locals;
	std::map<std::string, int> m_locals;

	std::shared_ptr<void> evaluate(std::shared_ptr<Expr> expr);
	void execute(std::shared_ptr<Stmt> stmt);

	bool isTruthy(std::shared_ptr<Token> token);
	bool isEqual(std::shared_ptr<Token> left, std::shared_ptr<Token> right);

	std::shared_ptr<Token> createTruthToken(bool value, int lineNum);
	std::shared_ptr<Token> createNumberToken(double value, int lineNum);
	std::shared_ptr<Token> createStringToken(std::string value, int lineNum);

	std::string stringify(std::shared_ptr<Token> value);

	std::shared_ptr<void> lookupVar(Token name, const std::shared_ptr<Expr>& expr);
};

