import Functions

{-
  Write a complete program which prompts for an input, calls 
  the function `palindrome` and prints its output. Use `avemain.idr`
  from the second lecture as example.
-}



main : IO ()
main = do putStrLn "Please enter a string. I will check if it is a palindrome:"
          s <- getLine                                                            
          putStrLn $ "Is " ++ show s ++ " a palindrome? " ++ (show $ palindrome s)
