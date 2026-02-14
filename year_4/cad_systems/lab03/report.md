# Systemy CAD/CAE - Zadanie 3

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Zmiany wprowadzone do stworzenia funkcji `spline2D_comp`

![git diff](imgs/diff.png)

## 2. Rysowanie labiryntu

### 2.1 Parametry

Wektory węzłów:

```matlab
knot_vectorx =  [0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 42]
knot_vectory =  [0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 42]
```

Współczynniki:

![Współczynniki](imgs/params.png)

### 2.2 Wygenerowany labirynt

![Labirynt](imgs/maze.png)

---

## 3. Dodatkowe modyfikacje

### 3.1 Generowanie labiryntu

Kod do generowania labiryntu, wzorowany na kodzie z [linku](https://rosettacode.org/wiki/Maze_generation#Python).

```python
from random import shuffle, randrange

def make_maze(w = 16, h = 8):
    w //= 2
    h //= 2
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [["10"] * w + ['1'] for _ in range(h)] + [[]]
    hor = [["11"] * w + ['1'] for _ in range(h + 1)]

    def walk(x, y):
        vis[y][x] = 1

        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: hor[max(y, yy)][x] = "10"
            if yy == y: ver[y][max(x, xx)] = "00"
            walk(xx, yy)

    walk(randrange(w), randrange(h))

    s = "1"*(2*w+3) + "\n"
    
    for (a, b) in zip(hor, ver):
        if len(a) > 0 and len(b) > 0:
            s += ''.join(["1"] + a + ["1"] + ['\n'] + ["1"] + b + ["1"] + ['\n'])
    s += "1"*(2*w+3) + "\n"
    s += "1"*(2*w+3) + "\n"
    return s


if __name__ == "__main__":
    lines = [line for line in make_maze(40, 40).split("\n") if line.strip()]

    num_lines = len(lines)
    num_cols = len(lines[0])

    knots_x = [0] + list(range(num_cols)) + [num_cols-1]
    knots_y = [0] + list(range(num_lines)) + [num_lines-1]

    print("knot_vectorx = ", knots_x)
    print("knot_vectory = ", knots_y)
    
    print("coeffs = [")
    for line in lines:
        print("    " + " ".join(list(map(lambda x: str(int(x)), line.strip()))) + ";")
    print("]")
```
