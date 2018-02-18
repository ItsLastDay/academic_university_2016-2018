{-
An integer arithmetic expression can be one of the following:
  1. A single integer
  2. Addition of an expression to an expression
  3. Subtraction of an expression from an expression
  4. Multiplication of an expression with an expression
Define a recursive data type Expr which can be used to represent such
expressions.

Look at data type Picture (4-pictures.idr) for ideas.
-}

data Expr = EInt Int
            | EAdd Expr Expr
            | ESub Expr Expr
            | EMul Expr Expr


-- Implement function, which evaluates an integer arithmetic expression.
evaluate : Expr -> Int 
evaluate (EInt x) = x
evaluate (EAdd x y) = evaluate x + evaluate y
evaluate (ESub x y) = evaluate x - evaluate y
evaluate (EMul x y) = evaluate x * evaluate y
