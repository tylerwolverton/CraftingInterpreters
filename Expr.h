#include "Types.h"
class Expr {
};

  class Binary : Expr {
    Binary( Expr left, Token op, Expr right ) 
      : m_left(left),
      m_op(op),
      m_right(right)
        {}

      Expr m_left;
      Token m_op;
      Expr m_right;

  };

  class Grouping : Expr {
    Grouping( Expr expr ) 
      : m_expr(expr)
        {}

      Expr m_expr;

  };

  class Literal : Expr {
    Literal( Token literal ) 
      : m_literal(literal)
        {}

      Token m_literal;

  };

  class Unary : Expr {
    Unary( Token op, Expr right ) 
      : m_op(op),
      m_right(right)
        {}

      Token m_op;
      Expr m_right;

  };

