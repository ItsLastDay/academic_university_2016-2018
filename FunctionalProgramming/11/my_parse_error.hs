import Control.Monad.Except
import Control.Monad.Trans.Maybe
import Data.Char (isNumber, isPunctuation)
import Data.Char

data ParseError = ParseError { location :: Int, reason :: String }

type ParseMonad = Either ParseError

parseHex :: String -> ParseMonad Integer
parseHex [] = Left (ParseError 0 "empty input")
parseHex s@(c:res) = foldM advanceHex 0 (zip s [1..]) 
advanceHex :: Integer -> (Char, Int) -> ParseMonad Integer
advanceHex res (ch, pos) | isHexDigit ch = Right $ res * 16 + (toInteger $ digitToInt ch)
                         | otherwise = Left $ ParseError pos (ch:": invalid digit")


printError :: ParseError -> ParseMonad String        
printError (ParseError loc r) = Right $ "At pos " ++ (show loc) ++ ": " ++ r

-- тестирование
test s = str where
  (Right str) = do 
      n <- parseHex s
      return $ show n  
    `catchError` printError
    

