# Optymalizacja kodu na różne architektury - Zadanie 2

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Cel zadania

Celem tego zadania była optymalizacja algorytmu eliminacji Gaussa wzorując się na laboratoriach oraz [instruktażu](https://github.com/flame/how-to-optimize-gemm)

## 2. Opis sprzętu

Zadanie wykonano na laptopie z 10-rdzeniowym procesorem Apple M1 Pro oraz 32GB pamięci RAM. Dane dotyczące pamięci cache:

|Poziom cache | Rozmiar | Uwagi|
|------------|---------|------|
|L1 (Firestorm) | 192 KB instrukcji + 128 KB danych | na rdzeń|
|L1 (Icestorm) | 128 KB instrukcji + 64 KB danych | na rdzeń|
|L2 (Firestorm) | 12 MB | współdzielone przez rdzenie wydajnościowe|
|L2 (Icestorm) | 4 MB | współdzielone przez rdzenie energooszczędne|
|SLC | 24 MB | SLC - System-Level Cache|

Procesor posiada 4 128-bitowe jednostki SIMD.

Niestety, autorowi nie udało się znaleźć oficjalnych danych dotyczących wydajności FP64, jednak korzystając ze wzoru `nflops_per_cycle * nprocessors * GHz_of_processor` można oszacować, że procesor (a właściwie jego rdzenie wydajnościowe) jest w stanie wykonać około `204.8` GFLOPS, co odpowiada `25.6` GFLOPS na rdzeń.

## 3. Wersja bazowa

Wersja najprostsza:

```c
// base.c
void ref_gauss(double *A, int SIZE) {
  int i,j,k;
  for (k = 0; k < SIZE; k++) { 
    for (i = k + 1; i < SIZE; i++) { 
      for (j = k + 1; j < SIZE; j++) {
        A[IDX(i, j, SIZE)] -= A[IDX(k, j, SIZE)] * (A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)]);
      } 
    }
  }
}
```

Wersja z ograniczoną liczbą mnożeń (liczba operacji identyczna do wersji zoptymalizowanej):

```c
// base-mult.c
void ref_gauss(double *A, int SIZE) {
  int i,j,k;
  double mul;
  for (k = 0; k < SIZE; k++) { 
    for (i = k + 1; i < SIZE; i++) { 
      mul = A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)];
      for (j = k + 1; j < SIZE; j++) {
        A[IDX(i, j, SIZE)] -= A[IDX(k, j, SIZE)] * mul;
      } 
    }
  }
}
```

## 4. Wersja zoptymalizowana

Wersja zoptymalizowana wykorzystuje instrukcje SIMD do przyspieszenia obliczeń. Użyto technologii Arm Neon, która jest dostępna na procesorach Apple M1.

Oprócz tego, przeniesiono zmienne do rejestrów.

```c
void gauss(double *A, int SIZE) {
  register int i, j, k;
  register double mul;
  register float64x2_t mul_vec;
  for (k = 0; k < SIZE; k++) {
    for (i = k + 1; i < SIZE; i++) {
      mul = A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)];
      mul_vec = vdupq_n_f64(mul);
      for (j = k + 1; j < SIZE;) {
        if (j <= SIZE - BLKSIZE) {
          float64x2_t aik0 = vld1q_f64(&A[IDX(i, j + 0, SIZE)]);
          float64x2_t akk0 = vld1q_f64(&A[IDX(k, j + 0, SIZE)]);
          float64x2_t aik1 = vld1q_f64(&A[IDX(i, j + 2, SIZE)]);
          float64x2_t akk1 = vld1q_f64(&A[IDX(k, j + 2, SIZE)]);
          float64x2_t aik2 = vld1q_f64(&A[IDX(i, j + 4, SIZE)]);
          float64x2_t akk2 = vld1q_f64(&A[IDX(k, j + 4, SIZE)]);
          float64x2_t aik3 = vld1q_f64(&A[IDX(i, j + 6, SIZE)]);
          float64x2_t akk3 = vld1q_f64(&A[IDX(k, j + 6, SIZE)]);

          aik0 = vmlsq_f64(aik0, akk0, mul_vec);
          aik1 = vmlsq_f64(aik1, akk1, mul_vec);
          aik2 = vmlsq_f64(aik2, akk2, mul_vec);
          aik3 = vmlsq_f64(aik3, akk3, mul_vec);

          vst1q_f64(&A[IDX(i, j + 0, SIZE)], aik0);
          vst1q_f64(&A[IDX(i, j + 2, SIZE)], aik1);
          vst1q_f64(&A[IDX(i, j + 4, SIZE)], aik2);
          vst1q_f64(&A[IDX(i, j + 6, SIZE)], aik3);

          j += BLKSIZE;
        } else {
          A[IDX(i, j, SIZE)] -= A[IDX(k, j, SIZE)] * mul;
          j++;
        }
      }
    }
  }
}
```

## 5. Sposób porównywania wyników

Do wygenerowania danych na temat czasu działania, GFLOPS oraz poprawności algorytmu wykorzystano skrypt `compare.c`, wzorowany na skrypcie z [instruktażu](https://github.com/flame/how-to-optimize-gemm/blob/master/src/HowToOptimizeGemm/test_MMult.c).

```c
// compare.c
int main() {
  int p, rep;
  double dtime, dtime_best, gflops, diff;
  double *a, *aold, *aref;

  srand(42);

  for (p = PFIRST; p <= PLAST; p += PINC) {
    gflops = ((p - 1) * (p) * (2 * p - 1) * 1.0e-09) / 2;

    a = (double *)malloc(PLAST * (PLAST + 1) * sizeof(double));
    aold = (double *)malloc(PLAST * PLAST * sizeof(double));
    aref = (double *)malloc(PLAST * PLAST * sizeof(double));

    random_matrix(a, p);

    copy_matrix(a, aref, p);
    copy_matrix(a, aold, p);

    ref_gauss(aref, p);
    print_matrix(aref, p);

    for (rep = 0; rep < NREPEATS; rep++) {
      copy_matrix(a, aold, p);
      dtime = dclock();
      gauss(aold, p);
      dtime = dclock() - dtime;

      if (rep == 0) {
        dtime_best = dtime;
        print_matrix(aold, p);
      } else {
        dtime_best = (dtime < dtime_best ? dtime : dtime_best);
      }
    }

    diff = compare_matrices(aold, aref, p);

    printf("{\"size\": %d, \"gflops\": %le, \"diff\": %le, \"time\": %le}\n", p, gflops / dtime_best, diff, dtime_best);
    fflush(stdout);

    free(a);
    free(aold);
    free(aref);
  }

  exit(0);
}
```

Wyniki zostały zapisane do plików JSONL. Przykładowy wynik:

```json
{"size": 1000, "gflops": 6.069191e+00, "diff": 0.000000e+00, "time": 1.097620e-01}
{"size": 1040, "gflops": 5.972062e+00, "diff": 0.000000e+00, "time": 1.254790e-01}
{"size": 1080, "gflops": 6.198297e+00, "diff": 0.000000e+00, "time": 1.353960e-01}
{"size": 1120, "gflops": 6.183221e+00, "diff": 0.000000e+00, "time": 1.513760e-01}
{"size": 1160, "gflops": 5.902423e+00, "diff": 0.000000e+00, "time": 1.761860e-01}
{"size": 1200, "gflops": 6.029632e+00, "diff": 0.000000e+00, "time": 1.909370e-01}
{"size": 1240, "gflops": 6.101352e+00, "diff": 0.000000e+00, "time": 2.082020e-01}
{"size": 1280, "gflops": 6.011107e+00, "diff": 0.000000e+00, "time": 2.324500e-01}
{"size": 1320, "gflops": 5.796773e+00, "diff": 0.000000e+00, "time": 2.643610e-01}
{"size": 1360, "gflops": 6.061385e+00, "diff": 0.000000e+00, "time": 2.765120e-01}
{"size": 1400, "gflops": 6.063204e+00, "diff": 0.000000e+00, "time": 3.015490e-01}
```

**Poprawność wyników** - wyniki zostały porównane z wersją bazową - były one identyczne, co widać powyżej (`"diff": 0.000000e+00`).

Dane zostały następnie przetworzone przez skrypt `plot.py` w celu wygenerowania wykresów.

```python
# plot.py
import json

import matplotlib.pyplot as plt


def load_data(path):
    with open(path, "r") as f:
        return [json.loads(line) for line in f]


file1 = "docs/ref.jsonl"
file2 = "docs/opt.jsonl"

data1 = load_data(file1)
data2 = load_data(file2)

data1.sort(key=lambda x: x["size"])
data2.sort(key=lambda x: x["size"])

sizes1 = [d["size"] for d in data1]
time1 = [d["time"] for d in data1]
gflops1 = [d["gflops"] for d in data1]

time2 = [d["time"] for d in data2]
gflops2 = [d["gflops"] for d in data2]

# Time plot
plt.figure(figsize=(10, 6))
plt.plot(sizes, time1, label="Reference", marker="o")
plt.plot(sizes, time2, label="Optimized", marker="s")
plt.xlabel("Matrix Size")
plt.ylabel("Time (s)")
plt.title("Time vs Matrix Size")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("docs/time.png")

# Gflops plot
plt.figure(figsize=(10, 6))
plt.plot(sizes, gflops1, label="Reference", marker="o")
plt.plot(sizes, gflops2, label="Optimized", marker="s")
plt.xlabel("Matrix Size")
plt.ylabel("Gflops")
plt.title("Gflops vs Matrix Size")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("docs/gflops.png")
```

## 6. Wykresy

Czas działania w zależności od rozmiaru macierzy:
![Czas działania](docs/time.png)

GFLOPS w zależności od rozmiaru macierzy:
![GFLOPS](docs/gflops.png)

## 7. Wyniki profilowania

Profilowanie wykonano na macierzy o rozmiarze 5000x5000 dla wersji zoptymalizowanej. Ze względu na niedostępność PAPI, wykorzystano narzędzie `Instruments` w Xcode, udostępnione przez Apple. Skorzystano z następujących profili:

- "Time Profiler" - profilowanie czasu działania i zużycia procesora
- "CPU Profiler" - profilowanie ilości cykli procesora
- "CPU Counters" - profilowanie ilości operacji SIMD z podziałem na instrukcje typu LOAD, STORE, ALU

Time Profiler - Stack Trace:

![Time Profiler Stack Trace](docs/time_profiler_st.png)

Time Profiler - zużycie procesora (stale na poziomie ~100%):
![Time Profiler](docs/time_profiler.png)

CPU Profiler - Stack Trace:

![CPU Profiler Stack Trace](docs/cpu_profiler.png)

CPU Profiler - podział na rdzenie:

![CPU Profiler](docs/cpus.png)

CPU Counters - podział na instrukcje typu LOAD, STORE, ALU:

![CPU Counters](docs/simd.png)

## 8. Podsumowanie i wnioski

1. Dla największych rozmiarów macierzy, wersja zoptymalizowana jest około 2.5x szybsza od wersji bazowej oraz około 2x szybsza od wersji z ograniczoną liczbą mnożeń.
2. Wersja zoptymalizowana wykorzystuje cztery 128-bitowe jednostki wektorowe, co pozwala na jednoczesne przetwarzanie 8 elementów macierzy.
3. Poprawność wersji zoptymalizowanej została potwierdzona – jej wyniki są zgodne z wersją referencyjną.
4. Najwyższa osiągnięta wydajność (~9 GFLOPS) stanowi około 36% teoretycznej maksymalnej wydajności procesora.
5. Pozostający zapas wydajności sugeruje możliwość dalszych optymalizacji, np. zwiększając lokalność danych.
