{-# LANGUAGE StandaloneDeriving, FlexibleContexts, UndecidableInstances #-}
module Fp13 where

newtype Fix f = In {out :: f (Fix f)}
-- ср. fix f = f (fix f)
deriving instance Show (f (Fix f)) => Show (Fix f)
deriving instance Eq (f (Fix f)) => Eq (Fix f)

{-
> :k Fix
Fix :: (* -> *) -> *
> :t In
In :: f (Fix f) -> Fix f
-}


-- data Nat = Z | S Nat

-- функтор, описывающий структуру типа
-- N = \X. 1 + X
data N x = Z | S x deriving Show

instance Functor N where
  fmap g  Z    = Z
  fmap g (S x) = S (g x)

-- рекурсивный тип вводим через неподвижную точку функтора на уровне типов
type Nat = Fix N

{-
нерекурсивный функтор
> :t Z
Z :: N x
> :t S Z
S Z :: N (N x)
> :t S (S Z)
S $ S Z :: N (N (N x))
тип Nat, то есть Fix N, как его неподвижная точка
> :t In Z
In Z :: Fix N
> :t S (In Z)
S (In Z) :: N (Fix N)
> :t In (S (In Z))
In (S (In Z)) :: Fix N
> :t In (S (In (S (In Z))))
In (S (In (S (In Z)))) :: Fix N
-}


-- data List a = Nil | Cons a (List a)

-- функтор, описывающий структуру типа
-- L = \A. \X. 1 + A * X
data L a l = Nil | Cons a l deriving Show

instance Functor (L a) where
 fmap g Nil        = Nil
 fmap g (Cons a l) = Cons a (g l)

-- рекурсивный тип вводим через неподвижную точку функтора на уровне типов
type List a = Fix (L a) 

{-
-- нерекурсивный функтор
Nil                     :: L a l
Cons 'i' Nil            :: L Char (L a l)
Cons 'h' $ Cons 'i' Nil :: L Char (L Char (L a l))
тип List Char, то есть Fix (L Char), как его неподвижная точка
In Nil                                 :: Fix (L a)
In $ Cons 'i' $ In Nil                 :: Fix (L Char)
In $ Cons 'h' $ In $ Cons 'i' $ In Nil :: Fix (L Char)
-}



-----------------------------------------------------------------

-- Рассмотрим преобразование 
copy :: Functor f => Fix f -> Fix f
copy (In x) = In $ fmap copy x

-- Утверждение: это преобразование есть тождество.

{-
Напишем обобщение copy, которое заменяет In :: f (Fix f) -> Fix f не на себя же, 
а на произвольную phi :: f a -> a.
(Для данных функтора f и типа a функция phi :: f a -> a известна как f-алгебра.)
-}
type Algebra f a = f a -> a
{-
Получим обобщение понятия свёртки, катаморфизм:
-}

cata :: Functor f => Algebra f a -> Fix f -> a   -- Functor f => (f a -> a) -> Fix f -> a
cata phi (In x) = phi $ fmap (cata phi) x

{-

-}

-- Если в примере с функтором N задать функцию phi, заменяющую Z на 0, а S на succ, 
-- то получим N-алгебру типа N Int -> Int
phiN :: Algebra N Int -- алиас для N Int -> Int
phiN Z     = 0
phiN (S n) = succ n

-- Применяя cata к этой алгебре, получим стандартный преобразователь 
natToInt :: Nat -> Int -- алиас для Fix N -> Int
natToInt = cata phiN

{-
>  natToInt $ In (S (In (S (In Z))))
2
-}

-- Пример (L a)-алгебры
phiL :: Algebra (L a) [a] -- алиас для L a [a] -> [a]
phiL Nil         = []
phiL (Cons e es) = e : es

listify :: List a -> [a] -- алиас для Fix (L a) -> [a]
listify = cata phiL

{-
>  listify $ In Nil
[]
>  listify $ In $ Cons 'i' $ In Nil
"i"
>  listify $ In $ Cons 'h' $ In $ Cons 'i' $ In Nil
"hi"
-}

-- Ещё пара списочных алгебр

phiLLen :: Algebra (L a) Int --  L a Int -> Int
phiLLen Nil         = 0
phiLLen (Cons _ es) = 1 + es

phiLSum :: Num a =>  Algebra (L a) a -- Num a => L a a -> a
phiLSum Nil         = 0
phiLSum (Cons e es) = e + es


{-
> cata phiLLen $ In $ Cons 'h' $ In $ Cons 'i' $ In Nil
2
> cata phiLSum $ In $ Cons 2 $ In $ Cons 3 $ In Nil
5
-}

{-
Конструктор In :: f (Fix f) -> Fix f сам является алгеброй
-}
phiIn :: Algebra f (Fix f)
phiIn = In
{-
Эта алгебра называется инициальной алгеброй.


Инициальная алгебра сохраняет всю информацию о 
структуре, поданной на вход. Ее катаморфизм --- 
тождественная функция

copy :: Functor f => Fix f -> Fix f
copy = cata phiIn
-}

copy'' :: Functor f => Fix f -> Fix f
copy'' = cata phiIn  -- == id

{-
Имеется уникальный гомоморфизм из инициальной алгебры
в любую другую с данным функтором \lstinline!f!. 

ЗАКОНЫ

(1 cata-cancel) 
cata phi . In == phi . fmap (cata phi)

(2 cata-refl)
cata In == id

(3 cata-fusion)
f . phi == phi . fmap f => f . cata phi == cata phi

(4 cata-compose)
eps :: f :~> g => 
cata phi . cata (In . eps) == cata (phi . eps)

-}


----------------------------------------------------------------------

-- Анаморфизм

-- Введём операцию, обратную In
--out :: Fix f -> f (Fix f)
--out (In x) = x

-- Рассмотрим преобразование рекурсивного типа в себя
copy' :: Functor f => Fix f -> Fix f
copy' x = In $ fmap copy' (out x)

-- Утверждение: это преобразование есть тождество.

{-
--  Покажем что copy' = id на примере выражения In (S (In (S (In Z)))) типа Nat:
copy'               (In (S (In (S (In Z)))))   ~> по def copy'
In (fmap copy' (out (In (S (In (S (In Z))))))  ~> по def out
In (fmap copy'          (S (In (S (In Z))))    ~> по def fmap
In (S (copy'               (In (S (In Z)))))   ~> по def copy'
In (S (In (fmap copy' (out (In (S (In Z))))))) ~> по def out
In (S (In (fmap copy'          (S (In Z)))))   ~> по def fmap
In (S (In (S (copy'               (In Z)))))   ~> по def copy'
In (S (In (S (In (fmap copy'(out  (In Z))))))) ~> по def out
In (S (In (S (In (fmap copy'          Z)))))   ~> по def fmap
In (S (In (S (In Z)))))
-}

-- Заменим в copy' каждое вхождение out :: Fix f -> f (Fix f) 
-- на произвольную функцию psi :: a -> f a 

type Coalgebra f a = a -> f a 

-- Получим нотацию анаморфизма
ana :: Functor f => Coalgebra f a -> a -> Fix f
ana psi x = In $ fmap (ana psi) (psi x)

-- Пример 

psiN :: Coalgebra N Int   -- Int -> N Int
psiN 0 = Z
psiN n = S (n-1)

-- задает анаморфизм
intToNat :: Int -> Nat
intToNat = ana psiN

{-
> intToNat 3
In (S (In (S (In (S (In Z))))))
-}

{-
Терминальная коалгебра
Функция out :: Fix f -> f (Fix f) является коалгеброй
-}

psiOut :: Coalgebra f (Fix f)
psiOut = out

copy''' :: Functor f => Fix f -> Fix f
copy''' = ana psiOut  -- == id
