import Data.Vect

-- Use implicit arguments to implement following function
vlength : Vect n a -> Nat
vlength {n} xs = n


-- sum of vectors (define recursively)
vadd : Num a => Vect n a -> Vect n a -> Vect n a
vadd [] [] = []
vadd (x :: xs) (y :: ys) = (x + y) :: vadd xs ys


-- scalar product (use functions zipWith and sum)
vScalarProd : Num a => Vect n a -> Vect n a -> a
vScalarProd xs ys = sum $ zipWith (*) xs ys


-- replace all coordinates that are close to zero with zero
toAxis : (eps : Double) -> Vect n Double -> Vect n Double
toAxis eps xs = map (\x => if abs x < eps then 0 else x) xs


-- increase vector dimension, adding 0 as the first coordinate
incDimFirst : Vect n Double -> Vect (n+1) Double
incDimFirst xs = reverse $ reverse xs ++ [0]


-- increase vector dimension, adding 0 as the last coordinate
incDimLast : Vect n Double -> Vect (n+1) Double
incDimLast [] = [0]
incDimLast (x :: xs) = x :: incDimLast xs


-- project vector to the space given by vector of indices
-- use function `map`
project : Vect n a -> Vect k (Fin n) -> Vect k a
project xs ys = map (\idx => index idx xs) ys


test1 : Bool
test1 = project [1,2,3,4] [FS FZ, FZ, FS (FS FZ)] == [2,1,3]
        && project [1,2,3,4] [FS FZ, FS FZ, FS (FS (FS FZ))] == [2,2,4] 
        && project [0] [FZ, FZ, FZ, FZ] == [0,0,0,0]
--      Following tests don't compile, why? Reasons differ!
--      && project [1,2,3,4] [FS FZ, FS FZ, (FS (FS (FS (FS FZ))))] == [2,2,0] 
--      && project [0] [FZ, FZ, FZ, FZ] == [0,0,0]

-- reverse given vector
reverse' : Vect n a -> Vect n a
reverse' [] = []
reverse' (x :: xs) = insLast (reverse xs) x
    where insLast : Vect n a -> a -> Vect (S n) a
          insLast [] x = [x]
          insLast (y :: xs) x = y :: insLast xs x

-- matrix transposition
transpose_mat : Vect m (Vect n elem) -> Vect n (Vect m elem)
-- use zipWith
transpose_mat {n = n} {elem = elem} [] = replicate n (the (Vect 0 elem) [])
transpose_mat {n = n} {elem = elem} (x :: xs) = zipWith (::) x $ transpose_mat xs

-- matrix addition and multiplication
addMatrix : Num numType => Vect rows (Vect cols numType) 
                           -> Vect rows (Vect cols numType) 
                           -> Vect rows (Vect cols numType)
addMatrix [] [] = []
addMatrix (x :: xs) (y :: ys) = vadd x y :: addMatrix xs ys



multMatrix : Num numType => Vect n (Vect m numType) 
                            -> Vect m (Vect p numType) 
                            -> Vect n (Vect p numType)
multMatrix xs ys = mulMatrixTrans xs (transpose_mat ys)
    where mulMatrixTrans : Num numType => Vect n (Vect m numType)
                                          -> Vect p (Vect m numType)
                                          -> Vect n (Vect p numType)
          mulMatrixTrans [] ys = []
          mulMatrixTrans (x :: xs) ys = map (vScalarProd x) ys :: mulMatrixTrans xs ys
