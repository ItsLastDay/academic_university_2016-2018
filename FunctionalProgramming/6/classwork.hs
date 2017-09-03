module Classwork where


data Tree a = Leaf a | Branch (Tree a) a (Tree a)

instance (Eq a) => Eq (Tree a) where
    (==) (Leaf x) (Leaf y) = x == y
    (==) (Branch l1 v1 r1) (Branch l2 v2 r2) = v1 == v2 && l1 == l2 && r1 == r2
    (==) _ _ = False

tr1 = Branch (Leaf 42) 15 (Branch (Leaf 0) 5 (Leaf 0))
tr2 = Branch (Leaf 42) 15 (Branch (Leaf 0) 5 (Leaf 0))
tr3 = Branch (Leaf 42) 15 (Branch (Leaf 0) 5 (Leaf 1))
tr4 = Branch (Leaf 42) 15 (Leaf 6)

inft n = Branch (inft (n + 1)) 1 (Leaf n)
inft_mirror n = Branch (Leaf n) 1 (inft_mirror (n + 1))
inft' n = Branch (inft' (n + 1)) n (inft' (n + 1))

inf_tr1 = inft 0
inf_tr2 = inft_mirror 0
inf_tr3 = inft' 0

elemTree tr x = 
    let helper [] x = False
        helper ((Leaf y):ys) x = y == x || helper ys x
        helper ((Branch le y rg):ys) x = y == x || helper (ys ++ [le, rg]) x
    in helper [tr] x


instance Functor Tree where
    -- :: (a -> b) -> Tree a -> Tree b
    fmap f (Leaf x) = Leaf (f x)
    fmap f (Branch le x rg) = Branch (fmap f le) (f x) (fmap f rg)

squared_tree_1 = fmap (^2) inf_tr1 


data List a = Nil | Cons a (List a)

instance (Show a) => Show (List a) where
    show Nil = "|"
    show (Cons x tail) = "<" ++ (show x) ++ (show tail) ++ ">" 


instance (Show a) => Show (Tree a) where
--    show (Leaf x) = show x
--    show (Branch le x rg) = "<" ++ (show le) ++ "{" ++ (show x) ++ "}"  ++ (show rg) ++ ">"
    showsPrec _ = myShowTree


myShowTree :: Show a => Tree a -> ShowS
myShowTree (Leaf x) = shows x
myShowTree (Branch le x rg) = ('<' :)
                            . shows le 
                            . (('{':) .(shows x) .('}':))
                            . shows rg 
                            . ('>' :)


instance (Read a) => Read (Tree a) where
    readsPrec _ = myReadsTree

myReadsTree :: Read a => ReadS (Tree a)
myReadsTree ('<':s) = [(Branch le x rg, tail) | (le, '{':t1) <- myReadsTree s,
                                              (x, '}':t2) <- reads t1,
                                              (rg, '>':tail) <- reads t2]
myReadsTree s = [(Leaf x, tail) | (x, tail) <- reads s]
