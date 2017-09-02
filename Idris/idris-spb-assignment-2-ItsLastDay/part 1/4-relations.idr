import Data.Vect

%name Elem el, el1, el2, el3, el4

-- find an index of an element in the vector
vpos : (v : Vect n a) -> Elem x v -> Fin n
vpos (x :: xs) Here = FZ
vpos (y :: xs) (There later) = FS (vpos xs later)

vreplace : (v : Vect n a) -> Elem x v -> (y : a) -> (v : Vect n a ** Elem y v)
vreplace {x = x} (x :: xs) Here y = (y :: xs ** Here)
vreplace {x = x} (z :: xs) (There later) y = (z :: (fst $ vreplace xs later y) ** There $ snd $ vreplace xs later y)

vswap : (v1 : Vect n1 t ** Elem x v1) -> (v2 : Vect n2 t ** Elem y v2) ->
             ((v1':Vect n1 t ** Elem y v1'), (v2' : Vect n2 t ** Elem x v2'))


-- relation over vector, natural number and number of occurences
data Occ : (x : Nat) -> (n : Nat) -> (xs : Vect m Nat) -> Type where
  OccNil : Occ x 0 Nil
  OccNext : Not (x=y) -> Occ x n xs -> Occ x n (y :: xs)
  OccMore : (x = y) -> Occ x n xs -> Occ x (S n) (y :: xs)


mkOcc : (x : Nat) -> (xs : Vect m Nat) -> (n : Nat ** Occ x n xs)
mkOcc x [] = (0 ** OccNil)
mkOcc x (y :: xs) = case decEq x y of
                         (Yes prf) => (_ ** OccMore prf $ snd $ mkOcc x xs)
                         (No contra) => (_ ** OccNext contra $ snd $ mkOcc x xs)

-- relation list and its last element
data Last : List a -> a -> Type where
  LastOne : Last [value] value
  LastCons : (prf : Last xs value) -> Last (x :: xs) value

last123 : Last [1,2,3] 3
last123 = LastCons (LastCons LastOne)

lastEmpty : Last [] value -> Void
lastEmpty LastOne impossible
lastEmpty (LastCons _) impossible

lastNotEqual : (contra : (x = value) -> Void) -> Last [x] value -> Void
lastNotEqual contra LastOne = contra Refl
lastNotEqual contra (LastCons prf) impossible

lastNotEqualFull : (contra : Last (y :: xs) value -> Void) -> Last (x :: (y :: xs)) value -> Void
lastNotEqualFull contra (LastCons LastOne) = contra LastOne
lastNotEqualFull contra (LastCons (LastCons prf)) = contra (LastCons prf)

isLast : DecEq a => (xs : List a) -> (value : a) -> Dec (Last xs value)
isLast [] value = No lastEmpty
isLast [x] value = case decEq x value of
                        (Yes Refl) => Yes LastOne
                        (No contra) => No (lastNotEqual contra)
isLast (x :: y :: xs) value = case isLast (y :: xs) value of
                                   (Yes prf) => Yes (LastCons prf)
                                   (No contra) => No (lastNotEqualFull contra) 


-- define relation over lists which is analogous to Elem
-- and implement its decision procedure
data LElem : List a -> a -> Type where
  LElemHead : LElem (x :: xs) x
  LElemCons : (prf : LElem xs val) -> LElem (x :: xs) val


isElemEmpty : LElem [] value -> Void
isElemEmpty LElemHead impossible
isElemEmpty (LElemCons _) impossible

isElemNotEqual : (contra2 : LElem xs value -> Void) -> (contra : (x = value) -> Void) -> LElem (x :: xs) value -> Void
isElemNotEqual contra2 contra LElemHead = contra Refl
isElemNotEqual contra2 contra (LElemCons prf) = contra2 prf

isLElem : DecEq a => (xs : List a) -> (value : a) -> Dec (LElem xs value)
isLElem [] value = No isElemEmpty
isLElem (x :: xs) value = case decEq x value of
                               (Yes Refl) => Yes LElemHead
                               (No contra) => (case isLElem xs value of
                                                    (Yes prf) => Yes $ LElemCons prf
                                                    (No contra2) => No (isElemNotEqual contra2 contra))
