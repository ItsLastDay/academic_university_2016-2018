module Classwork where
import Control.Monad
import Control.Applicative
import Control.Monad.State

newtype Reader r a = Reader { runReader :: (r -> a) }


instance Monad (Reader r) where
    return x = reader $ \e -> x
    m >>= k = reader $ \e -> let v = runReader m e
                                in runReader (k v) e

instance Functor (Reader r) where
    fmap f r = reader $ \e -> let v = runReader r e
                                in f v

instance Applicative (Reader r) where
    pure = return
    (<*>) = ap


reader :: (r -> a) -> Reader r a
reader x = Reader x

ask :: Reader r r
ask = reader id

asks :: (r -> a) -> Reader r a
asks f = reader $ \e -> f e

local :: (r -> r) -> Reader r a -> Reader r a
local f (Reader g) = reader $ g . f


data Logged a = Logged String a deriving (Eq, Show)


instance Monad Logged where
    return = Logged ""
    (Logged s x) >>= k = let (Logged s' x') = k x
                            in Logged (s' ++ s) x'

instance Functor Logged where
    fmap f (Logged s x) = Logged s (f x)

instance Applicative Logged where
    pure = return
    (<*>) = ap


write2log :: String -> Logged ()
write2log s = Logged s ()


logIt :: (Show a) => a -> Logged a
logIt v = do
    write2log $ "var = " ++ show v ++ "; "
    return v

test = do
    x <- logIt 3
    y <- logIt 5
    let res = x + y
    write2log $ "sum = " ++ show res ++ "; "
    return res


fac n = fst $ execState (replicateM n facStep) (1, 0)

facStep :: State (Integer, Integer) ()
facStep = do
    (fac, num) <- get
    put (fac * (num + 1), num + 1)


fac' n = execState (forM_ [1..n] facStep') 1

facStep' :: Integer -> State Integer ()
facStep' i = do
    modify (* i)



