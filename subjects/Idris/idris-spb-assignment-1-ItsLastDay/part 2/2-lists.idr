||| Calculates length of the given list
length' : List a -> Nat
length' [] = 0
length' (x :: xs) = 1 + length' xs

||| Returnes reversed list
reverse' : List a -> List a
reverse' [] = []
reverse' (x :: xs) = (reverse' xs) ++ [x]

||| Calculates sum of the fist and the last elements of the given list 
||| or returns 0 if the list is empty
sum_fl : List Integer -> Integer
sum_fl [] = 0
sum_fl xs = getFirst xs + getFirst (reverse' xs)
    where getFirst : List Integer -> Integer
          getFirst (x :: xs) = x

||| Returns length of the given list if there exists
||| 0 in there or returns (-1) otherwise
len_withZero : List Integer -> Int
-- Use `filter` and `length`.
len_withZero xs = case length $ filter (== 0) xs of
                       Z => -1
                       (S k) => cast $ length xs



{-
 Define datatype with four possible values corresponding to various 
 list calculations:
   * sum
   * product
   * maximum
   * minimum
   
 Define function, which processes list according to the specified 
 calculation. If list is empty result should be Nothing.
-}

data Calculation : Type where
  CSum : Calculation
  CProduct : Calculation
  CMax : Calculation
  CMin: Calculation


process_list : Calculation -> (l: List Integer) -> Maybe Integer
process_list CSum l = Just $ sum l
process_list CProduct l = Just $ product l
process_list CMax l = Just $ foldr1 max l
process_list CMin l = Just $ foldr1 min l

{-
 Use previously defined function and datatype in the case when input data 
 are given in a string (first word in the string can be either SUM, PROD, 
 MIN, or MAX). Define whatever auxiliary functions you need.
 Your should return Nothing if the given string is somehow incorrect.
 
 Hint: functions `map` and `cast` may be useful.
 
 Recommendation: don't try to overcomplicate this function checking what 
 exactly follows the first word, just cast it to an Integer without doubt.
-}

getCalculation : String -> Maybe Calculation
getCalculation x = case take 1 $ words x of
                        ["SUM"] => Just CSum
                        ["PROD"] => Just CProduct
                        ["MAX"] => Just CMax
                        ["MIN"] => Just CMin
                        _ => Nothing


process_string : String -> Maybe Integer
process_string x = case getCalculation x of
                        Nothing => Nothing
                        (Just cal) => (case numbers of
                                            [] => Nothing
                                            _ => process_list cal numbers)
                    where numbers : List Integer
                          numbers = drop 1 $ map cast $ words x


-- Should be True
test : Bool
test = (process_string "SUM" == Nothing)
       && (process_string "SUM   " == Nothing)
       && (process_string "SUM 1 2 3" == Just 6)
       && (process_string "MAX 5 2 3" == Just 5)
       && (process_string "PROD 1 0 5" == Just 0)
       && (process_string "MIN 1 0 5" == Just 0)
       && (process_string "ABC 1 2 4" == Nothing)
