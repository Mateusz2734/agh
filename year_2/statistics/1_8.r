<<<<<<< HEAD
# Napisz funkcję obliczającą odchylenie standardowe w próbie (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną sd.
# Czy można wykorzystać tę funkcję do obliczenia odchylenia standardowego dla całej populacji?
=======
# Napisz funkcję obliczającą wariancję w próbie (zadanej jako wektor v).
# Porównaj działanie z funkcją biblioteczną var.
# Czy można wykorzystać tę funkcję do obliczenia wariancji dla całej populacji?
>>>>>>> 3c63e9c878024ee4907394fcbb8c07c04c20d03a

std <- function(v) {
  return(sqrt(var(v)))
}
