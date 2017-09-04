module Homework where

data LogLevel = Error | Warning | Info

cmp :: LogLevel -> LogLevel -> Ordering
cmp Error Error = EQ
cmp Warning Warning = EQ
cmp Info Info = EQ
cmp Error _ = GT
cmp Warning Error = LT
cmp Warning _ = GT
cmp _ _ = LT

data Person = Person { firstName :: String, lastName :: String, age :: Int }
    deriving Show

abbrFirstName :: Person -> Person
abbrFirstName (Person {firstName=x, lastName=y, age=z})
    | (length x) < 2 = Person {firstName=x, lastName=y, age=z}
    | otherwise = Person {firstName=(head x) : ".", lastName=y, age=z}


data Tree a = Leaf | Node (Tree a) a (Tree a)

treeSum :: Tree Integer -> Integer
treeSum Leaf = 0
treeSum (Node x y z) = y + treeSum x + treeSum z

treeHeight :: Tree a -> Int
treeHeight Leaf = 0
treeHeight (Node x y z) = 1 + (max (treeHeight x) (treeHeight z))


sum3 :: Num a => [a] -> [a] -> [a] -> [a]
sum3 (x:xs) (y:ys) (z:zs) = (x + y + z) : sum3 xs ys zs
sum3 [] (x:xs) (y:ys) = (x + y) : sum3 [] xs ys
sum3 (x:xs) [] (y:ys) = (x + y) : sum3 xs [] ys
sum3 (x:xs) (y:ys) [] = (x + y) : sum3 xs ys []
sum3 [] [] x = x
sum3 [] x [] = x
sum3 x [] [] = x


digits :: Integer -> [Integer]
digits x 
    | x < 0 = digits (-1 * x)
    | x == 0 = [0]
    | otherwise = helper x []
    where helper n lst | n == 0 = lst
            | otherwise = helper (n `div` 10) ((n `mod` 10) : lst)


containsAllDigitsOnes :: Integer -> Bool
containsAllDigitsOnes t =
    [ count x (digits t) | x <- [1..9] ] == 
        [ 1 | x <- [1..9]]
    where count x [] = 0
          count x (y:ys) | x == y = 1 + (count x ys)
                         | otherwise = count x ys


sublist :: Int -> Int -> [a] -> [a]
sublist _ _ [] = []
sublist 0 0 _ = []
sublist 0 k (x:xs) 
    | k > 0 = x : (sublist 0 (k - 1) xs)
    | otherwise = []
sublist n k (x:xs)
    | n >= k = []
    | n < 0 = sublist 0 k (x:xs)
    | otherwise = sublist (n - 1) (k - 1) xs



repeatEveryElem :: Int -> [a] -> [a]
repeatEveryElem k [] = []
repeatEveryElem 0 _ = []
repeatEveryElem k (x:xs)
    | k < 0 = []
    | otherwise = helper k x (repeatEveryElem k xs)
    where helper k x lst | k == 0 = lst
                | otherwise = helper (k - 1) x (x:lst)


movingLists :: Int -> [a] -> [[a]]
movingLists n [] = []
movingLists n (x:xs) 
    | (length y) == n = y : (movingLists n xs)
    | otherwise = []
    where y = (take n (x:xs))
