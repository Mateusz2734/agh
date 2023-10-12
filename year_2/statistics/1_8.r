# Napisz funkcję obliczającą wariancję w próbie (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną var.
# Czy można wykorzystać tę funkcję do obliczenia wariancji dla całej populacji?

std <- function(v) {
  return(sqrt(var(v)))
}
