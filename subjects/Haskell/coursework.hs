module Coursework where

type Symb = String 
infixl 2 :@
infix 1 `alphaEq`
infix 1 `betaEq`

data Expr = Var Symb
          | Expr :@ Expr
          | Lam Symb Expr
          deriving (Eq, Read)

instance Show Expr where
    showsPrec _ = myShowExpr

myShowExpr :: Expr -> ShowS
myShowExpr (Var x) = showString x
myShowExpr (x :@ y) = (showChar '(') 
            . (myShowExpr x) 
            . (showChar ' ') 
            . (myShowExpr y)
            . (showChar ')')
myShowExpr (Lam x y) = (showChar '(')
            . (showChar '\\')  
            . (showString x) 
            . (showString " -> ") . (myShowExpr y)
            . (showChar ')')

freeVars :: Expr -> [Symb]
freeVars (Var v) = [v]
freeVars (e1 :@ e2) = (freeVars e1) ++ (freeVars e2)
freeVars (Lam x e) = 
    let 
        erase x [] = []
        erase x (y:ys) | x == y = z
                     | otherwise = (y:z)
            where z = erase x ys
    in erase x (freeVars e)

subst :: Symb -> Expr -> Expr -> Expr 
subst v n m = 
    let 
        cnt x [] = 0
        cnt x (y:ys) | x == y = 1 + z
                     | otherwise = z
            where z = cnt x ys

        decor s 0 = s
        decor s cnt = (decor s (cnt - 1)) ++ "'"

        helper var_name sub expr stack | not (elem var_name (freeVars expr)) = expr

        helper var_name sub (Lam cat e) stack | elem cat (freeVars sub) = Lam (decor cat 
            (1 + (cnt cat stack))) (helper var_name sub e (cat:stack))
                | otherwise = Lam cat (helper var_name sub e (cat:stack))

        helper var_name sub (Var v) stack | z > 0 && (elem v (freeVars sub)) = Var (decor v z)
                                        | z > 0 && (not (elem v (freeVars sub))) = (Var v)
                                        | v == var_name = sub
                                        | otherwise = (Var v)
                where z = cnt v stack

        helper var_name sub (e1 :@ e2) stack = (helper var_name sub e1 stack) :@
                    (helper var_name sub e2 stack)

    in helper v n m []


alphaEq :: Expr -> Expr -> Bool
alphaEq (Var v) (Var y) = v == y
alphaEq (Lam v1 e1) (Lam v2 e2) | v1 == v2 = alphaEq e1 e2
                    | not (elem v2 (freeVars e1)) = alphaEq (subst v1 (Var v2) e1) e2
                    | otherwise = False
alphaEq (e1 :@ e2) (e3 :@ e4) = (alphaEq e1 e3) && (alphaEq e2 e4)
alphaEq _ _ = False




reduceOnce :: Expr -> Maybe Expr
reduceOnce (Var v) = Nothing
reduceOnce ((Lam v e1) :@ e2) = Just $ subst v e2 e1
reduceOnce (Lam v e1) = case reduceOnce e1 of
    Nothing -> Nothing
    Just x -> Just $ Lam v x
reduceOnce (e1 :@ e2) = case reduceOnce e1 of
    Just x -> Just $ x :@ e2
    Nothing -> case reduceOnce e2 of
        Nothing -> Nothing
        Just x -> Just $ e1 :@ x

nf :: Expr -> Expr
nf t = case reduceOnce t of
    Nothing -> t
    Just x -> nf x


betaEq :: Expr -> Expr -> Bool 
betaEq a b = alphaEq (nf a) (nf b)
