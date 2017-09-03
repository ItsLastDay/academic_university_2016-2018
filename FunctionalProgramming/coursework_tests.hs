module CourseworkTests where

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
                    |  not (elem v2 (freeVars e1)) = alphaEq (subst v1 (Var v2) e1) e2
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


-- компактно записанные переменные 
[a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z] = 
  map (Var . (:[])) "abcdefghijklmnopqrstuvwxyz"
-- аппликация двух аргументов
app2 f x y = f :@ x :@ y
-- аппликация трёх аргументов
app3 f x y z = f :@ x :@ y :@ z

-- комбинаторы
cI     = Lam "x" x
cK     = Lam "x" $ Lam "y" x
cK_ast = Lam "x" $ Lam "y" y
cB     = Lam "f" $ Lam "g" $ Lam "x" $ f :@ (g :@ x)
cS     = Lam "f" $ Lam "g" $ Lam "x" $ f :@ x :@ (g :@ x)

-- Булевы значения
fls = Lam "t" $ Lam "f" f
tru = Lam "t" $ Lam "f" t

iif = Lam "b" $ Lam "x" $ Lam "y" $ b :@ x :@ y

not' = Lam "b" $ Lam "t" $ Lam "f" $ app2 b f t
and' = Lam "x" $ Lam "y" $ app2 x y fls
or'  = Lam "x" $ Lam "y" $ app2 x tru y

-- пары
pair = Lam "x" $ Lam "y" $ Lam "f" $ app2 f x y

fst' = Lam "p" $ p :@ tru
snd' = Lam "p" $ p :@ fls

-- числа Чёрча
zero  = Lam "s" $ Lam "z" z
one   = Lam "s" $ Lam "z" $ s :@ z
two   = Lam "s" $ Lam "z" $ s :@ (s :@ z)
three = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ z))
four  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ z)))
five  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ z))))
six   = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))
seven = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z))))))
eight = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))))
nine  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z))))))))
ten   = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))))))
  
iszro = Lam "n" $ n :@ (Lam "x" fls) :@ tru

suc = Lam "n" $ Lam "s" $ Lam "z" $  s :@ (n :@ s :@ z)

plus  = Lam "m" $ Lam "n" $ Lam "s" $ Lam "z" $ app2 m s (app2 n s z)

mult = Lam "m" $ Lam "n" $ Lam "s" $ m :@ (n :@ s)

pow = Lam "m" $ Lam "n" $ n :@ m

omega = Lam "x" $ x :@ x

zp = pair :@ zero :@ zero
sp = Lam "p" $  pair :@ (snd' :@ p) :@ (suc :@ (snd' :@ p))
pred' = Lam "m" $ fst' :@ (m :@ sp :@ zp)

-- факториал
zf = pair :@ one :@ zero
sf = Lam "p" $ pair :@ (mult :@ (fst' :@  p) :@ (suc :@ (snd' :@ p))) :@ (suc :@ (snd' :@ p))
fac = Lam "m" $ fst' :@ (m :@ sf :@ zf)

-- общая схема примитивной рекурсии
xz = Lam "x" $ pair :@ x :@ zero
fs = Lam "f" $ Lam "p" $ pair :@ (f :@ (fst' :@ p) :@ (snd' :@ p)) :@ (suc :@ (snd' :@ p))
rec = Lam "m" $ Lam "f" $ Lam "x" $ fst' :@ (m :@ (fs :@ f) :@ (xz :@ x))

pred'' = Lam "n" $ rec :@ n :@ cK_ast :@ zero

minus = Lam "k" $ Lam "l" $ l :@ pred' :@ k

lt = Lam "n" $ Lam "m" $ not' :@ (iszro :@ (minus :@ m :@ n))
ge = Lam "n" $ Lam "m" $  iszro :@ (app2 minus m n)
gt = Lam "n" $ Lam "m" $ not' :@ (iszro :@ (app2 minus n m))
le = Lam "n" $ Lam "m" $  iszro :@ (app2 minus n m)
eq = Lam "n" $ Lam "m" $ and' :@ (le :@ m :@ n) :@ (ge :@ m :@ n)

fac'step = Lam "u" $ Lam "v" $ app2 mult u (suc :@ v)
fac' = Lam "n" $ app3 rec n fac'step one

-- Y-комбинатор
cY = Lam "f" $ (Lam "z" $ f :@ (z :@ z)) :@ (Lam "z" $ f :@ (z :@ z)) 
cTheta = aa :@ aa 
  where aa = Lam "a" $ Lam "b" $ b :@ (a :@ a :@ b)

fac''step = Lam "f" $ Lam "n" $ iif :@ (iszro :@ n) :@ one :@ (mult :@ n :@ (f :@ (pred' :@ n))) 
fac''  =  cY :@ fac''step
fac''' = cTheta :@ fac''step

-- списки
nil  = Lam "c" $ Lam "n" n
cons = Lam "e" $ Lam "l" $ Lam "c" $ Lam "n" $ app2 c e (app2 l c n)

l532 = app2 cons five (app2 cons three (app2 cons two nil))
l2 = Lam "c" $ Lam "n" $ c :@ two :@ n

empty = Lam "l" $ app2 l (Lam "h" $ Lam "t" fls) tru

length' = Lam "l" $ app2 l (Lam "x" $ Lam "y" $ suc :@ y) zero
length'' = Lam "l" $ app2 l (Lam "y" $ suc) zero

head' = Lam "l" $ app2 l cK cI

zpl = app2 pair nil nil
spl = Lam "e" $ Lam "p" $ app2 pair (snd' :@ p) (app2 cons e (snd' :@ p))
tail' = Lam "l" $ fst' :@ (app2 l spl zpl)

sum' = Lam "l" $ app2 l plus zero
sum'' = Lam "l" $ app2 l (Lam "h" $ Lam "t" $ app2 plus h t) zero


