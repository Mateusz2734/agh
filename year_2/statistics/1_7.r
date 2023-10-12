# Napisz funkcję obliczającą wariancję w próbie (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną var.
# Czy można wykorzystać tę funkcję do obliczenia wariancji dla całej populacji?

variance <- function(v) {
  x <- mean(v)
  s <- 0

  for (i in v) {
    s <- s + (i - x)^2
  }

  return(s / (length(v) - 1))
}

