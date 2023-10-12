# Napisz funkcję obliczającą odchylenie przeciętne od wartości średniej dla
# próby (zadanej jako wektor v).

ad <- function(v) {
  x <- mean(v)
  s <- 0

  for (xi in v) {
    s <- s + abs(xi - x)
  }
  
  return(s / length(v))
}
