module Main

import Data.Vect

infixr 5 .+.

data Schema = SString                -- String component
            | SInt                   -- Int Component
            | SBool                  -- Bool component
            | (.+.) Schema Schema    -- Combination of schemas (use tuples)

SchemaType : Schema -> Type
-- Calculates type for the datastore elements
-- e.g. (String, String, Int) from (SString .+. (SString .+. SInt))
SchemaType SString = String
SchemaType SInt = Int
SchemaType SBool = Bool
SchemaType (x .+. y) = (SchemaType x, SchemaType y)

record DataStore where
  constructor MkData
  schema : Schema
  size : Nat
  items : Vect size (SchemaType schema)

addToStore : (store : DataStore) -> SchemaType (schema store) -> DataStore
addToStore (MkData schema size items) x = MkData schema (size + 1) (items ++ [x])

setSchema : (store : DataStore) -> Schema -> Maybe DataStore
-- setting schema should be supported only if the store is empty, 
-- otherwise return Nothing
setSchema (MkData schema Z items) x = Just $ MkData x 0 []
setSchema (MkData schema (S k) items) x = Nothing

-- User commands
data Command : Schema -> Type where
     SetSchema : Schema -> Command schema
     Add : SchemaType schema -> Command schema
     Get : Integer -> Command schema
     GetAll : Command schema
     Quit : Command schema
     Clear : Command schema


||| Parse an integer that starts at the beginning of the first argument. 
||| The second argument (boolean) indicates whether we can have '-' as prefix (i.e. negative number).
||| Arguments: 
|||     List Char -- string to parse
|||     Bool      -- can we have '-' in the beginning.
|||
||| Example usage: 
|||     parseInteger (unpack "-123 abcd") True => Just (-123, "abcd")
|||
||| Valid examples:
|||    123 aa 
|||    -3 x
|||    345f             (whitespaces at the end are not enforced)
||| Invalid examples (return value is Nothing):
|||    --3
|||    x
parseInteger : List Char -> Bool -> Maybe (SchemaType SInt, String)
parseInteger [] y = Nothing
parseInteger ('-' :: xs) True = case parseInteger xs False of
                                     Nothing => Nothing
                                     (Just (int, s)) => Just (negate int, s)
parseInteger xs _ = (case span isDigit xs of
                          ([], b) => Nothing
                          (xs, s) => Just (cast $ pack xs, ltrim $ pack s))                                     

||| Parse a string that starts at the beginning of the first argument.
||| String must be enclosed in double quotes. Escape sequences do not exist.
||| Both quotes are parsed.
|||
||| Arguments: 
|||     List Char -- string to parse
|||
||| Example usage: 
|||     parseString (unpack "\"qqq eee\"xxx") => Just ("qqq eee", "xxx")
|||
||| Valid examples:
|||    "sdf 234 34f"
|||    "a\"324           (returns Just ("a\\", "324")
|||    ""
||| Invalid examples (return value is Nothing):
|||    "123
|||    abc
parseString : List Char -> Maybe (SchemaType SString, String)
parseString ('"' :: xs) = parseInsideQuotes xs
    where parseInsideQuotes : List Char -> Maybe (SchemaType SString, String)
          parseInsideQuotes [] = Nothing
          parseInsideQuotes ('"' :: xs) = Just ("", ltrim $ pack xs)
          parseInsideQuotes (x :: xs) = case parseInsideQuotes xs of
                                             Nothing => Nothing
                                             (Just (s, unparsed)) => Just (strCons x s, unparsed)
parseString _ = Nothing

||| Boolean values are represented as "True" or "False" (without quotes, case-sensitive).
parseBool : String -> Maybe (SchemaType SBool, String)
parseBool y = case substr 0 lt y of
                   "True" => Just $ (True, ltrim $ substr lt (length y) y)
                   _ => (case substr 0 lf y of
                              "False" => Just $ (False, ltrim $ substr lf (length y) y)
                              _ => Nothing)
    where lt : Nat
          lt = length "True"
          lf : Nat
          lf = length "False"


parsePortion : (schema : Schema) -> String -> Maybe (SchemaType schema, String)
-- This function tries to parse one element of a schema (string or integer or pair)
-- in one recursive step, it is the harderst function to implement. 
-- Second component of the pair is what was left unparsed.
-- Use span, pack, ltrim, cast
parsePortion SString x = parseString (unpack x)
parsePortion SInt x = parseInteger (unpack x) True
parsePortion SBool x = parseBool x
parsePortion (y .+. z) x = case parsePortion y x of
                                Nothing => Nothing
                                (Just (left_parsed, left_unparsed)) => 
                                    (case parsePortion z left_unparsed of
                                           Nothing => Nothing
                                           (Just (right_parsed, right_unparsed)) =>
                                                    Just ((left_parsed, right_parsed), right_unparsed))

parseBySchema : (schema : Schema) -> String -> Maybe (SchemaType schema)
parseBySchema schema x = case parsePortion schema x of
                              Nothing => Nothing
                              Just (res, "") => Just res -- returns Just only if
                                                         -- everything was parsed
                              Just _ => Nothing

||| String representation of Schema: a string of characters 'B', 'I' and 'S' without spaces.
||| Each 'S' represents a String, 'I' represents an Integer, 'B' represents Boolean.
parseSchema : String -> Maybe Schema
parseSchema "I" = Just SInt
parseSchema "S" = Just SString
parseSchema "B" = Just SBool
parseSchema s = case substr 0 1 s of
                     "S" => case parseSchema rest of
                                     Nothing => Nothing
                                     (Just x) => Just $ SString .+. x
                     "I" => case parseSchema rest of
                                     Nothing => Nothing
                                     (Just x) => Just $ SInt .+. x
                     "B" => case parseSchema rest of
                                     Nothing => Nothing
                                     (Just x) => Just $ SBool .+. x
                     _ => Nothing
        where rest : String
              rest = substr 1 (length s) s

||| User commands are represented by one-letter strings:
||| S - set schema
||| A - add tuple into database
||| G - get tuple by index (0-based). If no index is provided, list all tuples in chronological order.
||| Q - quit
||| C - clear database
|||
||| Each command is followed by spaces followed by it's arguments (if any).
parse : (schema : Schema) -> (input : String) -> Maybe (Command schema)
parse schema input = case substr 0 1 input of
                          "S" => case parseSchema rest of
                                      Nothing => Nothing
                                      Just x => Just $ SetSchema x
                          "A" => case parseBySchema schema rest of
                                      Nothing => Nothing
                                      Just x => Just $ Add x
                          "G" => case rest of 
                                      "" => Just $ GetAll
                                      _ => Just $ Get $ cast rest
                          "Q" => Just $ Quit
                          "C" => Just $ Clear

        where rest : String
              rest = ltrim $ substr 1 (length input) input

display : SchemaType schema -> String
-- pattern match over schema implicit argument to get information about types
-- and how to display them
display {schema = SString} x = "\"" ++ x ++ "\""
display {schema = SInt} x = cast x
display {schema = SBool} x = show x
display {schema = (y .+. z)} (a, b) = display a ++ " " ++ display b

getEntry : (pos : Integer) -> (store : DataStore) ->
           Maybe (String, DataStore)
-- use integerToFin and index to extract data out of the store
getEntry pos store@(MkData schema size items) = case integerToFin pos size of
                                               Nothing => Nothing
                                               (Just x) => Just $ ((display $ index x items) ++ "\n",
                                                                   store)

getAllEntries : (store : DataStore) -> Maybe (String, DataStore)
getAllEntries store@(MkData schema size items) = getEntriesIndex size store
    where getEntriesIndex : Nat -> DataStore -> Maybe (String, DataStore)
          getEntriesIndex (S Z) x = getEntry 0 x
          getEntriesIndex (S k) x = Just (fst other_entries ++ entry, snd other_entries)
            where entry : String
                  entry = fst $ fromMaybe ?no $ getEntry (cast k) x
                  other_entries : (String, DataStore)
                  other_entries = fromMaybe ?no $ getEntriesIndex k x

processInput : DataStore -> String -> Maybe (String, DataStore)
processInput store y = case parse (schema store) y of
                            Nothing => Nothing
                            (Just (SetSchema x)) => (case setSchema store x of
                                                          Nothing => Nothing
                                                          (Just x) => Just $ ("Schema changed.\n", x))
                            (Just (Add x)) => Just ("Successfully added " ++ display x ++ ".\n", 
                                                      addToStore store x)
                            (Just (Get x)) => getEntry x store
                            (Just GetAll) => getAllEntries store
                            (Just Quit) => Nothing
                            (Just Clear) => Just ("Database cleared.\n",
                                            MkData (schema store) _ [])

main : IO ()
main = replWith (MkData (SString .+. SString .+. SInt) _ []) "Command: " processInput
