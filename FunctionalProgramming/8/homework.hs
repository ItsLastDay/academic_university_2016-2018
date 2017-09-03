module Homework where

{-
(>$<) func lst = getZipList (func <$> ZipList lst)
(>*<) lst1 lst2 = getZipList $ ZipList lst1 <*> ZipList lst2

data Triple a = Tr a a a deriving (Eq,Show)

instance Functor Triple where
    fmap f (Tr x y z) = Tr (f x) (f y) (f z)

instance Applicative Triple where
    pure x = Tr x x x
    Tr f1 f2 f3 <*> Tr x y z = Tr (f1 x) (f2 y) (f3 z)

instance Foldable Triple where
    foldMap f (Tr x y z) = f x `mappend` f y `mappend` f z

instance Traversable Triple where
    traverse f (Tr x y z) = pure Tr <*> f x <*> f y <*> f z
-}


data Tree a = Nil | Branch (Tree a) a (Tree a) deriving (Eq, Show)

instance Functor Tree where
    fmap f Nil = Nil
    fmap f (Branch x y z) = Branch (fmap f x) (f y) (fmap f z)

instance Applicative Tree where
    pure x = Branch (pure x) x (pure x)
    Nil <*> _ = Nil
    _ <*> Nil = Nil
    Branch x func z <*> Branch a b c = Branch (x <*> a) (func b) (z <*> c)

instance Foldable Tree where
    foldMap f Nil = mempty
    foldMap f (Branch x y z) = (foldMap f x) `mappend` (f y) `mappend` (foldMap f z) 

instance Traversable Tree where
    traverse f Nil = pure Nil
    traverse f (Branch x y z) = pure Branch <*> traverse f x <*> f y <*> traverse f z



newtype Cmps f g x = Cmps {getCmps :: f (g x)}   deriving (Eq,Show)

instance (Functor f, Functor g) => Functor (Cmps f g) where
    fmap f (Cmps x) = Cmps ((fmap . fmap) f x)


instance (Applicative f, Applicative g) => Applicative (Cmps f g) where
    pure x = Cmps $ (pure . pure) x
    Cmps func <*> Cmps val = Cmps $ (pure (<*>)) <*> func <*> val


instance (Foldable f, Foldable g) => Foldable (Cmps f g) where
    foldMap f (Cmps x) = foldMap (\t -> foldMap f t) x

instance (Traversable f, Traversable g) => Traversable (Cmps f g) where
    traverse func (Cmps x) = pure Cmps <*> traverse (\t -> traverse func t) x


