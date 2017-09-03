module Classwork where

num_even :: [Integer] -> Integer
num_even (x : xs) = (1 - x `rem` 2) + num_even xs
num_even [] = 0


filter_even :: [Integer] -> [Integer]
filter_even (x : xs)
    | even x = x : ft
    | otherwise = ft
  where ft = filter_even xs
filter_even [] = []


swap_odd_even :: [Integer] -> [Integer]
swap_odd_even [] = []
swap_odd_even [x] = [x]
swap_odd_even (x : y : z) = y : x : swap_odd_even z


paired_sum :: [Integer] -> [Integer] -> [Integer]
paired_sum (x : xs) [] = x : paired_sum xs []
paired_sum [] (y : ys) = y : paired_sum [] ys
paired_sum (x : xs) (y : ys) = (x + y) : paired_sum xs ys
paired_sum [] [] = []


myreverse :: [Integer] -> [Integer]
myreverse [] = []
myreverse = 
    let helper t 
