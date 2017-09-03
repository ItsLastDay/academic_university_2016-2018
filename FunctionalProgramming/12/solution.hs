{-# LANGUAGE FlexibleContexts #-} 

import Data.List
import Data.Foldable

import Control.Monad.Except
import Control.Monad.State



infixl 2 :@
infixr 3 :->

type Symb = String 

-- Терм
data Expr = 
  Var Symb 
  | Expr :@ Expr
  | Lam Symb Expr
     deriving (Eq,Show)

-- Тип
data Type = 
  TVar Symb 
  | Type :-> Type
    deriving (Eq,Show)

-- Контекст
newtype Env = Env [(Symb,Type)]
  deriving (Eq,Show)

-- Подстановка
newtype SubsTy = SubsTy [(Symb, Type)]
  deriving (Eq,Show)



freeVars :: Expr -> [Symb] 
freeVars (Var s) = [s]
freeVars (a :@ b) = union (freeVars a) (freeVars b)
freeVars (Lam v e) = delete v (freeVars e)


freeTVars :: Type -> [Symb]
freeTVars (TVar s) = [s]
freeTVars (a :-> b) = union (freeTVars a) (freeTVars b)


extendEnv :: Env -> Symb -> Type -> Env
extendEnv (Env env) v t = Env $ (v, t):env

freeTVarsEnv :: Env -> [Symb]
freeTVarsEnv (Env env) = foldl union [] [ freeTVars (snd x) | x <- env ]


appEnv :: (MonadError String m) => Env -> Symb -> m Type
appEnv (Env xs) v = case lookup v xs of
    Nothing -> throwError $ "There is no variable " ++ (show v) ++ " in the enviroment." 
    Just x -> return x 


appSubsTy :: SubsTy -> Type -> Type
appSubsTy (SubsTy sub) (TVar x) = case lookup x sub of
    Nothing -> TVar x
    Just g -> g
appSubsTy sub (a :-> b) = (appSubsTy sub a) :-> (appSubsTy sub b)


appSubsEnv :: SubsTy -> Env -> Env
appSubsEnv sub (Env env) = Env $ [ (x, appSubsTy sub y) | (x, y) <- env ]

composeSubsTy :: SubsTy -> SubsTy -> SubsTy
composeSubsTy (SubsTy t) (SubsTy s) = SubsTy $ [ el | el@(x, y) <- t, (lookup x s) == Nothing ]
    ++ [ (x, appSubsTy (SubsTy t) y) | (x, y) <- s ]

instance Monoid SubsTy where
    mempty = SubsTy []
    mappend = composeSubsTy


unify :: (MonadError String m) => Type -> Type -> m SubsTy
unify (TVar s) (TVar b) | s == b = return $ SubsTy []
unify (TVar s) b | elem s (freeTVars b) = throwError $ "Can't unify (" ++
    (show (TVar s)) ++ ") with (" ++ (show b) ++ ")!"
                 | otherwise = return $ SubsTy [(s, b)]
unify x@(a :-> b) y@(TVar _)  = unify y x
unify x@(a :-> b) y@(c :-> d) = do
        u2 <- unify b d
        u1 <- unify (appSubsTy u2 a) (appSubsTy u2 c) 
        return $ u1 `mappend` u2 


equations :: (MonadError String m) => Env -> Expr -> Type -> m [(Type,Type)]
equations a b c = evalStateT (equations' a b c) 0

getFreshVar :: (MonadError String m) => StateT Integer m Type
getFreshVar = do
    modify (+ 1)
    num <- get
    return $ TVar $  "xapoier" ++ (show num)

equations' :: (MonadError String m) => Env -> Expr -> Type -> StateT Integer m [(Type, Type)]
equations' env (Var x) tp = do
    new_tp <- appEnv env x
    return $ [(tp, new_tp)] 
equations' env (t1 :@ t2) tp = do
    alpha <- getFreshVar
    eq1 <- equations' env t1 (alpha :-> tp)
    eq2 <- equations' env t2 alpha
    return $ eq1 ++ eq2
equations' env (Lam x m) tp = do
    alpha <- getFreshVar
    beta <- getFreshVar
    eq <- equations' (extendEnv env x alpha) m beta 
    return $ (alpha :-> beta, tp):eq

unify_eqs :: (MonadError String m) => [(Type, Type)] -> m SubsTy -> m SubsTy
unify_eqs [] res = res
unify_eqs ((t1, t2):ts) res = do
    g <- res
    t <- unify (appSubsTy g t1) (appSubsTy g t2)
    unify_eqs ts (return $ t `mappend` g)

principlePair :: (MonadError String m) =>  Expr -> m (Env,Type)
principlePair term = do
    let fv = freeVars term 
    let init_env = Env $ [(x, TVar $ 'a':(show num)) | (x, num) <- zip fv [1..]]
    let beta = TVar "beta"
    eqs <- equations init_env term beta
    unifier <- unify_eqs eqs (pure $ SubsTy [])
    let ans_type = appSubsTy unifier beta
    let ans_env = appSubsEnv unifier init_env 
    return $ (ans_env, ans_type)


