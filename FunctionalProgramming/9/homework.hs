module Homework where

surround :: a -> a -> [a] -> [a]
surround x y zs = do 
    z <- zs
    x : z : y : []

lookups :: (Eq k) => k -> [(k,v)] -> [v]
lookups x ys = do 
    y <- ys
    True <- [(fst y) == x]
    return (snd y)


factor2 :: Integer -> [(Integer, Integer)]
factor2 n = do 
    i <- [1..ceiling (sqrt (fromIntegral n))]
    True <- return (i <= (n `div` i) && (n `mod` i == 0))
    return (i, n `div` i)


absDiff :: Num a => [a] -> [a]
absDiff xs = do 
   (fi, se) <- zip xs (tail xs) 
   return $ abs (fi - se)

data Cell = E  -- empty, пустая клетка
          | X  -- крестик 
          | O  -- нолик
          deriving (Eq,Show)

type Row a = [a]
type Board = Row (Row Cell)

-- Начальная конфигурация для поля размера n
iniBoard :: Int -> Board
iniBoard n = let row = replicate n E in replicate n row

win :: Cell -> Board -> Bool
win E _   = False
win x brd = 
    let 
        checkBoard board = (any (\row -> all (\cell -> cell == x) row) board) 
            || (all (\cell -> cell == x) (zipWith (\row idx -> row !! idx) board
                [0..(length board) - 1]))
            || (all (\cell -> cell == x) (zipWith (\row idx -> row !! idx) board
                [(length board) - 1,(length board) - 2..0]))
        transpose ([]:_) = []
        transpose xs = (map head xs) : transpose (map tail xs)
    in checkBoard brd || checkBoard (transpose brd)

nxt :: Cell -> Board -> [Board]
nxt x brd = do 
    False <- return (win X brd || win O brd)
    let n = length brd
    idx <- [0..n*n-1]
    let flat = concat brd
    let (head, (elem:tail)) = splitAt idx flat
    True <- return (elem == E)
    let new_brd = (head ++ (x:tail))
    let unflat [] = []
        unflat lst = (take n lst) : (unflat $ drop n lst)
    let unflat_brd = unflat new_brd
    return unflat_brd
    

doNTurns :: Int -> Cell -> Board -> [Board]
doNTurns 0 frst brd = return brd
doNTurns n frst brd = do
    next_move <- nxt frst brd
    let next_turn = if frst == X then O else X
    doNTurns (n - 1) next_turn next_move 
