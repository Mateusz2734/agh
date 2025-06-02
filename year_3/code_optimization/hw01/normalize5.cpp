#include <sys/time.h>
#include <iostream>
#include <string.h>

static double gtod_ref_time_sec = 0.0;

char *normalizeText(const char *input) {
  size_t len = strlen(input);
  char *buf = (char *) malloc(len + 1);
  
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
  if (j > 0 && buf[j-1] == ' ') j--;
  buf[j] = '\0';

  char *result = (char *) malloc(j + 1);
  size_t res_idx = 0;

  char lastWord[256] = {0};
  char word[256];
  size_t i = 0;
  while (i < j) {
      while (i < j && buf[i] == ' ') i++;
      if (i >= j) break;

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

double dclock() {
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (gtod_ref_time_sec == 0.0) gtod_ref_time_sec = (double)tv.tv_sec;
  norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <REPEATS>\n";
    return 1;
  }

  int repeats = std::strtol(argv[1], nullptr, 10);

  std::string s, line;
  while (std::getline(std::cin, line)) {
    s += line + "\n";
  }

  char *c_result = nullptr;
  double dtime = dclock();
  for (int i = 0; i < repeats; i++) {
    if (c_result) free(c_result);
    c_result = normalizeText(s.c_str());
  }
  dtime = dclock() - dtime;

  std::cout << dtime << "\n";
  // std::cout << "Result: " << c_result << "\n";
  free(c_result);

  return 0;
}
