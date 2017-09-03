module Parser where

import Control.Applicative
import Data.Char

newtype Parser tok a = 
  Parser { runParser :: [tok] ->  Maybe ([tok],a) }

charA :: Parser Char Char
charA = Parser f where
  f (c:cs) | c == 'A' = Just (cs,c)
  f _                 = Nothing

{-
> runParser charA "ABC"
Just ('A',"BC")
> runParser charA "BCD"
Nothing
-}

satisfy :: (tok -> Bool) -> Parser tok tok
satisfy pr = Parser f where
  f (c:cs) | pr c  = Just (cs,c)
  f _              = Nothing

{-
> runParser (satisfy isUpper) "ABC"
Just ('A',"BC")
> runParser (satisfy isLower) "ABC"
Nothing
-}

lower :: Parser Char Char
lower = satisfy isLower

char :: Char -> Parser Char Char
char c = satisfy (== c)

digit :: Parser Char Int
digit = digitToInt <$> satisfy isDigit
-- для этого
instance Functor (Parser tok) where
--fmap :: (a -> b) -> Parser tok a -> Parser tok b
  fmap g (Parser p) = Parser f where
    f xs = case p xs of 
      Nothing      -> Nothing
      Just (cs, c) -> Just (cs, g c)
-- fmap g (Parser p) = Parser $ (fmap . fmap . fmap) g p    
{-
> runParser digit "12AB"
Just ("2AB",1)
> runParser digit "AB12"
Nothing
-}

{-
СЕМАНТИКА
pure: парсер, всегда возвращающий заданное значение;
(<*>): нужно получить результаты первого парсера, затем
второго, а после этого применить первые ко вторым.
-}  
instance Applicative (Parser tok) where
--pure :: a -> Parser tok a
  pure x = Parser $ \s -> Just (s, x)
--(<*>) :: Parser tok (a -> b) -> Parser tok a -> Parser tok b
  Parser u <*> Parser v = Parser f where
    f xs = case u xs of 
      Nothing       -> Nothing
      Just (xs', g) -> case v xs' of 
        Nothing        -> Nothing
        Just (xs'', x) -> Just (xs'', g x)

{-
А вот так не подойдет
  pure  = Parser . pure . pure . pure
  Parser u <*> Parser v = Parser $ (liftA2 . liftA2) (<*>) u v
Хотя это и настоящий композитный аппликативный функтор, но семантика не та.
-}
{-
> runParser (pure (,) <*> digit <*> digit) "12AB"
Just ("AB",(1,2))
> runParser (pure (,) <*> digit <*> digit) "1AB2"
Nothing
-}


multiplication :: Parser Char Int
multiplication = (*) <$> digit <*  char '*' <*> digit

{-
> runParser multiplication "6*7"
Just ("",42)
-}


-- Альтернативы


{-
СЕМАНТИКА
empty - парсер, всегда возвращающий неудачу;
<|> - пробуем первый, при неудаче пробуем второй на исходной строке.
-}

instance Alternative (Parser tok) where
--empty :: Parser tok a
  empty = Parser $ \_ -> Nothing
--(<|>) :: Parser tok a -> Parser tok a -> Parser tok a
  Parser u <|> Parser v = Parser f where 
    f xs = case u xs of
      Nothing -> v xs
      z       -> z



         
{-
> runParser  (char 'A' <|> char 'B') "ABC"
Just ("BC",'A')
> runParser  (char 'A' <|> char 'B') "BCD"
Just ("CD",'B')
> runParser  (char 'A' <|> char 'B') "CDE"
Nothing

> runParser (many digit) "12Cabdef"
Just ("Cabdef",[1,2])
> runParser (some digit) "12Cabdef"
Just ("Cabdef",[1,2])
-}

-- пример рекурсивного разбора

lowers :: Parser Char String
lowers = (:) <$> lower <*> lowers <|> pure ""


{-
> runParser lowers "abCd"
Just ("Cd","ab")
> runParser lowers "abcd"
Just ("","abcd")
> runParser lowers "Abcd"
Just ("Abcd","")
-}

multiplication2 :: Parser Char Int
multiplication2 = (*) <$> nat <*  char '*' <*> nat

nat :: Parser Char Int
nat = undefined

{-
> runParser multiplication2 "14*3"
Just ("",42)
-}
