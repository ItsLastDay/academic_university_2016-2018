-- Do not import Data.Vect

data Vect : Nat -> Type -> Type where
     Nil  : Vect Z a
     (::) : a -> Vect k a -> Vect (S k) a

%name Vect xs, ys, zs

{- 1 -}

reverseProof_nil : (xs : Vect n a) -> (acc : Vect n1 a) -> Vect n1 a -> Vect (plus n1 0) a
reverseProof_nil xs acc ys {n1} = rewrite plusCommutative n1 0 in ys

reverseProof_xs : Vect ((S n1) + k) a -> Vect (plus n1 (S k)) a
reverseProof_xs xs {n1} {k} = rewrite sym $ plusSuccRightSucc n1 k in xs

my_reverse : Vect n a -> Vect n a
my_reverse xs = reverse' [] xs
  where reverse' : Vect n a -> Vect m a -> Vect (n+m) a
        reverse' acc [] = reverseProof_nil xs acc acc
        reverse' acc (x :: xs)
                        = reverseProof_xs (reverse' (x::acc) xs)

{- 2 -}

head_unequal : DecEq a => {xs : Vect n a} -> {ys : Vect n a} ->
           (contra : (x = y) -> Void) -> (x :: xs) = (y :: ys) -> Void
head_unequal {x = x} {y = x} contra Refl = contra Refl

tail_unequal : DecEq a => {xs : Vect n a} -> {ys : Vect n a} ->
          (contra : (xs = ys) -> Void) -> (x :: xs) = (y :: ys) -> Void
tail_unequal {x = x} {y = x} contra Refl = contra Refl


{- 3 -}

DecEq a => DecEq (Vect n a) where
  decEq [] [] = Yes Refl
  decEq (x :: xs) (y :: ys) = case decEq x y of
                                (Yes Refl) => (case decEq xs ys of
                                                   (Yes Refl) => Yes Refl
                                                   (No contra) => No $ tail_unequal contra)
                                (No contra) => No $ head_unequal contra
