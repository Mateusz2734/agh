# Napisz funkcję obliczającą odchylenie standardowe w próbie (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną sd.
# Czy można wykorzystać tę funkcję do obliczenia odchylenia standardowego dla całej populacji?

std <- function(v) {
  return(sqrt(var(v)))
}
