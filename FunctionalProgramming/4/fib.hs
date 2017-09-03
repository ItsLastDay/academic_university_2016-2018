fibonacci :: Integer -> Integer
fibonacci n = 
    let helper a b n | n == 0 = a
                     | n == 1 = b
                     | otherwise = helper b (a + b) (n - 1)
    in helper 0 1 (abs n)
