#include "Types.h"
#include <memory>

    class BinaryExpr;

    class GroupingExpr;

    class LiteralExpr;

    class UnaryExpr;

class Visitor { 
public:
    virtual void visitBinaryExpr(const std::shared_ptr<BinaryExpr>& expr, const std::shared_ptr<std::string>& returnData) = 0;
    virtual void visitGroupingExpr(const std::shared_ptr<GroupingExpr>& expr, const std::shared_ptr<std::string>& returnData) = 0;
    virtual void visitLiteralExpr(const std::shared_ptr<LiteralExpr>& expr, const std::shared_ptr<std::string>& returnData) = 0;
    virtual void visitUnaryExpr(const std::shared_ptr<UnaryExpr>& expr, const std::shared_ptr<std::string>& returnData) = 0;
    };

class Expr {
    public:
    virtual void accept(Visitor& visitor, const std::shared_ptr<std::string>& returnStr) {}
};

  class BinaryExpr : public Expr {
    public:
    BinaryExpr( std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right ) 
      : m_left(left),
      m_op(op),
      m_right(right)
        {}

    void accept(Visitor& visitor, const std::shared_ptr<std::string>& returnStr) override {
        visitor.visitBinaryExpr(std::make_shared<BinaryExpr>(*this), returnStr);
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

    void accept(Visitor& visitor, const std::shared_ptr<std::string>& returnStr) override {
        visitor.visitGroupingExpr(std::make_shared<GroupingExpr>(*this), returnStr);
    }
      std::shared_ptr<Expr> m_expr;

  };

  class LiteralExpr : public Expr {
    public:
    LiteralExpr( Token literal ) 
      : m_literal(literal)
        {}

    void accept(Visitor& visitor, const std::shared_ptr<std::string>& returnStr) override {
        visitor.visitLiteralExpr(std::make_shared<LiteralExpr>(*this), returnStr);
    }
      Token m_literal;

  };

  class UnaryExpr : public Expr {
    public:
    UnaryExpr( Token op, std::shared_ptr<Expr> right ) 
      : m_op(op),
      m_right(right)
        {}

    void accept(Visitor& visitor, const std::shared_ptr<std::string>& returnStr) override {
        visitor.visitUnaryExpr(std::make_shared<UnaryExpr>(*this), returnStr);
    }
      Token m_op;
      std::shared_ptr<Expr> m_right;

  };

