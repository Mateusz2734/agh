isSortedAsc :: Ord a => [a] -> Bool
isSortedAsc xs = and (zipWith (<=) xs (tail xs))

everySecond :: [t] -> [t]
everySecond [] = []
everySecond (x1:x2:xs) = x1 : everySecond xs
everySecond x = x