# Napisz funkcję obliczającą kwartyl dolny (pierwszy) próby (wektor v).
# Porównaj działanie z funkcją biblioteczną fivenum.

q1 <- function(v) {
  v <- sort(v)
  val <- 0

  if (length(v) %% 2 == 0) {
    val <- median(v[1:(length(v) / 2)])
  } else {
    val <- median(v[1:(length(v) / 2 + 1)])
  }
  return(val)
}
