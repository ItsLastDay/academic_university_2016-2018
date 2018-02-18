
{-# LANGUAGE StandaloneDeriving, FlexibleContexts, UndecidableInstances #-}
newtype Fix f = In (f (Fix f))

deriving instance Show (f (Fix f)) => Show (Fix f)
deriving instance Eq (f (Fix f)) => Eq (Fix f)

out :: Fix f -> f (Fix f)
out (In x) = x

type Algebra f a = f a -> a

cata :: Functor f => Algebra f a -> Fix f -> a
cata phi (In x) = phi $ fmap (cata phi) x

type Coalgebra f a = a -> f a 

ana :: Functor f => Coalgebra f a -> a -> Fix f
ana psi x = In $ fmap (ana psi) (psi x)

hylo :: Functor f => Algebra f a -> Coalgebra f b -> (b -> a)
hylo phi psi = cata phi . ana psi

{-
data B b = Empty | Zero b | One b deriving (Eq, Show)
type Bin = Fix B

instance Functor B where
    fmap g Empty = Empty
    fmap g (Zero b) = Zero (g b)
    fmap g (One b) = One (g b)

bin2Int :: Bin -> Int
bin2Int = cata phiB

phiB :: B Int -> Int
phiB Empty = 0
phiB (Zero b) = b * 2
phiB (One b) = b * 2 + 1 

int2bin :: Int -> Bin
int2bin = ana psiB

psiB :: Int -> B Int
psiB 0 = Empty
psiB n = (if n `mod` 2 == 0 then Zero else One) (n `div` 2)
-}


{-
data E e = Num Int | Add e e | Mult e e deriving (Eq, Show) 

instance Functor E where
    fmap g (Num x) = Num x
    fmap g (Add e1 e2) = Add (g e1) (g e2)
    fmap g (Mult e1 e2) = Mult (g e1) (g e2)

type Expr = Fix E

phiE :: E Int -> Int
phiE (Num x) = x
phiE (Add e1 e2) = e1 + e2
phiE (Mult e1 e2) = e1 * e2

eval :: Expr -> Int
eval = cata phiE

phiEShow :: E String -> String
phiEShow (Num x) = show x
phiEShow (Add x y) = "(" ++ x ++ "+" ++ y ++ ")"
phiEShow (Mult x y) = "(" ++ x ++ "*" ++ y ++ ")"

en = In . Num
e3     = en 3
ep35   = In (Add e3 (en 5)) 
emp357 = In (Mult ep35 (en 7))
em7p35 = In (Mult (en 7) ep35)



phiEShowS :: E ShowS -> ShowS
phiEShowS (Num x) = shows x
phiEShowS (Add x y) = showString "+ " . x . showChar ' ' . y
phiEShowS (Mult x y) = showString "* " . x . showChar ' ' . y


type Stack = [Int]

push :: Int -> Stack -> Stack
push a as = a : as

add :: Stack -> Stack
add  (a : b : cs) = (b + a) : cs

mult :: Stack -> Stack
mult (a : b : cs) = (b * a) : cs

phiE' :: E (Stack -> Stack) -> Stack -> Stack
phiE' (Num x) = push x
phiE' (Add s1 s2) = \st -> add (s1 $ s2 st)
phiE' (Mult s1 s2) = \st -> mult (s1 $ s2 st)

eval' :: Expr -> Stack -> Stack
eval' = cata phiE'
-}

data T a x = Leaf | Branch x a x deriving (Eq, Show)

instance Functor (T a) where
    fmap g Leaf = Leaf
    fmap g (Branch a b c) = Branch (g a) b (g c)

type Tree a = Fix (T a)

phiTSum :: Algebra (T Integer) Integer
phiTSum Leaf = 0
phiTSum (Branch a b c) = a + b + c

treeSum :: Tree Integer -> Integer
treeSum = cata phiTSum

phiTInorder :: Algebra (T a) [a] -- T a [a] -> [a]
phiTInorder Leaf = []
phiTInorder (Branch a b c) = a ++ [b] ++ c

tree2listInorder :: Tree a -> [a] 
tree2listInorder = cata phiTInorder

psiTBST :: Ord a => Coalgebra (T a) [a]    -- [a] -> T a [a] 
psiTBST [] = Leaf
psiTBST (x:xs) = let
        lower = filter (< x) xs
        higher = filter (> x) xs
    in Branch lower x higher
    

list2BST :: Ord a => [a] -> Tree a
list2BST = ana psiTBST

sort :: Ord a => [a] -> [a]
sort = hylo phiTInorder psiTBST


iB l x r = In $ Branch l x r
iL = In Leaf

testTree = 
  iB
    (iB 
      (iB iL 
      2 
      iL) 
    3 
      (iB iL 
      4 
      iL)
    ) 
  5 
    (iB iL 
    6 
      (iB iL 
      7 
      iL)
    )
