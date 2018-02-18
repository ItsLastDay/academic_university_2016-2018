module Main

import Effects
import Effect.StdIO
import Effect.System
import Effect.State
import Effect.File
import Effect.Exception


data EErr = WrongNumber | BrokenPolishForm 

Stack : Type
Stack = List Int

rstrip : String -> String
rstrip x = pack $ reverse $ rstripInner $ reverse $ unpack x
    where rstripInner : List Char -> List Char
          rstripInner [] = []
          rstripInner (y :: xs) = case isSpace y of
                                       False => (y :: xs)
                                       True => rstripInner xs


readFile : Eff (List String) [FILE R]
readFile = readAcc [] where
    readAcc : List String -> Eff (List String) [FILE R]
    readAcc acc = if (not !eof)
                     then do Result s <- readLine
                             let stripped = rstrip s
                             if (length stripped) > 0 
                                then readAcc (stripped :: acc)
                                else readAcc acc
                     else pure (reverse acc)

push : Int -> Eff () [STATE Stack]
push a = put $ a :: !get

pop : Eff Int [STATE Stack, EXCEPTION EErr]
pop = do
  (x :: xs) <- get | [] => raise BrokenPolishForm
  put xs
  pure x

top : Eff Int [STATE Stack, EXCEPTION EErr]
top = do 
  (x :: _) <- get | [] => raise BrokenPolishForm
  pure x

tops : (Int -> Int -> Int) -> Eff () [STATE Stack, EXCEPTION EErr]
tops op = push $ !pop `op` !pop


step : String -> Eff () [STATE Stack, EXCEPTION EErr]
step "+" = tops (+)
step "*" = tops (*)
step n   = if all isDigit (unpack n)
              then push $ cast n
              else raise WrongNumber

evalRPN : String -> Eff Int [EXCEPTION EErr]
evalRPN s = case run $ evalRPNInner s of
        Right val => pure val
        Left exc => raise exc
    where evalRPNInner : String -> Eff Int [STATE Stack, EXCEPTION EErr]
          evalRPNInner s = do
              mapE (\s => step s) (words s)
              res <- pop
              if (length !get) > 0
                 then raise BrokenPolishForm
                 else pure res


rewritePolish : List String -> List String
rewritePolish [] = []
rewritePolish (x :: xs) = case run $ evalRPN x of
                               Right val => (x ++ " === " ++ (cast val)) :: rest
                               Left _ => (x ++ " ERROR") :: rest
    where rest : List String
          rest = rewritePolish xs


polishMain : Eff () [STDIO, SYSTEM, FILE ()]
polishMain = do 
  [prog, fileName] <- getArgs | [] => putStrLn "Can't happen!"
                          | [prog] => putStrLn "No arguments!"
                          | _ => putStrLn "Too many arguments!"

  Success <- open fileName Read | FError err => putStrLn "Error reading file!"
  lines <- readFile
  close

  let rewrittenFile = rewritePolish $ lines 

  Success <- open fileName WriteTruncate | FError err => putStrLn "Error writing file!"
  mapE (\s => writeLine (s ++ "\n")) rewrittenFile
  close


main : IO ()
main = run polishMain 
