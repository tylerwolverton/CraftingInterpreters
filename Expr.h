#include "Types.h"

    template <typename T>
    class Visitor;

class Expr {
    template <typename T>
    T accept(Visitor<T> visitor);
};

  class Binary : Expr {
    public:
    Binary( Expr left, Token op, Expr right ) 
      : m_left(left),
      m_op(op),
      m_right(right)
        {}

    template <typename T>
    T accept(Visitor<T> visitor) {
        return visitor.visitBinaryExpr(this);
    }
      Expr m_left;
      Token m_op;
      Expr m_right;

  };

  class Grouping : Expr {
    public:
    Grouping( Expr expr ) 
      : m_expr(expr)
        {}

    template <typename T>
    T accept(Visitor<T> visitor) {
        return visitor.visitGroupingExpr(this);
    }
      Expr m_expr;

  };

  class Literal : Expr {
    public:
    Literal( Token literal ) 
      : m_literal(literal)
        {}

    template <typename T>
    T accept(Visitor<T> visitor) {
        return visitor.visitLiteralExpr(this);
    }
      Token m_literal;

  };

  class Unary : Expr {
    public:
    Unary( Token op, Expr right ) 
      : m_op(op),
      m_right(right)
        {}

    template <typename T>
    T accept(Visitor<T> visitor) {
        return visitor.visitUnaryExpr(this);
    }
      Token m_op;
      Expr m_right;

  };

    template <typename T>
    class Visitor { 
    public:
        T visitBinaryExpr(Binary expr);
        T visitGroupingExpr(Grouping expr);
        T visitLiteralExpr(Literal expr);
        T visitUnaryExpr(Unary expr);
    };

