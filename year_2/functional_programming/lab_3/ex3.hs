sqr x = x ^ 2

funcFactory n = case n of
  1 -> id
  2 -> sqr
  3 -> (^ 3)
  4 -> \x -> x ^ 4
  5 -> intFunc
  _ -> const n
  where
    intFunc x = x ^ 5

expApproxUpTo :: Int -> Double -> Double
expApproxUpTo n = case n of
  0 -> \x -> 1 + x
  1 -> \x -> 1 + x + x ^ 2 / 2
  2 -> \x -> 1 + x + x ^ 2 / 2 + x ^ 3 / 6
  3 -> \x -> 1 + x + x ^ 2 / 2 + x ^ 3 / 6 + x ^ 4 / 24
  4 -> \x -> 1 + x + x ^ 2 / 2 + x ^ 3 / 6 + x ^ 4 / 24 + x ^ 5 / 120
  5 -> \x -> 1 + x + x ^ 2 / 2 + x ^ 3 / 6 + x ^ 4 / 24 + x ^ 5 / 120 + x ^ 6 / 720

factorial 0 = 1
factorial n = product [1..n]

expApproxUpTo' :: Int -> Double -> Double
expApproxUpTo' 0 = \x -> 1 + x
expApproxUpTo' n = \x -> x^(n+1)/factorial (fromIntegral n + 1) + expApproxUpTo' (n - 1) x