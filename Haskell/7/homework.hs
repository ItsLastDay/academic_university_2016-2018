module Homework where
import Data.List

{-
revRange :: (Char, Char) -> [Char]
revRange = unfoldr fun


fun = (\b -> if (fst b) <= (snd b) then 
    Just ((snd b), (fst b, pred (snd b))) 
    else Nothing)

tails' :: [a] -> [[a]]
tails' = foldr fun ini
fun x y = (x:(head y)):y
ini = [[]]

inits' :: [a] -> [[a]]
inits' = foldr fun' ini'
fun' x y = [] : [x:tail | tail <- y]
ini' = [[]]


reverse'' :: [a] -> [a]
reverse'' = foldl fun'' ini''
fun'' x y = y:x
ini'' = []


infixl 9 !!!

(!!!) :: [a] -> Int -> Maybe a
xs !!! n = foldr fun ini xs n 
fun x y 0 = Just x
fun x y f | f < 0 = Nothing
    | otherwise = y (f - 1)
ini = const Nothing



foldl'' :: (b -> a -> b) -> b -> [a] -> b
foldl'' f v xs = foldr (fun f) ini xs v
fun f x y v = y (f v x)
ini = id

-}

data Tree a = Nil | Branch (Tree a) a (Tree a)  deriving (Eq, Show)

newtype Preorder a = PreO (Tree a) deriving (Eq, Show)
newtype Postorder a = PostO (Tree a) deriving (Eq, Show)
newtype Levelorder a = LevelO (Tree a) deriving (Eq, Show)

reverse' :: [a] -> [a]
reverse' = foldr fun' ini'
fun' x y = y ++ [x]
ini' = []

instance Foldable Tree where
    foldr f z Nil = z
    foldr f z (Branch a x b) = foldr f (f x (foldr f z b)) a

instance Foldable Preorder where
    foldr f z t = myFoldr f z (getPreOrder t)

instance Foldable Postorder where
    foldr f z t = myFoldr f z (getPostOrder t)

instance Foldable Levelorder where
    foldr f z t = myFoldr f z (reverse' (getBfs [t] []))

myFoldr f z [] = z
myFoldr f z (x:t) = f x (myFoldr f z t)

getPreOrder :: Preorder a -> [a]
getPreOrder (PreO Nil) = []
getPreOrder (PreO (Branch a v b)) = [v] ++ (getPreOrder (PreO a)) ++ (getPreOrder (PreO b))

getPostOrder :: Postorder a -> [a]
getPostOrder (PostO Nil) = []
getPostOrder (PostO (Branch a v b)) = (getPostOrder (PostO a)) ++ (getPostOrder (PostO b)) ++ [v]

getBfs :: [Levelorder a] -> [a] -> [a]
getBfs [] x = x
getBfs ((LevelO Nil):tail) x = getBfs tail x
getBfs ((LevelO (Branch a v b)):tail) x = getBfs (tail ++ [(LevelO a), (LevelO b)]) (v:x)


tree = Branch (Branch Nil 1 (Branch Nil 2 Nil)) 3 (Branch Nil 4 Nil)
tr = Branch (Branch Nil 1 (Branch Nil 2 Nil)) 3 Nil
tre = Branch Nil 1 (Branch Nil 2 Nil)





