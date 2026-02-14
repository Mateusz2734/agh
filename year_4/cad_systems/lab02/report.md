# Systemy CAD/CAE - Zadanie 2

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Zmiany wprowadzone do stworzenia funkcji `bspline_curve3D`

![git diff](imgs/diff.png)

## 2. Rysowanie inicjałów

### 2.1 Wywołania funkcji

```matlab
knots = [0 0 0 1 1 2 2 3 3 4 4 4]
points1 = [0 0 0; 1 0 1; 1 0 1; 2 0 0.5; 2 0 0.5; 3 0 1; 3 0 1; 4 0 0; 4 0 0]
points2 = [0 0 1; 1 0 0; 1 0 0; 2 0 0.5; 2 0 0.5; 3 0 0; 3 0 0; 4 0 1; 4 0 1]

bspline_curve3D(100, knots, points1) % M
bspline_curve3D(100, knots, points2) % W
```

### 2.2 Wyniki

![Litera M](imgs/initial_1.png)
![Litera W](imgs/initial_2.png)
