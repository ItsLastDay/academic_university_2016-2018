import Functions

{-
  Write a complete program which prompts for an input, calls 
  the function `counts` and prints its output.
-}



main : IO ()
main = do putStrLn "Please, enter a line. I will count the number of words and characters in it:"
          s <- getLine
          let (num_words, num_chars) = counts s
          putStrLn $ "Number of words: " ++ show num_words
          putStrLn $ "Number of characters: " ++ show num_chars
          
