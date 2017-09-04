{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE MultiParamTypeClasses #-}
import Control.Monad.Except
import Control.Applicative

data Excep a =  Err String | Ok a 
  deriving (Eq, Show)


instance Applicative Excep where
    pure = Ok
    (Err x) <*> _ = Err x
    (Ok a) <*> (Err x) = Err x
    (Ok a) <*> (Ok b) = Ok $ a b

instance Functor Excep where
    fmap f (Err s) = Err s
    fmap f (Ok a) = Ok $ f a
    

instance Monad Excep where
    return = Ok
-- >>= :: Excep a -> (a -> Excep b) -> Excep b
    (Err t) >>= k = Err t
    (Ok a) >>= k = k a
    fail _ = Err "Monad.fail error."

instance Alternative Excep where
    empty = Err "Alternative.empty error."
    m@(Ok _) <|> _ = m
    (Err _) <|> x = x

instance MonadPlus Excep where
    mzero = Err "" 
    m@(Ok a) `mplus` _ = m
    (Err x) `mplus` (Err y) = Err $ x ++ y
    (Err x) `mplus` m@(Ok _) = m

instance MonadError String Excep where
    throwError = Err 
    catchError (Err s) h = h s
    catchError m@(Ok a) _ = m


-- тестирование
(?/) :: (MonadError String m) 
            => Double -> Double -> m Double
x ?/ 0 = throwError "Division by 0."
x ?/ y = return $ x / y

example :: Double -> Double -> Excep String
example x y = action  `catchError` return where 
  action = do 
    q <- x ?/ y
    guard (q >=0)
    if q  > 100 then do 
      100 <- return q
      undefined
    else 
      return $ show q

