fst2Eq :: (Eq a) => [a] -> Bool
fst2Eq (x : y : _) | x == y = True
fst2Eq _ = False

fst2Mod :: (Integral a) => [a] -> Bool
fst2Mod (x : y : _) | y `mod` x == 0 = True
fst2Mod _ = False
