module Homework where
import Control.Monad.Writer
import Control.Monad.State
import System.Random
import Data.IORef


minusLoggedR :: (Show a, Num a) => a -> [a] -> Writer String a
minusLoggedR init [] = writer (init, (show init))
minusLoggedR init (x:xs) = do
    let (val, prev_log) = runWriter $ minusLoggedR init xs
    tell $ "(" ++ (show x) ++ "-" ++ prev_log ++ ")"
    return (x - val)


minusLoggedL :: (Show a, Num a) => a -> [a] -> Writer String a
minusLoggedL init xs = 
    let 
        helper wr [] = wr
        helper wr (x:xs) = 
            let (cur_val, cur_log) = runWriter wr
                next_val = cur_val - x
                next_log = "(" ++ cur_log ++ "-" ++ (show x) ++ ")"
            in 
                helper (writer (next_val, next_log)) xs
    in 
        helper (writer (init, show init)) xs

fib :: Int -> Integer
fib n = fst $ execState (replicateM n fibStep) (0,1)

fibStep :: State (Integer,Integer) ()
fibStep = do 
    (cur_fib, next_fib) <- get
    put (next_fib, cur_fib + next_fib)


while :: IORef a -> (a -> Bool) -> IO () -> IO ()
while ref p action = do 
    cur <- readIORef ref
    if p cur then do
        action
        while ref p action
    else
        return () 

helper :: Int -> IO Double
helper n = do
    lst <- replicateM n $ (randomRIO (0, 1) :: IO Int)
    return $ abs $ fromIntegral (sum lst) - (fromIntegral n) / 2


avgdev :: Int -> Int -> IO Double
avgdev k n = do
    results <- replicateM k $ helper n
    return $ sum results / fromIntegral k



testWork :: ([Int], [Int])
testWork = evalState doWork (mkStdGen 42)

doWork :: State StdGen ([Int], [Int])
doWork = do
    xs <- replicateM 5 $ randomRState (1, 6)
    ys <- replicateM 5 $ randomRState (1, 6)
    return (xs, ys)



randomRState :: (Random a, RandomGen g) => (a, a) -> State g a 
randomRState (x,y) = do 
    (a, g) <- gets $ \g -> randomR (x, y) g
    put g
    return a
    

helper' n [] = []
helper' n lst = 
    let (h, t) = splitAt n lst
    in (abs $ fromIntegral (sum h) - fromIntegral(n) / 2):(helper' n t)

avgdev' :: Int -> Int -> State StdGen Double
avgdev' k n = do
    results <- replicateM (n * k) $ (randomRState (0, 1) :: (RandomGen g) => State g Int)
    return $ (sum $ helper' n results) / fromIntegral k


avgdev'' :: Int -> Int -> Double
avgdev'' k n = 
    let 
        gen = mkStdGen 1234
        results = take (k * n) ((randomRs (0, 1) gen) :: [Int])
        helper [] = []
        helper lst = (abs $ fromIntegral(sum (take n lst)) - (fromIntegral n) / 2):(helper $ drop n lst)
    in 
        (sum $ helper results) / fromIntegral k

