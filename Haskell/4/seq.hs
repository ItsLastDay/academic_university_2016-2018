seqA :: Integer -> Integer
seqA n = 
    let helper a b c n | n == 0 = a
                       | n == 1 = b
                       | n == 2 = c
                       | otherwise = helper b c (c + b - 2 * a) (n - 1)
    in helper 1 2 3 n
