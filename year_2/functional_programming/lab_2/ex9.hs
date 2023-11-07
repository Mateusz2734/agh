qSort :: (Ord a) => [a] -> [a]
qSort [] = []
qSort (x : xs) = qSort (leftPart xs) ++ [x] ++ qSort (rightPart xs)
  where
    leftPart = filter (<= x)
    rightPart = filter (> x)