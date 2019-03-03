#pragma once
#include "Types.h"
#include <memory>

#include <vector>
// This file was generated from ExprGenerator.exe. 
// If changes are needed, modify ExprGenerator/main.cpp 

class BlockStmt;
class ExpressionStmt;
class IfStmt;
class PrintStmt;
class VarStmt;
class WhileStmt;

class StmtVisitor { 
public:
    virtual void visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) = 0;
    virtual void visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) = 0;
    virtual void visitIfStmt(const std::shared_ptr<IfStmt>& stmt) = 0;
    virtual void visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt) = 0;
    virtual void visitVarStmt(const std::shared_ptr<VarStmt>& stmt) = 0;
    virtual void visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt) = 0;
};

class Stmt {
    public:
    virtual void accept(StmtVisitor& visitor) = 0;
};

class BlockStmt : public Stmt {
    public:
    BlockStmt( std::vector<std::shared_ptr<Stmt>> statements ) 
      : m_statements(statements)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitBlockStmt(std::make_shared<BlockStmt>(*this));
    }

    std::vector<std::shared_ptr<Stmt>> m_statements;
};

class ExpressionStmt : public Stmt {
    public:
    ExpressionStmt( std::shared_ptr<Expr> expr ) 
      : m_expr(expr)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitExpressionStmt(std::make_shared<ExpressionStmt>(*this));
    }

    std::shared_ptr<Expr> m_expr;
};

class IfStmt : public Stmt {
    public:
    IfStmt( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch ) 
      : m_condition(condition),
      m_thenBranch(thenBranch),
      m_elseBranch(elseBranch)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitIfStmt(std::make_shared<IfStmt>(*this));
    }

    std::shared_ptr<Expr> m_condition;
    std::shared_ptr<Stmt> m_thenBranch;
    std::shared_ptr<Stmt> m_elseBranch;
};

class PrintStmt : public Stmt {
    public:
    PrintStmt( std::shared_ptr<Expr> expr ) 
      : m_expr(expr)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitPrintStmt(std::make_shared<PrintStmt>(*this));
    }

    std::shared_ptr<Expr> m_expr;
};

class VarStmt : public Stmt {
    public:
    VarStmt( Token name, std::shared_ptr<Expr> initializer ) 
      : m_name(name),
      m_initializer(initializer)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitVarStmt(std::make_shared<VarStmt>(*this));
    }

    Token m_name;
    std::shared_ptr<Expr> m_initializer;
};

class WhileStmt : public Stmt {
    public:
    WhileStmt( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body ) 
      : m_condition(condition),
      m_body(body)
        {}

    void accept(StmtVisitor& visitor) override {
        return visitor.visitWhileStmt(std::make_shared<WhileStmt>(*this));
    }

    std::shared_ptr<Expr> m_condition;
    std::shared_ptr<Stmt> m_body;
};

