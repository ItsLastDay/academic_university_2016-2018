module Classwork where
import Data.Function
tripler x = 3 * x

sign x  
  | x > 0 = 1
  | x == 0 = 0
  | otherwise = -1

-- Pattern matching
pierce False False = True
pierce _ _ = False

-- Using accumulator
fib n = 
    let helper a b n | n == 0 = a
                     | n == 1 = b
                     | otherwise = helper b (a + b) (n - 1)
    in helper 0 1 n

-- (a -> b -> c) -> b -> a -> c
flip f = \x y -> f y x


factorial n = if n > 0 then n * factorial(n - 1) else 1

factorial' = fix (\f n -> if n > 0 then n * f(n - 1) else 1)



sum_sq = on (+) (^ 2)
