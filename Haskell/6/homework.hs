module Homework where
import Data.Complex

newtype Matrix a = Matrix [[a]]


instance (Show a) => Show (Matrix a) where
    showsPrec _ = myShowMatrix

myShowMatrix :: Show a => Matrix a -> ShowS
myShowMatrix (Matrix []) = showString "EMPTY"
myShowMatrix (Matrix x) = 
    let 
        helper (Matrix (t:[])) = showList t
        helper (Matrix (lst:rest)) = (showList lst)
                    . (showChar '\n')
                    . (helper (Matrix rest))
    in helper (Matrix x)


newtype Cmplx = Cmplx (Complex Double) deriving Eq

instance Show Cmplx where
    showsPrec _ = myShowCmplx

myShowCmplx :: Cmplx -> ShowS
myShowCmplx (Cmplx (x :+ y)) 
    | y < 0 = (shows x) . (showString "-i*") . (shows (- y))
    | otherwise = (shows x) . (showString "+i*") . (shows y)

instance Read Cmplx where
    readsPrec _ = myReadCmplx

myReadCmplx :: ReadS Cmplx
myReadCmplx s = 
    [(Cmplx (x :+ y), tail) | (x, '+':'i':'*':t1) <- reads s,
                                        (y, tail) <- reads t1]
        ++
    [(Cmplx (x :+ (- y)), tail) | (x, '-':'i':'*':t1) <- reads s,
                                        (y, tail) <- reads t1]


class (Enum a, Bounded a, Eq a) => SafeEnum a where
    ssucc :: a -> a
    ssucc x | x == (maxBound `asTypeOf` x) = (minBound `asTypeOf` x)
            | otherwise = succ x

    spred :: a -> a
    spred x | x == (minBound `asTypeOf` x) = (maxBound `asTypeOf` x)
            | otherwise = pred x



rotate :: Int -> [a] -> [a]
rotate 0 x = x
rotate k [] = []
rotate n x | n > 0 = zipWith const (drop n (cycle x)) x
        | otherwise = take (length x) $ drop ((length x) - ((- n) `mod` (length x))) (cycle x)




comb :: Int -> [a] -> [[a]]
comb 0 _ = [[]]
comb k _ | k <= 0 = []
comb _ [] = [[]]
comb 1 x = [[t] | t <- x]
comb k (x:xs) = [(x:t) | t <- comb (k - 1) xs] ++ (comb k xs)




