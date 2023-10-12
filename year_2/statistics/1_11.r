# Napisz funkcję obliczającą kwartyl górny (trzeci) próby (wektor v).
# Porównaj działanie z funkcją biblioteczną fivenum

q3 <- function(v) {
  n <- length(v)
  v <- sort(v)
  val <- 0

  if (length(v) %% 2 == 0) {
    val <- median(v[(n / 2):n])
  } else {
    val <- median(v[(n / 2 + 1):n])
  }
  return(val)
}
