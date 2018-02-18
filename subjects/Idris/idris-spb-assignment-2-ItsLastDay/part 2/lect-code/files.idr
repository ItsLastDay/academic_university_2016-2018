module Main

import Effects
import Effect.File
import Effect.StdIO
import Effect.System

readFile : Eff (List String) [FILE R]
readFile = readAcc [] where
    readAcc : List String -> Eff (List String) [FILE R]
    readAcc acc = if (not !eof)
                     then do Result s <- readLine
                             readAcc (s :: acc)
                     else pure (reverse acc)

dumpFile : String -> Eff () [FILE (), STDIO]
dumpFile name = do 
     Success <- open name Read | FError err => putStrLn "Ошибка!"
     ss <- readFile
     mapE (\s => putStr s) ss
     close
     
emain : Eff () [FILE (), SYSTEM, STDIO]
emain = do 
  [prog, name] <- getArgs | [] => putStrLn "Can't happen!"
                          | [prog] => putStrLn "No arguments!"
                          | _ => putStrLn "Too many arguments!"
  dumpFile name

main : IO ()
main = run emain
