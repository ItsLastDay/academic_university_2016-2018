{-# LANGUAGE FlexibleContexts #-}
module Homework where
import Control.Monad.Except

data ListIndexError = 
  ErrTooLargeIndex Int 
  | ErrNegativeIndex 
  | OtherErr String
  deriving (Eq, Show)

infixl 9 !!!

(!!!) :: (MonadError ListIndexError m) => [a] -> Int -> m a
xs !!! n | n < 0 = throwError ErrNegativeIndex
         | otherwise = helper xs n n where
    helper [] idx orig = throwError $ ErrTooLargeIndex orig
    helper (x:xs) idx orig | idx == 0 = return x
                           | otherwise = helper xs (idx - 1) orig

