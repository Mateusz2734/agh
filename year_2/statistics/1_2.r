# Napisz kilka wariantów funkcji obliczającej średnią z próby (zadanej jako wektor v).
# Kolejne warianty mogą wykorzystywać np. różne rodzaje pętli (for, while, repeat).
# Porównaj działanie z funkcją biblioteczną mean.

mean1 <- function(v) {
  return(sum(v) / length(v))
}

mean2 <- function(v) {
  s <- 0
  for (i in v) {
    s <- s + i
  }
  return(s / length(v))
}

mean3 <- function(v) {
  s <- 0
  i <- 1
  while (i <= length(v)) {
    s <- s + v[i]
    i <- i + 1
  }
  return(s / length(v))
}

mean4 <- function(v) {
  s <- 0
  for (i in seq_along(v)) {
    s <- s + v[i]
  }
  return(s / length(v))
}

mean5 <- function(v) {
  s <- 0
  i <- 1
  repeat {
    s <- s + v[i]
    i <- i + 1
    if (i > length(v)) {
      break
    }
  }
  return(s / length(v))
}