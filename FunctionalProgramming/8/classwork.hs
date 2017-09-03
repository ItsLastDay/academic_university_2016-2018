module Classwork where
import Control.Applicative

data Either' a b = Left' a | Right' b
    deriving Show

instance Functor (Either' e) where
    fmap = liftA

instance Applicative (Either' e) where
    pure = Right'
    (Left' a) <*> _ = Left' a
    _ <*> (Left' x) = Left' x
    (Right' a) <*> (Right' b) = Right' (a b)
    
    -- Other variang: Right' g <*> v = fmap g v


--instance Applicative ((->) e) where
--    pure = const
--    (<*>) f g x = f x (g x)


newtype Cmps f g x = Cmps {getCmps :: f (g x) }

instance (Functor f, Functor g) => Functor (Cmps f g) where
    fmap fun (Cmps t) = Cmps (fmap (fmap fun) (getCmps t))
