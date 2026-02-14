# Systemy CAD/CAE - Zadanie 1

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Zmiany wprowadzone do stworzenia funkcji `splines_comp`

```matlab
% drawing sum of functions
ys = 0 .* x;
for i=1:nr
  ys = ys + coeffs(i) * compute_splines(knot_vector,p,i,x);
end
plot(x, ys, 'LineWidth', 3);
```

### 1.1. Poprawność działania

Poniżej znajduje się wykres po następującym wywołaniu funkcji:

```matlab
splines_comp(100, [1,1,2,3,4,4], [1,1,1,1])
```

![Wykres funkcji](imgs/sum.png)

Wykres pokazuje prostą `y = 1` na przedziale od `x = 1` do `x = 4`, co jest zgodne z oczekiwaniami.

## 2. Przybliżanie zdjęcia

### 2.1. Zdjęcie

![Zdjęcie](imgs/mountain.png)

### 2.2. Parametry wywołania

```matlab
knots = [0, 0, 0, 21, 41, 62, 82, 103, 123, 144, 165, 185, 206, 226, 247, 267, 288, 309, 329, 350, 370, 391, 412, 432, 453, 473, 494, 514, 535, 556, 576, 597, 617, 638, 658, 679, 679, 679]
coeffs = [192, 193, 207, 215, 232, 236, 251, 255, 260, 267, 274, 285, 291, 305, 319, 335, 344, 339, 329, 325, 320, 298, 282, 263, 244, 225, 214, 192, 196, 206, 217, 234, 250, 233, 235]
splines_comp(1000, knots, coeffs)
```

### 2.3. Wygenerowana krzywa

![Wygenerowana krzywa](imgs/plot.png)

### 2.4. Krzywa nałożona na zdjęcie

![Krzywa nałożona na zdjęcie](imgs/mountain_plot.png)

---

## 3. Dodatkowe modyfikacje

### 3.1. Próba osiągnięcia "piku" w kombinacji liniowej (B-spline 4-go stopnia)

Parametry wywołania:

```matlab
splines_comp(10000, [1,1,1,1,1,2,3,3,3,3,4,4,4,4,4], [1,1,1,0,2,0,4,0,1,1])
```

![Wykres funkcji](imgs/peak.png)

### 3.2. Użycie LLM do przetłumaczenia kodu MATLAB -> Python

Do wygenerowania wykorzystano model GPT-5 z włączonym trybem Thinking.
Przetłumaczono początkowy kod ćwiczenia (`splines.m`)

#### 3.2.1. Prompty

Prompt pierwszy:

```txt
You will be given a Matlab function that generates B-splines and plots them. Your task is to rewrite it in Python. For plotting, use matplotlib. You can use any libraries.

<Całość kodu MATLAB>
```

Prompt drugi (wprowadzający poprawki):

```txt
Carefully check the check_sanity function. It does not work as expected.
For Example 1: splines(1000, [1, 1, 2, 3, 4, 4]) it raises:
Traceback (most recent call last):
  File "/Users/mwala/Projects/CADy/main.py", line 216, in <module>
    splines(1000, [1, 1, 2, 3, 4, 4])
    ~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/Users/mwala/Projects/CADy/main.py", line 69, in splines
    raise ValueError("Poorly constructed knot_vector (sanity check failed)")
ValueError: Poorly constructed knot_vector (sanity check failed)
```

#### 3.2.2. Wygenerowany kod Python

```python
"""
b_splines.py

Python translation of the provided MATLAB B-spline basis visualiser.

Functions
---------
- splines(precision, knot_vector, show=True)
    Draws B-spline basis functions for the provided knot vector using Cox-de
    Boor recursion and matplotlib. Returns (x, basis) where basis is an
    array of shape (n_basis, len(x)).

Usage examples
--------------
>>> from b_splines import splines
>>> # evenly distributed knots, 1st order splines, 3 elements
>>> splines(1000, [1,1,2,3,4,4])

>>> # uneven knots, 3rd order splines, many elements
>>> splines(100000, [0,0,0,0,1,3,6,10,11,11.5,11.5,11.5,11.5])

Notes
-----
- Requires numpy and matplotlib.
- The MATLAB implementation used 1-based indexing; this code uses 0-based.
- The `precision` argument controls the number of intervals; the code builds
  `precision+1` sample points on the plotting domain (inclusive endpoints),
  matching the MATLAB behaviour of `a:(c-a)/precision:c`.
"""

from functools import lru_cache
from typing import Iterable, List, Tuple

import numpy as np
import matplotlib.pyplot as plt


def splines(precision: int, knot_vector: Iterable[float], show: bool = True) -> Tuple[np.ndarray, np.ndarray]:
    """Draw B-spline basis functions for a given knot vector.

    Parameters
    ----------
    precision : int
        Number of subdivisions between the drawing range endpoints. Generated
        x will have precision+1 points (inclusive endpoints), matching the
        MATLAB code's `a:(c-a)/precision:c`.
    knot_vector : iterable of float
        Non-decreasing knot vector. Multiplicity of the first knot defines
        the spline order: p = multiplicity - 1.
    show : bool
        If True (default), shows a matplotlib plot. If False, only returns
        the computed values.

    Returns
    -------
    x : ndarray
        1D array of sample points used for plotting.
    basis : ndarray
        2D array with shape (n_basis, len(x)), each row is one basis
        function evaluated on x.
    """

    kv = np.asarray(knot_vector, dtype=float)
    if kv.ndim != 1:
        raise ValueError("knot_vector must be a 1D sequence")

    p = _compute_p(kv)
    if not _check_sanity(kv, p):
        raise ValueError("Poorly constructed knot_vector (sanity check failed)")

    n_basis = len(kv) - p - 1

    x_begin = kv[0]
    x_end = kv[-1]
    x = np.linspace(x_begin, x_end, precision + 1)

    # Compute all basis functions
    basis = np.empty((n_basis, x.size), dtype=float)
    for i in range(n_basis):
        basis[i, :] = _compute_spline(kv, p, i, x)

    # Plot
    if show:
        fig, ax = plt.subplots()
        for i in range(n_basis):
            ax.plot(x, basis[i, :], linewidth=3, label=f"B_{{{i},{p}}}")
        ax.set_xlabel("x")
        ax.set_ylabel("Basis value")
        ax.set_title(f"B-spline basis functions (p={p}, n_basis={n_basis})")
        ax.legend()
        plt.show()

    return x, basis


def _compute_p(kv: np.ndarray) -> int:
    """Compute polynomial order p from the multiplicity of the first knot.

    The MATLAB code sets p equal to number_of_repetitions_of_first_entry - 1.
    """
    initial = kv[0]
    p = 0
    # MATLAB checks kv[p+2] while (p+2 <= kvsize) with p starting at 0, so
    # that corresponds to checking kv[1], kv[2], ... in 0-based Python.
    while (p + 1) < kv.size and np.isclose(kv[p + 1], initial):
        p += 1
    return p


def _check_sanity(kv: np.ndarray, p: int) -> bool:
    """Sanity checks translated from the MATLAB implementation.

    Fixed bug: the original translation incorrectly initialized the `initial`
    variable before scanning the middle of the knot vector. That caused the
    very first knot encountered in the middle-scan to be treated as a repeat
    (counter incremented), which made valid vectors like [1,1,2,3,4,4] fail.
    This version mirrors the MATLAB logic by starting `initial` at the *first*
    knot and then updating it inside the loop when a new knot value is seen.
    """
    kvsize = kv.size

    # first p+1 entries must be equal
    if not np.allclose(kv[: p + 1], kv[0]):
        return False

    # middle knots must not have multiplicity > p
    # Initialize `initial` to the first knot (matches MATLAB behaviour)
    initial = kv[0]
    counter = 1
    # MATLAB loop: for i = p+2 : kvsize-p-1 (1-based)
    # Python 0-based equivalent: range(p+1, kvsize - p - 1)
    start = p + 1
    end = kvsize - p - 1
    if end > start:
        for i in range(start, end):
            if np.isclose(kv[i], initial):
                counter += 1
                if counter > p:
                    return False
            else:
                initial = kv[i]
                counter = 1

    # last p+1 entries must be equal
    if not np.allclose(kv[-(p + 1) :], kv[-1]):
        return False

    # knot vector must be non-decreasing (allow tiny numerical tolerance)
    if np.any(np.diff(kv) < -1e-12):
        return False

    return True


def _compute_spline(kv: np.ndarray, p: int, i: int, x: np.ndarray) -> np.ndarray:
    """Recursive Cox-de Boor implementation.

    Parameters
    ----------
    kv : ndarray
        Knot vector (1D array).
    p : int
        Polynomial order.
    i : int
        Basis function index (0-based). Corresponds to "i" in B_{i,p}.
    x : ndarray
        Points where to evaluate the basis function.

    Returns
    -------
    y : ndarray
        Evaluated basis function values at x.
    """
    # local names echoing MATLAB variables for clarity
    a = kv[i]
    b = kv[i + p]
    c = kv[i + 1]
    d = kv[i + p + 1]

    # p == 0 -> piecewise constant (indicator on [a, d])
    if p == 0:
        y = np.zeros_like(x, dtype=float)
        # inclusive interval [a, d]
        mask = (x >= a) & (x <= d)
        y[mask] = 1.0
        return y

    # recursive components
    lp = _compute_spline(kv, p - 1, i, x)
    rp = _compute_spline(kv, p - 1, i + 1, x)

    # first factor: (x - a) / (b - a) on [a, b]
    if np.isclose(a, b):
        y1 = np.zeros_like(x, dtype=float)
        mask = (x >= a) & (x <= b)
        y1[mask] = 1.0
    else:
        denom = (b - a)
        y1 = np.zeros_like(x, dtype=float)
        mask = (x >= a) & (x <= b)
        y1[mask] = (x[mask] - a) / denom

    # second factor: (d - x) / (d - c) on (c, d]
    if np.isclose(c, d):
        y2 = np.zeros_like(x, dtype=float)
        mask = (x > c) & (x <= d)
        y2[mask] = 1.0
    else:
        denom = (d - c)
        y2 = np.zeros_like(x, dtype=float)
        mask = (x > c) & (x <= d)
        y2[mask] = (d - x[mask]) / denom

    y = lp * y1 + rp * y2
    return y


if __name__ == "__main__":
    # Quick demonstration (mirrors the MATLAB examples from the prompt)
    # Example 1
    splines(1000, [1, 1, 2, 3, 4, 4])

    # Example 2 (uncomment to run; it may draw many points)
    # splines(100000, [0,0,0,0,1,3,6,10,11,11.5,11.5,11.5,11.5])
```

#### 3.2.3. Poprawność działania

W celu weryfikacji poprawności wygenerowanego kodu, porównano działanie z oryginalnym kodem MATLAB. Wyniki:

| Parametry wywołania | Wynik Python | Wynik MATLAB |
|---------------------|--------------|--------------|
| `splines(100, [0,0,0,1,1.5,2,2,2])` | ![Python test 1](imgs/funcs/P1.png) | ![MATLAB test 1](imgs/funcs/M1.png) |
| `splines(1000, [1,1,2,3,4,4])` | ![Python test 2](imgs/funcs/P2.png) | ![MATLAB test 2](imgs/funcs/M2.png) |
| `splines(1000, [0,0,0,0,1,3,6,10,11,11.5,11.5,11.5,11.5])` | ![Python test 3](imgs/funcs/P3.png) | ![MATLAB test 3](imgs/funcs/M3.png) |
| `splines(1000, [1,1,1,2,2,3,4,4,4])` | ![Python test 4](imgs/funcs/P4.png) | ![MATLAB test 4](imgs/funcs/M4.png) |

Wyniki sugerują, że wygenerowany kod działa poprawnie.

### 3.3. Automatyczne wyznaczanie węzłów i współczynników

#### 3.3.1. Wykorzystany kod

```python
import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_lsq_spline

# Load image and convert to grayscale
img = cv2.imread("imgs/everest.jpg")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
height, width = gray.shape

# Edge detection
edges = cv2.Canny(gray, 50, 150)

plt.imshow(edges, cmap='gray')
plt.show()

# Extract skyline
x_coords = []
y_coords = []

for x in range(width):
    ys = np.where(edges[:, x])[0]
    if len(ys) > 0:
        y_coords.append(ys.min())
        x_coords.append(x)

x_coords = np.array(x_coords)
y_coords = np.array(y_coords)

# Flip y-axis
y_coords = height - 1 - y_coords

# Fit B-spline
degree = 2
points = 35

x_min, x_max = x_coords.min(), x_coords.max()
inner_knots = np.linspace(x_min, x_max, points - degree + 1)
knot_vector = np.concatenate((
    np.full(degree, x_min),
    inner_knots,
    np.full(degree, x_max)
))

spline = make_lsq_spline(x_coords, y_coords, knot_vector, k=degree)

# Plot fitted curve
x_fine = np.linspace(x_min, x_max, 1000)
y_spline = spline(x_fine)

plt.figure(figsize=(12,6))
plt.imshow(np.flipud(gray), cmap='gray', origin="lower")
plt.plot(x_coords, y_coords, 'r.', markersize=2, label="Detected skyline")
plt.plot(x_fine, y_spline, 'b-', linewidth=2, label="B-spline fit")

for k in spline.t:
    plt.axvline(k, color='green', linestyle='--', alpha=0.5)

plt.legend()
plt.show()

print("knots =", list(map(lambda x: round(float(x)), spline.t)))
print("coeffs =", list(map(lambda x: round(float(x)), spline.c)))
```

#### 3.3.2. Zasada działania

1. Załadowanie zdjęcia i konwersja do skali szarości
2. Wykrycie krawędzi za pomocą algorytmu Canny'ego
3. Wydobycie "skyline" (najwyższej części zdjęcia)
4. Dopasowanie B-spline metodą najmniejszych kwadratów
5. Wyświetlenie zdjęcia z nałożoną krzywą B-spline, wykrytymi punktami "skyline" oraz węzłami B-spline

#### 3.3.3. Wynik na zdjęciu Mount Everest

![Wynik na zdjęciu Mount Everest](imgs/everest_approx.png)
