{-# LANGUAGE FlexibleInstances #-}
module Parser_applicative where
import Data.Char

data Parser tok a = Parser { runParser :: [tok] -> Maybe ([tok], a) }


instance Functor (Parser Char) where
    fmap func (Parser f) = Parser res where
        res xs = (fmap . fmap) func (f xs)


instance Applicative (Parser Char) where
    pure x = Parser f where
        f xs = Just (xs, x)
    
    (Parser first) <*> (Parser second) = Parser result where
        result xs = case first xs of 
            Nothing -> Nothing
            Just (first_string, func) -> case second first_string of
                Nothing -> Nothing
                Just (second_string, val) -> Just (second_string, func val)

-- fmap :: (a -> b) -> (f a -> f b)
-- fmap . fmap :: (a -> b) -> (g f a -> g f b)


satisfy pred = Parser f where
    f (x:xs) | pred x = Just (xs, x)
    f _ = Nothing

digit = satisfy isDigit


test = runParser (pure ((^2) . digitToInt) <*> digit) "7ABC"



























