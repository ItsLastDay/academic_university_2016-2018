{-# LANGUAGE Rank2Types #-}
{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE FlexibleInstances #-}
module Fp14SimpleLens where

{-
{-
Линза - инструмент для манипулирования элементом типа a некоторой структуры данных типа s,
находящимся в фокусе этой линзы.
Технически линза - это АТД составленный из пары геттер-сеттер
lens :: (s -> a) -> (s -> a -> s) -> Lens s a

Законы:

1) You get back what you put in:

view l (set l v s)  = v

2) Putting back what you got doesn't change anything:

set l (view l s) s  = s

3) Setting twice is the same as setting once:

set l v' (set l v s) = set l v' s



Наивный подход:
-}
data LensNaive s a = MkLens (s -> a) (s -> a -> s)

_1Naive :: LensNaive (a,b) a 
_1Naive = MkLens (\(x,_) -> x) (\(_,y) v -> (v,y)) 

viewNaive :: LensNaive s a -> s -> a
viewNaive (MkLens get _) s = get s

{-
GHCi> viewNaive _1Naive (5,7)
5

Это работает, но 
(1) неэффективно (конструктор данных MkLens дает дополнительный барьер
во время исполнения);
(2) имеет проблемы с расширением и обобщением (например, хотелось бы, чтобы
 композиция линз была линзой).
 
мы пишем сеттер и геттер вручную, но для записей с метками полей 
туда вкладываются непосредственно эти метки (в случае сеттера в синтаксисе обновления).
-}

----------------------------------------------
{-  
van Laarhoven lenses (Functor transformer lenses)
Линзы ван Ларховена 

Линза --- это функция, которая превращает вложение a в функтор f 
во вложение s в этот функтор.
-}
type Lens s a =
  forall f. Functor f =>
  (a -> f a) -> s -> f s

{-
NB Композиция в обратном порядке происходит отсюда!!!

l1 :: Lens t s -- (s -> f s) -> t -> f t
l2 :: Lens s a -- (a -> f a) -> s -> f s
l1 . l2 :: Lens t a


Как упаковать в такую конструкцию геттер и сеттер?
-}
  
--      (s -> a) -> (s -> a -> s) -> (a -> f a) -> s -> f s
lens :: (s -> a) -> (s -> a -> s) -> Lens s a
lens get set = \ret s -> fmap (set s) (ret $ get s)
{-
get s :: a
ret $ get s :: f a
set s :: a -> s
-}


-- Пример для пар: s == (a,b)

{-
--    (a -> f a) -> (a,b) -> f (a,b)
_1 :: Lens (a,b) a
_1 = lens (\(x,_) -> x)        -- get
          (\(_,y) v -> (v,y))  -- set

--    (b -> f b) -> (a,b) -> f (a,b)
_2 :: Lens (a,b) b 
-- _2 = lens (\(_,y) -> y) (\(x,_) v -> (x,v))
-- _2 = \ret (x,y) -> fmap ((\(x,_) v -> (x,v)) (x,y)) (ret $ (\(_,y) -> y) (x,y))
-- _2 = \ret (x,y) -> fmap (\v -> (x,v)) (ret $ y)
_2 ret (x,y) = fmap ((,) x) (ret y)
-}



{-
Как вынуть из линзы геттер и сеттер?
Использовать вместо f подходящий функтор!
-}

-- Геттер (x - фантомный параметр тпа)
newtype Const a x = Const {getConst :: a}
{-
Const    :: a -> Const a x
getConst :: Const a x -> a
-}

instance Functor (Const a) where
-- fmap :: (x -> y) -> Const a x -> Const a y
  fmap _ (Const v) = Const v
-- игнорирует функцию!
  
--      ((a -> Const a a) -> s -> Const a s) -> s -> a
view :: Lens s a                             -> s -> a
view lns s = getConst (lns Const s)
-- lns Const :: s -> Const a s
-- lns Const s :: Const a s
-- getConst :: Const a s -> a

{-
GHCi> view _1 (5,7)
5
GHCi> view _2 (5,7)
7
GHCi> view (_2 . _1) (5,(6,7))
6
-}

{-
view _2 (5,7) ~> 
getConst $ _2 Const (5,7) ~> 
getConst $ (\f (x,y) -> fmap ((,) x) (f y)) Const (5,7) ~> 
getConst $ fmap ((,) 5) (Const 7)) ~> 
getConst (Const 7) ~> 7

-}


-- Сеттер
newtype Identity a = Identity {runIdentity :: a}
{-
Identity :: a -> Identity a
runIdentity :: Identity a -> a
-}

instance Functor Identity where
  fmap f (Identity x) = Identity (f x)

--      ((a -> Identity a) -> s -> Identity s) -> (a -> a) -> s -> s
over :: Lens s a                               -> (a -> a) -> s -> s
over lns fn s = runIdentity $ lns (Identity . fn) s

{-
GHCi> over _1 (+5) (5,7)
(10,7)
GHCi> over _2 (+5) (5,7)
(5,12)
GHCi> over (_2 . _1) (+5) ("abc",(6,True))
("abc",(11,True))
-}

{-
over _2 (+5) (5,7) ~>
runIdentity $ _2 (Identity . (+5)) (5,7) ~>
runIdentity $ (\f (x,y) -> fmap ((,) x) (f y)) (Identity . (+5)) (5,7) ~>
runIdentity $ fmap ((,) 5) ((Identity . (+5)) 7) ~>
runIdentity $ fmap ((,) 5) ((Identity 12) ~>
runIdentity $  ((Identity (5,12)) ~>
(5,12)
-}



--     ((a -> Identity a) -> s -> Identity s) -> a -> s -> s
set :: Lens s a                               -> a -> s -> s

set lns a s = over lns (const a) s
--set lns a s = runIdentity $ lns (Identity . const a) s 

{-
GHCi> set _2 42 (5,7)
(5,42)
GHCi> set (_2 . _1) 33 ("abc",(6,True))
("abc",(33,True))
-}
  
data Tree a = Empty | Node (Tree a) a (Tree a)
    deriving (Show, Eq)

type TreeZ a = (a, CntxT a)

data CntxT a = CntxT (Tree a) (Tree a) [(Dir, a, Tree a)]
    deriving (Eq, Show)

data Dir = L | R deriving (Eq, Show)

mktz :: Tree a -> TreeZ a
mktz (Node tl a tr) = (a, CntxT tl tr [])

left :: TreeZ a -> TreeZ a
left (v, CntxT (Node son1 new_v son2) tr lst) = 
        (new_v, CntxT son1 son2 ((L, v, tr):lst))

right :: TreeZ a -> TreeZ a
right (v, CntxT tl (Node son1 new_v son2) lst) = 
        (new_v, CntxT son1 son2 ((R, v, tl):lst))

up :: TreeZ a -> TreeZ a
up (val, CntxT tl tr ((dir, root_val, other_son):lst)) | dir == L = 
        (root_val, CntxT (Node tl val tr) other_son lst)
    | dir == R = (root_val, CntxT other_son (Node tl val tr) lst)

untz :: TreeZ a -> Tree a
untz (root_val, CntxT tl tr []) = Node tl root_val tr
untz x = untz $ up x

updTZ :: a -> TreeZ a -> TreeZ a
updTZ new_v (v, ctx) = (new_v, ctx)


class Field1 s a | s -> a where
    _1 :: Lens s a

class Field2 s a | s -> a where
    _2 :: Lens s a

class Field3 s a | s -> a where
    _3 :: Lens s a


instance Field1 (a,b) a where
    _1 = lens (\(x, y) -> x) (\(x, y) v -> (v, y))

instance Field2 (b, a) a where
    _2 = lens (\(x, y) -> y) (\(x, y) v -> (x, v))


instance Field1 (a, b, c) a where
    _1 = lens (\(x, y, z) -> x) (\(x, y, z) v -> (v, y, z))

instance Field2 (a, b, c) b where
    _2 = lens (\(x, y, z) -> y) (\(x, y, z) v -> (x, v, z))

instance Field3 (a, b, c) c where
    _3 = lens (\(x, y, z) -> z) (\(x, y, z) v -> (x, y, v))

-}






































newtype Const a x = Const {getConst :: a}

instance Functor (Const a) where
  fmap _ (Const v) = Const v

newtype Identity a = Identity {runIdentity :: a}

instance Functor Identity where
  fmap f (Identity x) = Identity (f x)


class Field1 s t a b | s a b -> t,
                       s -> a,
                       t -> b
         where
    _1 :: Lens s t a b

class Field2 s t a b | s a b -> t,
                       s -> a,
                       t -> b
         where
    _2 :: Lens s t a b

class Field3 s t a b | s a b -> t,
                       s -> a,
                       t -> b
         where
    _3 :: Lens s t a b

type Lens s t a b = forall f. Functor f => (a -> f b) -> s -> f t

lens :: (s -> a) -> (s -> b -> t) -> Lens s t a b
lens get set = \ret s -> fmap (set s) (ret $ get s)

set :: Lens s t a b -> b -> s -> t
set lns a s = over lns (const a) s

view :: Lens s t a b -> s -> a
view lns s = getConst (lns Const s)

over :: Lens s t a b -> (a -> b) -> s -> t
over lns fn s = runIdentity $ lns (Identity . fn) s


instance Field1 (a,b) (c, b) a c where
    _1 = lens (\(x, y) -> x) (\(x, y) v -> (v, y))

instance Field2 (a,b) (a, c) b c where
    _2 = lens (\(x, y) -> y) (\(x, y) v -> (x, v))


instance Field1 (a, b, c) (x, b, c) a x where
    _1 = lens (\(x, y, z) -> x) (\(x, y, z) v -> (v, y, z))

instance Field2 (a, b, c) (a, x, c) b x where
    _2 = lens (\(x, y, z) -> y) (\(x, y, z) v -> (x, v, z))

instance Field3 (a, b, c) (a, b, x) c x where
    _3 = lens (\(x, y, z) -> z) (\(x, y, z) v -> (x, y, v))
