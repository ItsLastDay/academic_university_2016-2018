import Control.Monad.Except
import Data.Char (isNumber, isPunctuation)


data PwdError = PwdError String

instance Monoid PwdError where
    mempty = PwdError ""
    (PwdError x) `mappend` (PwdError y) = PwdError $ x ++ y

type PwdErrorMonad = ExceptT PwdError IO

askPassword' :: PwdErrorMonad ()
askPassword' = do
  liftIO $ putStrLn "Enter your new password:"
  value <- msum $ repeat getValidPassword'
  liftIO $ putStrLn "Storing in database..."

getValidPassword' :: PwdErrorMonad String
--getValidPassword' = undefined

getValidPassword' = do
    action `catchError` handler where
        handler (PwdError s) = do
            liftIO $ putStrLn $ "Incorrect input: " ++ s
            mzero 
        action = do
            s <- liftIO getLine
            isValidPassword s


short_pass = "password is too short!"
no_digit_pass = "password must contain some digits!"
no_punct_pass = "password must contain some punctuations!"

isValidPassword :: String -> PwdErrorMonad String
isValidPassword s | length s < 8 = throwError $ PwdError short_pass
                  | not $ any isNumber s = throwError $ PwdError no_digit_pass
                  | not $ any isPunctuation s = throwError $ PwdError no_punct_pass
                  | otherwise = return s
