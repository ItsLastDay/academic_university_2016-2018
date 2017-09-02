data BSTree : (a: Type) -> Type where
     Empty : Ord a => BSTree a
     Node : Ord a => (left : BSTree a) -> (val : a) ->
                        (right : BSTree a) -> BSTree a

%name BSTree t, t1
                        
insert : a -> BSTree a -> BSTree a
insert x Empty = Node Empty x Empty
insert x orig@(Node left val right) = 
     case compare x val of
        LT => Node (insert x left) val right
        EQ => orig
        GT => Node left val (insert x right)

||| Inserts elements of a list into a binary search tree
listToTree : Ord a => List a -> BSTree a 
-- Added Ord a context, because it does not make sense to create BST out of
-- unorderable elements (and it does not compile otherwise).
listToTree [] = Empty
listToTree (x :: xs) = insert x $ listToTree xs

||| Creates list from the elements of BSTree
treeToList : BSTree a -> List a 
treeToList Empty = []
treeToList (Node left val right) = val :: (treeToList left ++ treeToList right)
