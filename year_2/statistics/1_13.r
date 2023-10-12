# Napisz funkcję obliczającą rozstęp międzykwartylowy próby (wektor v).
# Porównaj działanie z funkcją biblioteczną IQR.

iqr <- function(v) {
  return(fivenum(v)[4] - fivenum(v)[2])
}
