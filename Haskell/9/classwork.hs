module Classwork where

res_1 = do  
    i <- Just 17
    t <- Just (i < 21)
    return t
    
res_2 = do
-- [1,2,3] >>= \x -> [x, 10 * x]. Мы связываем x с предыдущим.
    x <- [1,2,3]
    [x, 10 * x] 

res_3 = do
    n <- [1, 2]
    c <- ['a', 'b']
    return (n, c)

filter' p xs = do
    x <- xs
    True <- return (p x)
-- То же самое: 
-- if p x 
-- then "A"  <--- здесь не важно, какое значение, просто нужен эффект
--                непустого списка. Можно [()], т.е. unit-type
-- else
--   []
--
    return x

replicate' n x = do
    [1..n]
    return x

xs $>. y = xs >> return y

--fmap' f xs = xs >>= \x -> return (f x)
fmap' f xs = do
    x <- xs
    return (f x)

xs *>. ys = xs >> ys

liftA2' f xs ys = do 
    x <- xs
    y <- ys
    return (f x y)

{-fs <*>. xs = do
    f <- fs
    x <- xs
    return (f x)
-}
fs <*>. xs = fs >>= \f -> xs >>= \x -> return (f x)

f >=> g = \a -> f a >>= g
--join xs = xs >>= id
join xs = do
    x <- xs
    x



