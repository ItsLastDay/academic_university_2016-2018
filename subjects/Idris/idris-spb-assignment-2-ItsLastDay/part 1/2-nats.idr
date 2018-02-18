plus_n_zero : (n : Nat) -> n + 0 = n
plus_n_zero n = rewrite plusCommutative n 0 in Refl

plus_n_succ : (n, k : Nat) -> n + (S k) = S (n + k)
plus_n_succ n k = rewrite sym (plusSuccRightSucc n k) in Refl

plus_assoc : (a, b, c : Nat) -> a + (b + c) = (a + b) + c
plus_assoc Z b c = Refl
plus_assoc (S k) b c = rewrite plus_assoc k b c in Refl

succ_inj : (n, m : Nat) -> S n = S m -> n = m
succ_inj m m Refl = Refl

plus_eq : (n, m, m' : Nat) -> n + m = n + m' -> m = m'
plus_eq Z m m' prf = prf
plus_eq (S k) m m' prf = rewrite plus_eq k m m' (succ_inj _ _ prf) in Refl

plus_right_zero : (n, m : Nat) -> n + m = n -> m = 0
plus_right_zero Z m prf = prf
plus_right_zero (S k) m prf = plus_right_zero k m (succ_inj _ _ prf)

-- if x > y then y <= x
gt__lte : x `GT` y -> y `LTE` x 
gt__lte prop = lteSuccLeft prop

-- if not(x <= y) then (y <= x)
not_lte__lte : Not (x `LTE` y) -> y `LTE` x 
not_lte__lte prop {x} {y} = case isLTE x y of
                                 (Yes prf) => void $ prop prf
                                 (No contra) => not_lte__lte contra
