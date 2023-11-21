import Data.List

funcList :: [Double -> Double]
funcList = [\x -> (sin x) / x, \x -> log x + sqrt x + 1, \x -> (exp 1) ** x]

evalFuncListAt :: a -> [a -> b] -> [b]
evalFuncListAt x [] = []
evalFuncListAt x (f : fs) = f x : evalFuncListAt x fs

displEqs :: (Double -> Double, Double -> Double)
displEqs = (\t -> 4 * t ^ 2 + 2 * t, \t -> 3 * t ^ 2)

sortDesc :: Ord a => [a] -> [a]
sortDesc xs = (reverse . sort) xs

are2FunsEqAt :: Eq a => (t -> a) -> (t -> a) -> [t] -> Bool
are2FunsEqAt _ _ [] = True
are2FunsEqAt f g (x:xs) = f x == g x && are2FunsEqAt f g xs  