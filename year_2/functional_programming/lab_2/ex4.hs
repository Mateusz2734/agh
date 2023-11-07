import Data.List

isPalindrome :: [Char] -> Bool
isPalindrome [] = True -- isPalindrome "ABBA" = True
isPalindrome [x] = True
isPalindrome xs
  | head xs == last xs = isPalindrome (tail (init xs))
  | otherwise = False
