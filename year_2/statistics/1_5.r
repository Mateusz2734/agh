# Napisz funkcję obliczającą medianę próby (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną median.

median1 <- function(v) {
  v <- sort(v)
  n <- length(v)
  if (n %% 2 == 0) {
    return((v[n / 2] + v[n / 2 + 1]) / 2)
  } else {
    return(v[(n + 1) / 2])
  }
}
