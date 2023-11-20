sum' :: (Num a) => [a] -> a
sum' [] = 0
sum' (x : xs) = x + sum' xs

sumSqr' :: (Num a) => [a] -> a
sumSqr' [] = 0
sumSqr' (x : xs) = x ^ 2 + sumSqr' xs

sumWith :: (Num a) => (a -> a) -> [a] -> a
sumWith _ [] = 0
sumWith f (x : xs) = f x + sumWith f xs
