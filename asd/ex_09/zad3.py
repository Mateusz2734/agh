# Mamy dany zbiór zadań T = {t1 , ..., tn}. Każde zadanie t[i] dodatkowo posiada:
# (a) termin wykonania d(t[i]) (liczba naturalna) oraz 
# (b) zysk g(t[i]) za wykonanie w terminie (liczba naturalna).

# Wykonanie każdego zadania trwa jednostkę czasu. Jeśli
# zadanie t[i] zostanie wykonane przed przekroczeniem swojego terminu d(t[i]), to dostajemy
# za nie nagrodę g(t[i]) (pierwsze wybrane zadanie jest wykonywane w chwili 0, drugie wybrane
# zadanie w chwili 1, trzecie w chwili 2, itd.).
# Proszę podać algorytm, który znajduje podzbiór zadań, które można wykonać w terminie i który
# prowadzi do maksymalnego zysku.

# Zaczynamy od końca, czyli od najpóźniejszych zadań. Wybieramy najbardziej wartościowe zadanie