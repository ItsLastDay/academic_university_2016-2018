module Fp05 where

-- переставляет элементы пары местами (определена в Data.Tuple)
swap         :: (a,b) -> (b,a)
swap (x,y)    = (y,x)


-- перечисление
data Color = Red | Green | Blue | Indigo | Violet deriving Show

isRGB          :: Color -> Bool
isRGB Red     =  True
isRGB Green   =  True
isRGB Blue    =  True
isRGB _       =  False -- Wild-card


-- декартово произведение
data PointDouble = PtD Double Double   deriving Show

midPointDouble                         :: PointDouble -> PointDouble -> PointDouble
midPointDouble (PtD x1 y1) (PtD x2 y2) = PtD ((x1 + x2) / 2) ((y1 + y2) / 2)


-- полиморфные типы
data Point a = Pt a a   deriving Show

midPoint                       :: Fractional a => Point a -> Point a -> Point a
midPoint (Pt x1 y1) (Pt x2 y2) = Pt ((x1 + x2) / 2) ((y1 + y2) / 2)


-- рекурсивные типы
data List a = Nil | Cons a (List a)   deriving Show

len :: List a -> Int
len (Cons _ xs) = 1 + len xs
len  Nil        = 0


-- выражение case ... of ...
head' xs = case xs of
  (x:_) -> x
  []    -> error "head': empty list"

-- образцы в лямбде  
head'' = \(x:_) -> x

-- Семантика сопоставления с образцом
foo (1, 2) = 3
foo (0, _) = 5
  
-- As-образец
dupFirst            :: [a] -> [a]
dupFirst (x:xs)     =  x:x:xs

dupFirst'           :: [a] -> [a]
dupFirst' s@(x:xs)  =  x:s

-- ленивые образцы
(***) f g ~(x, y) = (f x, g y)
-- (***) f g p = let (x, y) = p in (f x, g y) -- тоже подойдет


-- Метки полей (Field Labels)
data Point' a = Pt' {ptx, pty :: a}   deriving Show

absP p = sqrt (ptx p ^ 2 + pty p ^ 2)

absP' (Pt' {ptx = x, pty = y}) = sqrt (x ^ 2 + y ^ 2)

-- Стандартные алгебраические типы
head'''          :: [a] -> Either String a
head''' (x:_)    =  Right x
head''' []       =  Left "head'': empty list"

