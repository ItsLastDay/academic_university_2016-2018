module Homework where
sum'n'count :: Integer -> (Integer, Integer)
sum'n'count x | x == 0 = (0, 1)
              | otherwise = helper x
     where helper t | t == 0 = (0, 0)
                    | otherwise = let rec_res = helper (quot t 10)
                    in ((fst rec_res) + abs (rem t 10), (snd rec_res) + 1)


integration :: (Double -> Double) -> Double -> Double -> Double
integration f a b = 
    let t = (b - a) / 1000
    in let helper step acc | step == 1000 = acc
                        | otherwise = let this_result = t * (f (a + step * t) + f (a + (step + 1) * t)) / 2
                        in helper (step + 1) (acc + this_result)
    in helper 0 0.0
