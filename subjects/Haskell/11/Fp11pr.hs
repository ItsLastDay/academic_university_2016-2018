{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE UndecidableInstances #-}

module Fp11pr where

import Control.Monad
import Control.Monad.Trans

import Control.Monad.Identity
import Control.Monad.State
import Control.Monad.Reader

import Control.Monad.IO.Class


{-
Сделайте на основе типа данных
-}

data Logged a = Logged String a deriving (Eq,Show)

{-
трансформер монад \lstinline!LoggT :: (* -> *) -> * -> *! с одноименным 
конструктором данных и меткой поля \lstinline!runLoggT!)
-}

newtype LoggT m a = LoggT { runLoggT :: m (Logged a) } 


{-
Реализуйте для произвольной монады 
\lstinline!m! представителя класса типов \lstinline!Monad! 
для \lstinline!LoggT m :: * -> *!. 
-}

instance Monad m => Monad (LoggT m) where
--  fail :: String -> LoggT m a
  fail msg  = LoggT $ fail msg

--  return :: a -> LoggT m a
  return  x = LoggT $ return $ Logged "" x 

-- >>= :: LoggT m a -> (a -> LoggT m b) -> LoggT m b
  m >>= k   = LoggT $ do
    Logged lg x <- runLoggT m
    Logged nxt_lg y <- runLoggT $ k x
    return $ Logged (lg ++ nxt_lg) y -- вычисления во внутренней монаде!
     

instance Monad m => Functor (LoggT m) where
  fmap  = undefined
instance Monad m => Applicative (LoggT m) where
  pure = undefined
  (<*>) = undefined

-- тестируем

logTst :: LoggT Identity Integer
logTst = do 
  x <- LoggT $ Identity $ Logged "AAA" 30
  y <- return 10
  z <- LoggT $ Identity $ Logged "BBB" 2
  return $ x + y + z
  
{-
> runIdentity (runLoggT logTst)
Logged "AAABBB" 42
-}  

failTst :: [Integer] -> LoggT [] Integer
failTst xs = do
  5 <- LoggT $ fmap (Logged "") xs
  LoggT [Logged "A" ()]
  return 42
  
{-
> runLoggT $ failTst [5,5]
[Logged "A" 42,Logged "A" 42]
> runLoggT $ failTst [5,6]
[Logged "A" 42]
> runLoggT $ failTst [7,6]
[]
-}  

{-
Напишите функцию 
\lstinline!write2log! обеспечивающую трансформер \lstinline!LoggT! 
стандартным логгирующим интерфейсом:
-}
 
write2log :: Monad m => String -> LoggT m ()
write2log s = LoggT $ return $ Logged s ()
 
{-
Эта функция позволяет пользователю осуществлять запись в лог в процессе вычисления 
в монаде \lstinline!LoggT m! для любой монады \lstinline!m!.
Введите для удобства упаковку для \lstinline!LoggT Identity!
и напишите функцию запускающую вычисления в этой монаде
-}

type Logg = LoggT Identity

runLogg :: Logg a -> Logged a
runLogg = runIdentity . runLoggT

logTst' :: Logg Integer   
logTst' = do 
  write2log "AAA"
  write2log "BBB"
  return 42
  
{-
> runLogg logTst'
Logged "AAABBB" 42
-}

stLog :: StateT Integer Logg Integer    -- Logg это LoggT Identity
stLog = do 
  modify (+1)
  a <- get
  lift $ write2log $ show $ a * 10
  put 42
  return $ a * 100

{-
> runLogg $ runStateT stLog 2
Logged "30" (300,42)
-}


{-
В последнем примере функция 
\lstinline!lift :: (MonadTrans t, Monad m) => m a -> t m a! 
позволяла поднять вычисление из
внутренней монады (в примере это был \lstinline!Logg!) во
внешний трансформер (\lstinline!StateT Integer!).
Это возможно, поскольку для трансформера \lstinline!StateT s!
реализован представитель класса типов \lstinline!MonadTrans!.
Сделайте трансформер \lstinline!LoggT! представителем
класса \lstinline!MonadTrans!, так чтобы 
можно было поднимать вычисления из произвольной внутренней 
монады в наш трансформер:
-}

instance MonadTrans LoggT where
  lift x = LoggT $ fmap (Logged "") x

logSt :: LoggT (State Integer) Integer  -- State Integer это StateT Integer Identity
logSt = do 
  lift $ modify (+1)
  a <- lift get
  write2log $ show $ a * 10
  lift $ put 42
  return $ a * 100


{-
> runIdentity $ runStateT (runLoggT logSt) 2
(Logged "30" 300,42)
> runState (runLoggT logSt) 2
(Logged "30" 300,42)
-}



{-
Избавьтесь от необходимости 
ручного подъема операций вложенной монады \lstinline!State! , 
сделав трансформер \lstinline!LoggT!, 
примененный к монаде с интерфейсом \lstinline!MonadState!, 
представителем этого (\lstinline!MonadState!) класса типов:
-}

instance MonadState s m => MonadState s (LoggT m) where
  get =   lift $ get
  put =   lift . put


logSt' :: LoggT (State Integer) Integer      
logSt' = do 
  modify (+1)                   -- no lift!
  a <- get                      -- no lift!
  write2log $ show $ a * 10
  put 42                        -- no lift!
  return $ a * 100

{-
> runState (runLoggT logSt') 2
(Logged "30" 300,42)
-}



{-
Избавьтесь от необходимости 
ручного подъема операций вложенной монады \lstinline!Reader! , 
сделав монаду \lstinline!LoggT m! представителем 
класса типов \lstinline!MonadReader!:
-}

instance MonadReader r m => MonadReader r (LoggT m) where
  ask = lift $ ask
  local f lg = mapLoggT (local f) lg
    
{-
Для упрощения реализации функции \lstinline!local!  
имеет смысл использовать вспомогательную
функцию, поднимающую стрелку между двумя 
"внутренними представлениями" трансформера \lstinline!LoggT! 
в стрелку между двумя \lstinline!LoggT!:
-}

mapLoggT :: (m (Logged a) -> n (Logged b)) -> LoggT m a -> LoggT n b
mapLoggT f x = LoggT $ f $ runLoggT x

logRdr :: LoggT (Reader [(Int,String)]) ()      
logRdr = do 
  Just x <- asks $ lookup 2                      -- no lift!
  write2log x
  Just y <- local ((3,"Jim"):) $ asks $ lookup 3 -- no lift!
  write2log y

{-
> runReader (runLoggT logRdr) [(1,"John"),(2,"Jane")]
Logged "JaneJim" ()
-}


{-
Чтобы избавится от необходимости ручного подъема операции
\lstinline!write2log! вложенного трансформера \lstinline!LoggT!, 
можно поступить по аналогии с другими трансформерами 
библиотеки \lstinline!mtl!.
А именно, разработать класс типов \lstinline!MonadLogg!, 
выставляющий стандартный интерфейс для нашего логгера:
-}

class Monad m => MonadLogg m where
  w2log :: String -> m ()
  logg :: Logged a -> m a

{-
(Мы переименовываем функцию \lstinline!write2log! в \lstinline!w2log!,
поскольку хотим держать всю реализацию в одном файле исходного кода. 
При следовании принятой в библиотеках transformers/mtl идеологии 
они имели бы одно и то же имя,
но были бы определены в разных модулях. При работе с transformers мы 
импортировали бы свободную функцию c квалифицированным именем
\lstinline!Control.Monad.Trans.Logg.write2log!, а при использовании 
mtl работа ли бы с методом класса типов \lstinline!Control.Monad.Logg.write2log!)

Во-первых, этот интерфейс должен выставлять 
сам трансформер \lstinline!LoggT!,
обернутый вокруг произвольной монады:
-}

instance Monad m => MonadLogg (LoggT m) where
  w2log = write2log
  logg  = LoggT . return

{-
Реализуйте этого представителя, для проверки используйте
-}
  
logSt'' :: LoggT (State Integer) Integer      
logSt'' = do 
  x <- logg $ Logged "BEGIN " 1   -- use logg
  modify (+x)
  a <- get
  w2log $ show $ a * 10           -- use w2log
  put 42
  w2log " END"                    -- use w2log
  return $ a * 100

  
{-
> runState (runLoggT logSt'') 2
(Logged "BEGIN 30 END" 300,42)
-}
  

{-
Во-вторых, интерфейс MonadLogg должн выставлять любой стандартный трансформер, обернутый вокруг монады,
выставляющий этот интерфейс:
-}
 
instance MonadLogg m => MonadLogg (StateT s m) where
  w2log = lift . w2log
  logg  = lift . logg

instance MonadLogg m => MonadLogg (ReaderT r m) where
  w2log = lift . w2log
  logg  = lift . logg
  
-- etc...


rdrStLog :: ReaderT Integer (StateT Integer Logg) Integer      
rdrStLog = do 
  x <- logg $ Logged "BEGIN " 1
  y <- ask
  modify (+ (x+y))
  a <- get
  w2log $ show $ a * 10      -- no lifts !!!
  put 42
  w2log " END"               -- no lifts !!!
  return $ a * 100

{-
> runLogg $ runStateT (runReaderT rdrStLog 4) 2
Logged "BEGIN 70 END" (700,42)
-}

{-
Если трансформер требует операций ввода-вывода, то в качестве его основы используют не
Identity, а IO. Для подъема операций ввода-вывода во внешние трансформеры 
используют специальный класс типов из модуля Control.Monad.IO.Class

class (Monad m) => MonadIO m where
    liftIO :: IO a -> m a

instance MonadIO IO where
    liftIO = id

Сделайте монаду LoggT представителем этого класса типов, если внутренняя монада 
m выставляет этот интерфейс:

-}

instance MonadIO m => MonadIO (LoggT m) where
  liftIO = undefined


logIO :: LoggT IO ()    
logIO = do
  x <- liftIO getLine
  w2log x

{-
> runLoggT logIO
Simon Peyton Jones
Logged "Simon Peyton Jones" ()
-}


{-
Тип \lstinline!Logged! является 
<<внутренним представлением>> для \lstinline!LoggT!, 
так же как тип \lstinline!Either e! для \lstinline!ExceptT e! 
или частично примененная пара для \lstinline!WriterT w!.
Сам по себе тип \lstinline!Logged! является монадой 
(так же как \lstinline!Either e! или пара). 
Вы реализовывали эту монаду на прошлой практике
-}

instance Monad Logged where
  return  = undefined
  m >>= k = undefined

instance Functor Logged where
  fmap  = undefined
instance Applicative Logged where
  pure  = undefined
  (<*>) = undefined

  
{-
Сделайте теперь монаду \lstinline!Logged! 
представителем класса типов \lstinline!MonadLogg!
-}  

instance MonadLogg Logged where
  w2log = undefined
  logg  = undefined

loggedTst :: Logged Integer
loggedTst = do
  w2log "AAA"             -- use w2log
  logg $ Logged "BBB" 42  -- use logg
  
{-
> loggedTst
Logged "AAABBB" 42
-}
