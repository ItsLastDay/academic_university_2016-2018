module Homework where
import Control.Applicative

divideList' :: (Show a, Fractional a) => [a] -> (String,a)
divideList' []     = ("1.0", 1)
divideList' (x:xs) = (/) <$> ("<-" ++ (show x) ++ "/", x) <*> divideList' xs


newtype Arr2 e1 e2 a = Arr2 { getArr2 :: e1 -> e2 -> a }
newtype Arr3 e1 e2 e3 a = Arr3 { getArr3 :: e1 -> e2 -> e3 -> a }

instance Functor (Arr2 e1 e2) where
  fmap func (Arr2 x) = Arr2 (\a b -> func (x a b))

instance Functor (Arr3 e1 e2 e3) where
  fmap func (Arr3 x) = Arr3 (\a b c -> func (x a b c))

instance Applicative (Arr2 e1 e2) where
-- pure :: a -> (Arr2 e1 e2) a
--         a -> (Arr2 e1 -> e2 -> a)
  pure x = Arr2 (\a b -> x)
-- ap :: (Arr2 e1 e2) (a -> b) -> (Arr2 e1 e2) a -> (Arr2 e1 e2) b
  (Arr2 func) <*> (Arr2 x) = Arr2 (\a b -> func a b (x a b))

instance Applicative (Arr3 e1 e2 e3) where
  pure x = Arr3 (\a b c -> x)
  (<*>) (Arr3 func) (Arr3 x) = Arr3 (\a b c -> func a b c (x a b c))
  
  

newtype Parser a = Parser { apply :: String -> [(a, String)] }

parse p = map fst . filter (null . snd) . apply p

char :: Char -> Parser Char
char t = Parser f where
    f (c:cs) | c == t = [(c, cs)]
    f _ = []
    

instance Functor Parser where
  fmap func (Parser g) = Parser f
    where f = \s -> [(func a, b) | (a, b) <- g s]

instance Applicative Parser where
  pure x = Parser (\s -> [(x, s)])
  (<*>) (Parser first) (Parser second) = Parser f
    where f s = [(func elem, final_s) | (func, str) <- first s,
                    (elem, final_s) <- second str]

instance Alternative Parser where
  empty = Parser (\s -> [])
  (<|>) (Parser first) (Parser second) = Parser f where
    f s = (first s) ++ (second s)

