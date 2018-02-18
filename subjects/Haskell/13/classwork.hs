{-# LANGUAGE StandaloneDeriving #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE UndecidableInstances #-}
module Classwork where

{-
fromP :: (Integer, Integer) -> (Bool -> Integer)
fromP (a, b) = \x -> if x then a else b

toP :: (Bool -> Integer) -> (Integer, Integer)
toP f = (f True, f False)



data Tree a = Empty | Node a (Tree a) (Tree a)
    deriving (Eq, Show)

data Tree' a = Empty' | Node' a (Bool -> Tree' a)

instance Show a => Show (Tree' a) where
    showsPrec _ Empty' = showString "Empty'"
    showsPrec d (Node' x f) = showParen (d > app_prec) $
        showString "Node' " .
        showsPrec (app_prec + 1) x .
        showChar ' ' .
        showsPrec (app_prec + 1) (f True) .
        showChar ' ' .
        showsPrec (app_prec + 1) (f False)
        where app_prec = 10

instance Eq a => Eq (Tree' a) where
    Empty' == Empty' = True
    Empty' == x = False
    x == Empty' = False
    (Node' x f) == (Node' y g) = x == y
                        && f True == g True
                        && f False == g False

from :: Tree a -> Tree' a
from Empty = Empty'
from (Node a b c) = Node' a (\x -> if x then from b else from c)

to :: Tree' a -> Tree a
to Empty' = Empty
to (Node' a f) = Node a (to (f True)) (to (f False))
-}

newtype Fix f = In (f (Fix f))

deriving instance Show (f (Fix f)) => Show (Fix f)
deriving instance Eq (f (Fix f)) => Eq (Fix f)

out :: Fix f -> f (Fix f)
out (In x) = x

--data List a = Nil | Cons a (List a) deriving (Show, Eq)

data L a l = Nil | Cons a l deriving (Eq, Show)
type List a = Fix (L a)

instance Functor (L a) where
    fmap g Nil = Nil
    fmap g (Cons a x) = Cons a (g x)




from :: [a] -> List a
from [] = In Nil
from (x:xs) = In $ Cons x (from xs)


to :: List a -> [a]
to (In Nil) = []
to (In (Cons a xs)) = a:(to xs)

tst = In (Cons 1 (In (Cons 2 (In (Cons 3 (In Nil))))))
