module Classwork where
import Data.Traversable
import Control.Applicative
import Data.Char


data Result a = Ok a | Error String
    deriving (Eq, Show)

instance Functor Result where
    fmap = fmapDefault -- через traverse

instance Foldable Result where
    foldMap = foldMapDefault  -- через traverse

instance Traversable Result where
    traverse _ (Error x) = pure (Error x)
    traverse g (Ok a) = pure Ok <*> g a 





data List a = None | Cons a (List a)
    deriving (Eq, Show)

instance Functor List where
    fmap = fmapDefault

instance Foldable List where
    foldMap = foldMapDefault

instance Traversable List where
    traverse _ None = pure None
    traverse g (Cons x y) = pure Cons <*> g x <*> traverse g y




newtype Parser tok a = Parser { runParser :: [tok] -> Maybe([tok], a) }

satisfy :: (tok -> Bool) -> Parser tok tok
satisfy pr = Parser f where
  f (c:cs) | pr c  = Just (cs,c)
  f _              = Nothing

lower :: Parser Char Char
lower = satisfy isLower

digit :: Parser Char Int
digit = digitToInt <$> satisfy isDigit

char :: Char -> Parser Char Char
char c = satisfy (== c)


instance Functor (Parser tok) where
--fmap :: (a -> b) -> Parser tok a -> Parser tok b
  fmap g (Parser p) = Parser f where
    f xs = case p xs of 
      Nothing      -> Nothing
      Just (cs, c) -> Just (cs, g c)

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

instance Alternative (Parser tok) where
--empty :: Parser tok a
  empty = Parser $ \_ -> Nothing
--(<|>) :: Parser tok a -> Parser tok a -> Parser tok a
  Parser u <|> Parser v = Parser f where 
    f xs = case u xs of
      Nothing -> v xs
      z       -> z


nat :: Parser Char Int
nat = (\x y -> if y == (- 1) then x else x * (10 ^ (length $ show y)) + y) <$> digit <*> nat <|> pure (- 1)

nat2 :: Parser Char Int
nat2 = (read :: Int) $ (:) <$> satisfy isDigit <*> nat2 <|> pure ""




