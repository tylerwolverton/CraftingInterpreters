#pragma once
#include "Types.h"
#include <memory>

class ExpressionStmt;
class PrintStmt;

class StmtVisitor { 
public:
    virtual void visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) = 0;
    virtual void visitPrintStmt(const std::shared_ptr<PrintStmt>& stmt) = 0;
    };

class Stmt {
    public:
    virtual void accept(StmtVisitor& visitor) = 0;
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

