#pragma once
#include "Types.h"
#include <memory>

// This file was generated from ExprGenerator.exe. 
// If changes are needed, modify ExprGenerator/main.cpp 

class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class VariableExpr;

class ExprVisitor { 
public:
    virtual std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitVariableExpr(const std::shared_ptr<VariableExpr>& expr) = 0;
};

class Expr {
    public:
    virtual std::shared_ptr<void> accept(ExprVisitor& visitor) = 0;
};

class BinaryExpr : public Expr {
    public:
    BinaryExpr( std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right ) 
      : m_left(left),
      m_op(op),
      m_right(right)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(std::make_shared<BinaryExpr>(*this));
    }

    std::shared_ptr<Expr> m_left;
    Token m_op;
    std::shared_ptr<Expr> m_right;
};

class GroupingExpr : public Expr {
    public:
    GroupingExpr( std::shared_ptr<Expr> expr ) 
      : m_expr(expr)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(std::make_shared<GroupingExpr>(*this));
    }

    std::shared_ptr<Expr> m_expr;
};

class LiteralExpr : public Expr {
    public:
    LiteralExpr( Token literal ) 
      : m_literal(literal)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(std::make_shared<LiteralExpr>(*this));
    }

    Token m_literal;
};

class UnaryExpr : public Expr {
    public:
    UnaryExpr( Token op, std::shared_ptr<Expr> right ) 
      : m_op(op),
      m_right(right)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(std::make_shared<UnaryExpr>(*this));
    }

    Token m_op;
    std::shared_ptr<Expr> m_right;
};

class VariableExpr : public Expr {
    public:
    VariableExpr( Token name ) 
      : m_name(name)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(std::make_shared<VariableExpr>(*this));
    }

    Token m_name;
};

