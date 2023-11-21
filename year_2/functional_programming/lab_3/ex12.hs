import Data.Char
import Data.List

capitalize :: [Char] -> [Char]
capitalize [] = []
capitalize (x : xs) = toUpper x : map toLower xs

formatStr s =
  foldl1 (\w s -> w ++ " " ++ s)
    . map capitalize
    . filter (\x -> length x > 1)
    $ words s

prodPrices p = case p of
  "A" -> 100
  "B" -> 500
  "C" -> 1000
  _ -> error "Unknown product"

products = ["A", "B", "C"]

discStr1 p
  | price > 999 = 0.3 * price
  | otherwise = 0.1 * price
  where
    price = prodPrices p

discStr2 p = 0.2 * prodPrices p

totalDiscount discStr = 
  scanl1 (+) .
  map discStr .
  filter (\p -> prodPrices p > 500)