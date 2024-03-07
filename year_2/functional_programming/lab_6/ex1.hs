import Text.XHtml (base)
import Language.Haskell.TH (safe)
import Distribution.Simple.Utils (safeTail)
(<$<) :: (a -> b) -> a -> b
(<$<) = ($)

(>$>) :: a -> (a -> b) -> b
x >$> f = f x
infixl 0 >$>

(<.<) :: (b -> c) -> (a -> b) -> (a -> c)
(<.<) = (.)

(>.>) :: (a -> b) -> (b -> c) -> (a -> c)
f >.> g = g . f
infixl 9 >.>

safeTail' :: [a] -> Maybe [a]
safeTail' [] = Nothing
safeTail' (x:xs) = Just xs