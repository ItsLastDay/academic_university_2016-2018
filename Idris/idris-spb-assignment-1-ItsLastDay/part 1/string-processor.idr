import Functions

{-
  Write a complete repl-program which follows given protocol:
    * every input string starts with either P or C with one space after it
    * process rest of the string using `palindrome` or `counts` respectively
    
  Example session:
  > P noon
  noon is a palindrome
  > C Hello, Idris world!
  (3, 19)
  > P midday
  midday is not a palindrome
  
  Hints: 
    a) you may find functions `take`, `drop`, `words`, `unwords`, `substr`
       useful (make sure you understand what these functions do);
    b) try to implement as many auxiliary functions as you can.
-}

isCommandPal : String -> Bool
isCommandPal x = substr 0 1 x == "P"

performCount : (x : String) -> String
performCount x = (show $ counts x) ++ "\n"

performPal : (x : String) -> String
performPal x = case palindrome x of
                    False => x ++ " is not a palindrome\n"
                    True => x ++ " is a palindrome\n"

getQuery : String -> String
getQuery x = substr 2 (length x) x

processQuery : String -> String
processQuery x =  case isCommandPal x of
                       False => performCount q
                       True => performPal q
        where q : String
              q = getQuery x


main : IO ()
main = repl "" processQuery
