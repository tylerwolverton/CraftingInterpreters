#pragma once
#include "Types.h"
#include <memory>

#include <vector>
// This file was generated from ExprGenerator.exe. 
// If changes are needed, modify ExprGenerator/main.cpp 

class AssignExpr;
class BinaryExpr;
class CallExpr;
class GetExpr;
class GroupingExpr;
class LiteralExpr;
class LogicalExpr;
class SetExpr;
class ThisExpr;
class UnaryExpr;
class VariableExpr;

class ExprVisitor { 
public:
    virtual std::shared_ptr<void> visitAssignExpr(const std::shared_ptr<AssignExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitCallExpr(const std::shared_ptr<CallExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitGetExpr(const std::shared_ptr<GetExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitLogicalExpr(const std::shared_ptr<LogicalExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitSetExpr(const std::shared_ptr<SetExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitThisExpr(const std::shared_ptr<ThisExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr) = 0;
    virtual std::shared_ptr<void> visitVariableExpr(const std::shared_ptr<VariableExpr>& expr) = 0;
};

class Expr {
    public:
    virtual std::shared_ptr<void> accept(ExprVisitor& visitor) = 0;
};

class AssignExpr : public Expr {
    public:
    AssignExpr( Token name, std::shared_ptr<Expr> value ) 
      : m_name(name),
      m_value(value)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitAssignExpr(std::make_shared<AssignExpr>(*this));
    }

    Token m_name;
    std::shared_ptr<Expr> m_value;
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

class CallExpr : public Expr {
    public:
    CallExpr( std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments ) 
      : m_callee(callee),
      m_paren(paren),
      m_arguments(arguments)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitCallExpr(std::make_shared<CallExpr>(*this));
    }

    std::shared_ptr<Expr> m_callee;
    Token m_paren;
    std::vector<std::shared_ptr<Expr>> m_arguments;
};

class GetExpr : public Expr {
    public:
    GetExpr( std::shared_ptr<Expr> obj, Token name ) 
      : m_obj(obj),
      m_name(name)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitGetExpr(std::make_shared<GetExpr>(*this));
    }

    std::shared_ptr<Expr> m_obj;
    Token m_name;
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

class LogicalExpr : public Expr {
    public:
    LogicalExpr( std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right ) 
      : m_left(left),
      m_op(op),
      m_right(right)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitLogicalExpr(std::make_shared<LogicalExpr>(*this));
    }

    std::shared_ptr<Expr> m_left;
    Token m_op;
    std::shared_ptr<Expr> m_right;
};

class SetExpr : public Expr {
    public:
    SetExpr( std::shared_ptr<Expr> obj, Token name, std::shared_ptr<Expr> value ) 
      : m_obj(obj),
      m_name(name),
      m_value(value)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitSetExpr(std::make_shared<SetExpr>(*this));
    }

    std::shared_ptr<Expr> m_obj;
    Token m_name;
    std::shared_ptr<Expr> m_value;
};

class ThisExpr : public Expr {
    public:
    ThisExpr( Token keyword ) 
      : m_keyword(keyword)
        {}

    std::shared_ptr<void> accept(ExprVisitor& visitor) override {
        return visitor.visitThisExpr(std::make_shared<ThisExpr>(*this));
    }

    Token m_keyword;
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

