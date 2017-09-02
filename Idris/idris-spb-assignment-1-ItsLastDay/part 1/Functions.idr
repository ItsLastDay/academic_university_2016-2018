module Functions

%access export

{-
  1) Determine types of the following values without referring to REPL
  and then check your ideas with ":t". Don't forget to mention your 
  mistakes if any.
  
  1) ("A", "B", "C") : (String, String, String)
  2) ["A", "B", "C"] : List String
  3) ["A", "B", 'C'] : does not compile
  4) ("A", "B", 'C') : (String, String, Char)
  5) (("A", "B"), 'C') : ((String, String), Char)
  6) ("A", ("B", 'C')) : (String, (String, Char)) <-- fail, the true type is 
    (String, String, Char)
  7) [["A", "B"], ["C"]] : List (List String)
  8) [("A", "B"), "C"] : does not compile
-}

{-
  2) Implement case-insensitive function `palindrome` using 
  library functions reverse and toLower. Make sure your 
  implementation passes tests given.
-}

palindrome : String -> Bool
palindrome x = s == (reverse s)
    where 
      s : String
      s = toLower x


test_palindrome : String
test_palindrome =  if palindrome "pullup" 
                     && palindrome "Kayak"
                     && palindrome "noON"
                     && palindrome "Glenelg"
                     && palindrome "tattArratTat"
                     && palindrome "kuulilennuteetunneliluuk"
                     && not (palindrome "Idris")
                     && palindrome ""
                   then "Tests passed"
                   else "Tests failed"

-- Btw, do you know meanings of test words?

{-
  3) Write function `counts`, which returns a pair of the number of words 
  in the input and the number of characters in the input.  For example, 
  the input "Hello, Idris world!" should give the output (3, 19) .
  Provide tests for this function following the idea from previous exercise.
-}

counts : String -> (Nat, Nat)
counts x = (length $ words x, length x)

test_counts : String
test_counts = if counts "Hello, Idris world!" == (3, 19)
                 && counts "hello" == (1, 5)
                 && counts "Coup d'etat" == (2, 11)
                 && counts "a b c" == (3, 5)
                 && counts "wasser!!! wer???" == (2, 16)
                 && counts "I think, therefore I am." == (5, 24)
                 && counts "" == (0, 0)
              then "Tests passed"
              else "Tests failed"

{-
  4) Write function `top_ten`, which returns ten largest values in a list. 
  Hint: You may find functions `take` and `sort` useful (use :t and :doc for details). 
  Provide tests.
-}

top_ten: Ord a => List a -> List a
top_ten xs = take 10 (reverse $ sort xs)

test_top_ten : String
test_top_ten = if top_ten ["abcd", "de"] == ["de", "abcd"]
                  && top_ten [1,2,3,4,5] == reverse [1,2,3,4,5]
                  && top_ten (replicate 100 5) == replicate 10 5
                  && top_ten [9,2,3,1,11,-5,3,9,9,9,10,-4,0] == reverse [1,2,3,3,9,9,9,9,10,11]
                  && top_ten ((replicate 100 'a') ++ (replicate 100 'b')) == replicate 10 'b'
                  && top_ten [1.0, 2.0, 3.0, 4.0] == reverse [1.0, 2.0, 3.0, 4.0]
               then "Tests passed"
               else "Tests failed"


{-
  5) Write function `over_length`, which returns the number of strings in the list
  longer than the given number of characters. For example, evaluating 
     over_length 3 ["One", "Two", "Three", "Four"] 
  should give the output 2.
  Provide tests.
-}

over_length : Nat -> List String -> Nat
over_length k [] = 0
over_length k (x :: xs) = case length x > k of
                               False => over_length k xs
                               True => 1 + over_length k xs

test_over_length : String
test_over_length = if over_length 3 ["One", "Two", "Three", "Four"] == 2
                    && over_length 0 ["a", "", "asdasd"] == 2
                    && over_length 0 [] == 0
                    && over_length 100 ["abacabadabacaba", "qwE", "ttRTTT"] == 0
                    && over_length 2 ["ab", "cd", "defg"] == 1
                    && over_length 1 ["a", "b", "c"] == 0
                    && over_length 4 (replicate 150 "abcde") == 150
                   then "Tests passed"
                   else "Tests failed"
