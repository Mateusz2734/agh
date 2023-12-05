data BinIntTree
  = EmptyIntBT
  | IntNodeBT Int BinIntTree BinIntTree

sumBinIntTree :: BinIntTree -> Int
sumBinIntTree EmptyIntBT = 0
sumBinIntTree (IntNodeBT n lt rt) = n + sumBinIntTree lt + sumBinIntTree rt

data BinTree a
  = EmptyBT
  | NodeBT a (BinTree a) (BinTree a)

sumBinTree :: (Num a) => BinTree a -> a
sumBinTree EmptyBT = 0
sumBinTree (NodeBT n lt rt) = n + sumBinTree lt + sumBinTree rt

instance Eq a => Eq (BinTree a) where
  (==) (NodeBT n1 lt1 rt1) (NodeBT n2 lt2 rt2) = n1 == n2 && lt1 == lt2 && rt1 == rt2

data Expr a
  = Lit a -- literal/value a, e.g. Lit 2 = 2
  | Add (Expr a) (Expr a)
  | Subtract (Expr a) (Expr a)
  | Multiply (Expr a) (Expr a)

eval :: (Num a) => Expr a -> a
eval (Lit n) = n
eval (Add e1 e2) = eval e1 + eval e2
eval (Subtract e1 e2) = eval e1 - eval e2
eval (Multiply e1 e2) = eval e1 * eval e2

show' :: (Show a) => Expr a -> String
show' (Lit n) = show n
show' (Add e1 e2) = "(" ++ show' e1 ++ "+" ++ show' e2 ++ ")"
show' (Subtract e1 e2) = "(" ++ show' e1 ++ "-" ++ show' e2 ++ ")"
show' (Multiply e1 e2) = "(" ++ show' e1 ++ "*" ++ show' e2 ++ ")"

depthOfBT :: BinTree a -> Int -- głębokość drzewa binarnego
depthOfBT EmptyBT = 0
depthOfBT (NodeBT n lt rt) = max (1 + depthOfBT lt) (1 + depthOfBT rt)

flattenBTPre :: BinTree a -> [a] -- preorder
flattenBTPre EmptyBT = []
flattenBTPre (NodeBT n lt rt) = [n] ++ flattenBTPre lt ++ flattenBTPre rt

flattenBTIn :: BinTree a -> [a] -- inorder
flattenBTIn EmptyBT = []
flattenBTIn (NodeBT n lt rt) = flattenBTIn lt ++ [n] ++ flattenBTIn rt

flattenBTPost :: BinTree a -> [a] -- postorder
flattenBTPost EmptyBT = []
flattenBTPost (NodeBT n lt rt) = flattenBTPost lt ++ flattenBTPost rt ++ [n]

mapBT :: (a -> b) -> BinTree a -> BinTree b -- funkcja map dla drzewa binarnego
mapBT f EmptyBT = EmptyBT
mapBT f (NodeBT n lt rt) = NodeBT (f n) (mapBT f lt) (mapBT f rt)

insert :: (Ord a) => a -> BinTree a -> BinTree a -- insert element into BinTree
insert x EmptyBT = NodeBT x EmptyBT EmptyBT
insert x (NodeBT n lt rt)
  | x > n = insert x rt
  | otherwise = insert x lt

list2BST :: Ord a => [a] -> BinTree a -- list to Binary Search Tree (BST)
list2BST [] = EmptyBT
list2BST (x:xs) = insert x (list2BST xs)