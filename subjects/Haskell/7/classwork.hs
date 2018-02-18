module Classwork where
import Data.Monoid

newtype Endom a = Endom { appEndom :: a -> a };

instance Monoid (Endom a) where
    mempty = Endom id
    Endom f `mappend` Endom g = Endom $ f . g

fn = mconcat $ map Endom [(+5), (*3), (^2)]

fn' = mconcat $ map (Dual . Endom) [(+5), (*3), (^2)]

{-
instance (Monoid a) => Monoid (Maybe a) where
    mempty = Nothing
    Nothing `mappend` Nothing = Nothing
    (Just x) `mappend` Nothing = Just x
    Nothing `mappend` Just y = Just y
    Just x `mappend` Just y = Just (x `mappend` y)
-}


f :: [String] -> String
f = foldr1 (\x y -> x ++ "," ++ y)


or' :: [Bool] -> Bool
or' = foldr (\x y -> x || y) False

length' :: [a] -> Int
length' = foldr (\_ y -> y + 1) 0

head' :: [a] -> a
head' = foldl1 (\x _ -> x)

last' :: [a] -> a
last' = foldr1 (\_ y -> y)

filter' :: (a -> Bool) -> [a] -> [a] 
filter' p = foldr (\x tail -> if p x then x:tail else tail) []

map' :: (a -> b) -> [a] -> [b]
map' f = foldr (\x tail -> (f x):tail) []


take' :: Int -> [a] -> [a]
take' n xs = foldr step ini xs n
    where
    step :: a -> (Int -> [a]) -> Int -> [a]
    step x g 0 = []
    step x g n = x : (g (n - 1))
    ini ::  Int -> [a]
    ini = const []




