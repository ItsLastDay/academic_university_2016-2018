module Fp14Zipper where

update2 :: new -> (a, (old, c)) -> (a, (new, c))
update2 v (x,(_,z)) = (x,(v,z))

{-
GHCi> stru = (1,(2,(3,4)))
GHCi> update2 42 stru
(1,(42,(3,4)))
GHCi> update2 "Hello" stru
(1,("Hello",(3,4)))
-}

type Triple a = (a,(a,a))

type TripleZ a = (a,      -- фокус
                  Cntx a) -- контекст

data Cntx a = C1 ((),(a,a)) | C2 (a,((),a)) | C3 (a,(a,()))


type ListZ a = (a,       -- фокус
                CntxL a) -- контекст

type CntxL a = ([a],[a])

-- вкладываем список в зиппер
mklz :: [a] -> ListZ a
mklz (x:xs) = (x,([],xs))


-- навигация вперед
fwd :: ListZ a -> ListZ a
fwd (e,(xs,y:ys)) = (y,(e:xs,ys))

{-
GHCi> lz = mklz [0..3]
GHCi> lz
(0,([],[1,2,3]))
GHCi> fwd lz
(1,([0],[2,3]))
GHCi> (fwd . fwd) lz
(2,([1,0],[3]))
-}

-- навигация назад
bwd :: ListZ a -> ListZ a
bwd (e,(x:xs,ys)) = (x,(xs,e:ys))

unlz :: ListZ a -> [a]
unlz (x,([],xs)) = x:xs
unlz z  = unlz (bwd z)

{-
GHCi> lz' = (fwd . fwd) lz
GHCi> lz'
(2,([1,0],[3]))
GHCi> bwd lz'
(1,([0],[2,3]))
GHCi> unlz lz'
[0,1,2,3]
-}

-- внесение изменений в значение в фокусе
updLZ :: a -> ListZ a -> ListZ a
updLZ v (_,ctx) = (v,ctx)

insLZ :: a -> ListZ a -> ListZ a
insLZ v (e,(xs,ys)) = (v,(xs,e:ys))

delLZ :: ListZ a -> ListZ a
delLZ (e,(xs,y:ys)) = (y,(xs,ys))
delLZ (e,(x:xs,[])) = (x,(xs,[]))

{-
GHCi> (unlz . updLZ 42 . fwd . fwd . mklz) [0..3]
[0,1,42,3]
GHCi> (unlz . insLZ 33 . insLZ 42 . fwd . fwd . mklz) [0..3]
[0,1,33,42,2,3]
-}