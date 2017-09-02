-- Define and implement datatype Ternary, which is analogous to Binary n
-- from lecture 10, covering function and translation to ternary number 
-- representation.


data Ternary : Nat -> Type where
  TEnd : Ternary Z
  TO : Ternary n -> Ternary (n + n + n)
  TI : Ternary n -> Ternary (S (n + n + n))
  TN : Ternary n -> Ternary (S (S (n + n + n)))


data Ternarity : Nat -> Type where
  TZero : Ternarity (n + n + n)
  TOne : Ternarity (S (n + n + n))
  TTwo : Ternarity (S (S (n + n + n)))


ternarity_lemma_1 : (n : Nat) -> Ternarity ((S n) + (S n) + (S n)) -> Ternarity (S (S (S (n + n + n))))
ternarity_lemma_1 n x = rewrite sym $ lemma_1_helper n in x
    where lemma_1_helper : (n : Nat) -> ((S n) + (S n) + (S n)) = (S (S (S (n + n + n)))) 
          lemma_1_helper n = rewrite sym $ plusSuccRightSucc n n in rewrite plusSuccRightSucc (plus n n) n in Refl


ternarity_lemma_2 : (n : Nat) -> Ternarity (S (((S n) + (S n)) + (S n))) -> Ternarity (S (S (S (S (plus (plus n n) n)))))
ternarity_lemma_2 n x = rewrite lemma_2_helper n in x
    where lemma_2_helper : (n : Nat) -> (S (S (S (S (plus (plus n n) n))))) = (S (((S n) + (S n)) + (S n)))
          lemma_2_helper n = rewrite sym $ plusSuccRightSucc n n in rewrite plusSuccRightSucc (plus n n) n in Refl


ternarity_lemma_3 : (n : Nat) -> Ternarity (S (S (((S n) + (S n)) + (S n)))) -> Ternarity (S (S (S (S (S (plus (plus n n) n))))))
ternarity_lemma_3 n x = rewrite lemma_3_helper n in x
    where lemma_3_helper : (n : Nat) -> (S (S (S (S (S (plus (plus n n) n)))))) = (S (S (((S n) + (S n)) + (S n))))
          lemma_3_helper n = rewrite sym $ plusSuccRightSucc n n in rewrite plusSuccRightSucc (plus n n) n in Refl


ternarity : (n : Nat) -> Ternarity n 
ternarity Z = TZero {n=Z}
ternarity (S Z) = TOne {n=Z}
ternarity (S (S Z)) = TTwo {n=Z}
ternarity (S (S (S k))) with (ternarity k)
  ternarity (S (S (S ((n + n) + n)))) | TZero = ternarity_lemma_1 n (TZero {n=(S n)})
  ternarity (S (S (S (S ((n + n) + n))))) | TOne = ternarity_lemma_2 n (TOne {n=(S n)})
  ternarity (S (S (S (S (S ((n + n) + n)))))) | TTwo = ternarity_lemma_3 n (TTwo {n=(S n)})


natToTernary : (n : Nat) -> Ternary n
natToTernary Z = TEnd
natToTernary n with (ternarity n)
  natToTernary ((x + x) + x) | TZero = TO (natToTernary x)
  natToTernary (S ((x + x) + x)) | TOne = TI (natToTernary x)
  natToTernary (S (S ((x + x) + x))) | TTwo = TN (natToTernary x)

