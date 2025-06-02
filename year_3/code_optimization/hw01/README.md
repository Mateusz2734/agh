# Optymalizacja kodu na różne architektury - Zadanie 1

Mateusz Wala (*<mateuszwala@student.agh.edu.pl>*)

## 1. Cel zadania

Celem tego zadania była implementacja funkcji przetwarzającej duże ciągi tekstowe (np. zawartość dużego pliku) wykonując szereg operacji normalizacyjnych oraz optymalizacyjnych:

- usunięcie znaków spoza zakresu drukowalnych
- zamiana sekwencji białych znaków na pojedynczą spację
- zamiana znaków interpunkcyjnych na przecinki
- konwersja wszystkich liter na małe
- usunięcie powtórzeń słów

## 2. Opis sprzętu

Zadanie wykonano na laptopie z 10-rdzeniowym procesorem Apple M1 Pro oraz 32GB pamięci RAM. Dane dotyczące pamięci cache:

|Poziom cache | Rozmiar | Uwagi|
|------------|---------|------|
|L1 (Firestorm) | 192 KB instrukcji + 128 KB danych | na rdzeń|
|L1 (Icestorm) | 128 KB instrukcji + 64 KB danych | na rdzeń|
|L2 (Firestorm) | 12 MB | współdzielone przez rdzenie wydajnościowe|
|L2 (Icestorm) | 4 MB | współdzielone przez rdzenie energooszczędne|
|SLC | 24 MB | współdzielone m.in. przez CPU i GPU|

gdzie:

- **Firestorm** - rdzenie wydajnościowe
- **Icestorm** - rdzenie energooszczędne

Procesor posiada 4 128-bitowe jednostki SIMD.

Wszystkie testy przeprowadzono w kontenerze Docker z systemem Debian 12, zainstalwanym kompilatorem `g++` w wersji `12.2.0`, ze względu na problemy z profilowaniem kodu w systemie macOS. Kontener uruchomiono bez ograniczeń zasobów.

## 3. Opis implementacji kolejnych wersji

Kolejne wersje kodu zostaną przedstawione w 3 sekcjach:

- **Implementacja** - kod źródłowy
- **Opis** - krótki opis działania
- **Profilowanie** - graficzne wyniki profilowania kodu oraz czas działania dla przykładowych danych (`~10kiB`, `10 000` iteracji)

<!-- WERSJA 1 -->

### **Wersja 1** Implementacja bazowa bez optymalizacji

#### Implementacja

```c++
std::string normalizeText(std::string input) {
  std::string processed;
  bool lastWasSpace = false;
  for (char ch : input) {
    if (std::isspace(static_cast<unsigned char>(ch))) {
      if (!lastWasSpace) {
        processed = processed + ' ';
        lastWasSpace = true;
      }
    } else {
      processed = processed + ch;
      lastWasSpace = false;
    }
  }

  std::string cleaned;
  for (char ch : processed) {
    if (ch >= 32 && ch <= 126) {
      cleaned = cleaned + ch;
    }
  }

  for (char &ch : cleaned) {
    ch = std::tolower(static_cast<unsigned char>(ch));
  }

  for (char &ch : cleaned) {
    if (std::ispunct(static_cast<unsigned char>(ch))) {
      ch = ',';
    }
  }

  std::istringstream iss(cleaned);
  std::ostringstream oss;
  std::string word, lastWord;
  bool first = true;
  while (iss >> word) {
    if (word != lastWord) {
      if (!first) {
        oss << " ";
      }
      oss << word;
      lastWord = word;
      first = false;
    }
  }

  return oss.str();
}
```

#### Opis

Wersja bazowa funkcji wykonuje wszystkie wymagane operacje w sposób najbardziej prosty i według autora oczywisty, bez jakichkolwiek optymalizacji. Wykorzystuje ona wiele kosztownych operacji na stringach, wymagających alokacji pamięci, na przykład `cleaned = cleaned + ch;`. Niską wydajność można zauważyć już przy stosunkowo niewielkich danych wejściowych, co zostanie pokazane w późniejszych punktach sprawozdania.

#### Profilowanie

![Profilowanie wersji 1](./report/output1.png)

**Czasy działania**:

Bez flagi kompilatora -O2: `53.8493 s`

Z flagą -O2: `42.4778 s`

<!-- WERSJA 2 -->

### **Wersja 2** Prealokacja pamięci oraz transformacje wykonywane in-place

##### Implementacja

```c++
std::string normalizeText(std::string input) {
  std::string processed;
  processed.reserve(input.length());

  bool lastWasSpace = false;
  for (char ch : input) {
    if (std::isspace(static_cast<unsigned char>(ch))) {
      if (!lastWasSpace) {
        processed += ' ';
        lastWasSpace = true;
      }
    }
    else {
      processed += ch;
      lastWasSpace = false;
    }
  }

  std::string cleaned;
  cleaned.reserve(processed.length());
  for (char ch : processed) {
    if (ch >= 32 && ch <= 126) {
      cleaned += ch;
    }
  }

  for (char &ch : cleaned) {
    ch = std::tolower(static_cast<unsigned char>(ch));
  }

  for (char &ch : cleaned) {
    if (std::ispunct(static_cast<unsigned char>(ch))) {
      ch = ',';
    }
  }

  std::istringstream iss(cleaned);
  std::ostringstream oss;
  std::string word, lastWord;
  bool first = true;
  while (iss >> word) {
    if (word != lastWord) {
      if (!first) {
        oss << " ";
      }
      oss << word;
      lastWord = word;
      first = false;
    }
  }

  return oss.str();
}
```

#### Opis

Wersja druga wprowadza kilka optymalizacji w stosunku do wersji pierwszej. Przede wszystkim, wykorzystano operacje in-place, jak na przykład `cleaned += ch;` Dodatkowo, wykorzystano prealokację pamięci dla zmiennych `processed` oraz `cleaned` (`processed.reserve(input.length())`). Pozwala to uniknąć kosztownych operacji alokacji pamięci.

#### Profilowanie

![Profilowanie wersji 2](./report/output2.png)

Dzięki wykorzystanym optymalizacjom, całkowicie pozbyto się wszystkich 190mln wywołań operacji `__gnu_cxx11::basic_string std::operator+`. Można również zauważyć około 20-krotny spadek czasu wykonania programu z (flagą -O2) w porównaniu do wersji pierwszej.

**Czasy działania**:

Bez flagi kompilatora -O2: `14.7858 s`

Z flagą -O2: `2.46263 s`

### **Wersja 3** Zwinięcie większości operacji do jednej pętli

##### Implementacja

```c++
std::string normalizeText(const std::string & input) {
  std::string processed;
  processed.reserve(input.length());

  bool lastWasSpace = false;
  for (char ch : input) {
    if (std::isspace(static_cast<unsigned char>(ch))) {
      if (!lastWasSpace) {
        processed += ' ';
        lastWasSpace = true;
      }
    } else if (ch >= 32 && ch <= 126) {
      processed += std::ispunct(static_cast<unsigned char>(ch)) ? ',' : std::tolower(static_cast<unsigned char>(ch));
      lastWasSpace = false;
    }
  }


  std::istringstream iss(processed);
  std::ostringstream oss;
  std::string word, lastWord;
  bool first = true;
  while (iss >> word) {
    if (word != lastWord) {
      if (!first) {
        oss << " ";
      }
      oss << word;
      lastWord = word;
      first = false;
    }
  }

  return oss.str();
}
```

#### Opis

Wersja trzecia wprowadza kolejne optymalizacje. Wykorzystano jedną pętlę do wykonania większości transformacji, co pozwala na uniknięcie wielokrotnego przechodzenia przez ten sam tekst.

#### Profilowanie

![Profilowanie wersji 3](./report/output3.png)

Zmniejszenie ilości pętli przechodzących przez ten sam tekst pozwoliło na znaczną (niemal 4-krotną) redukcję wywołań operacji typu `__gnu_cxx::__normal_iterator*`

**Czasy działania**:

Bez flagi kompilatora -O2: `6.14901 s`

Z flagą -O2: `2.34785 s`

### **Wersja 4** Iteratory i algorytmy standardowe

##### Implementacja

```c++
std::string normalizeText(const std::string& input) {
  std::string processed;
  processed.resize(input.size());
  
  std::transform(input.begin(), input.end(), processed.begin(), [](unsigned char ch) -> char {
      if (std::isspace(ch))
          return ' ';
      if (ch >= 32 && ch <= 126)
          return std::ispunct(ch) ? ',' : static_cast<char>(std::tolower(ch));
      return ch;
  });
  
  std::string cleared;
  cleared.reserve(processed.size());
  std::unique_copy(processed.begin(), processed.end(),
                    std::back_inserter(cleared),
                    [](char a, char b) { return a == ' ' && b == ' '; });

  std::istringstream iss(cleared);
  std::ostringstream oss;
  std::unique_copy(
      std::istream_iterator<std::string>(iss),
      std::istream_iterator<std::string>(),
      std::ostream_iterator<std::string>(oss, " ")
  );

  return oss.str();
}
```

#### Opis

Wersja czwarta wykorzystuje algorytmy standardowe oraz iteratory, co pozwala na bardziej nowoczesny kod. Autor, ze względu na słabą znajomość języka C++, nie jest w stanie ocenić, czy takie podejście jest bardziej czytelne, czy "lepsze" od poprzednich.

#### Profilowanie

![Profilowanie wersji 4](./report/output4.png)

Wersja czwarta wprowadza znaczną ilość nowych wywołań, jednakże nie wpływa to znacząco na czas działania programu. Można zauważyć, że czas działania z flagą -O2 jest porównywalny z wersją trzecią, lecz jednak nieco wyższy. Bez flagi -O2 czas działania jest znacznie wyższy.

**Czasy działania**:

Bez flagi kompilatora -O2: `16.2092 s`

Z flagą -O2: `2.66428 s`

### **Wersja 5** Bare-metal

##### Implementacja

```c++
char *normalizeText(const char *input) {
  size_t len = strlen(input);
  char *buf = (char *) malloc(len + 1);
  if (!buf) return NULL;

  size_t j = 0;
  int lastWasSpace = 0;
  for (size_t i = 0; i < len; i++) {
      unsigned char ch = (unsigned char)input[i];
      if (isspace(ch)) {
          if (!lastWasSpace) {
              buf[j++] = ' ';
              lastWasSpace = 1;
          }
      } else if (ch >= 32 && ch <= 126) {
          buf[j++] = ispunct(ch) ? ',' : (char)tolower(ch);
          lastWasSpace = 0;
      }
  }
  if (j > 0 && buf[j-1] == ' ')
      j--;
  buf[j] = '\0';

  char *result = (char *) malloc(j + 1);
  if (!result) {
      free(buf);
      return NULL;
  }
  size_t res_idx = 0;

  char lastWord[256] = {0};
  char word[256];
  size_t i = 0;
  while (i < j) {
      while (i < j && buf[i] == ' ')
          i++;
      if (i >= j)
          break;

      size_t k = 0;
      while (i < j && buf[i] != ' ' && k < sizeof(word) - 1) {
          word[k++] = buf[i++];
      }
      word[k] = '\0';

      if (strcmp(word, lastWord) != 0) {
          if (res_idx > 0) {
              result[res_idx++] = ' ';
          }
          memcpy(result + res_idx, word, k);
          res_idx += k;
          strncpy(lastWord, word, sizeof(lastWord) - 1);
          lastWord[sizeof(lastWord) - 1] = '\0';
      }
  }
  result[res_idx] = '\0';

  free(buf);
  return result;
}
```

#### Opis

Wersja piąta to wersja bare-metal, która nie korzysta z żadnych bibliotek C++. Wykorzystuje jedynie funkcje z biblioteki C, takie jak `malloc`, `free`, `memcpy`, `strncpy` oraz `strcmp`.

#### Profilowanie

**Czasy działania**:

Bez flagi kompilatora -O2: `2.07847 s`

Z flagą -O2: `1.84943 s`

Wersja ta jest najszybsza, nawet bez optymalizacji dokonanych przez kompilator.

### **Wersja 6** Wersja równoległa

#### Brak implementacji

Wersja równoległa nie została zaimplementowana, ponieważ autor nie miał wystarczającej ilości czasu oraz wiedzy na temat zastosowania równoległości w C++.  

## 4. Porównanie czasów działania

Tabela porównawcza czasów działania wszystkich wersji kodu w zależności od rozmiaru danych wejściowych:

| Rozmiar danych wejściowych | Wersja 1  | Wersja 2 | Wersja 3 | Wersja 4 | Wersja 5 |
|----------------------------|-----------|----------|----------|----------|----------|
| 1 KiB                      | 0.000182 s| 2.6e-05 s| 4.5e-05 s | 3.7e-05 s | 2.6e-05 s |
| 10 KiB                     | 0.004407 s|  0.0002 s|0.000239 s |0.000339 s |0.000157 s |
| 100 KiB                    | 0.453645 s| 0.00274 s|0.002095 s |0.002417 s |0.002092 s |
| 1 MiB                      | 55.2972 s | 0.02663 s|0.023164 s |0.027906 s |0.019636 s |
| 10 MiB                     | -         | 0.27235 s| 0.24753 s | 0.28103 s |0.197145 s |
| 100 MiB                    | -         | 2.68582 s| 2.42997 s | 2.82031 s | 1.95215 s |
| 1 GiB                      | -         |  -       | 22.7304 s | 27.4443 s | 19.8913 s |

## 5. Podsumowanie

1. Wersja piąta, bare-metal, okazała się najszybsza, czego można było się spodziewać. Brak narzutów związanych z OOP pozwolił na uzyskanie najlepszych wyników.
2. Największym problemem napotkanym podczas implementacji była znikoma znajomość języka C++, co skutkowało wieloma sesjami debugowania i poszukiwaniem informacji w Internecie.
