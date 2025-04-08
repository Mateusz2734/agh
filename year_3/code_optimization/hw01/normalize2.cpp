#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>

static double gtod_ref_time_sec = 0.0;

std::string normalizeText(std::string input) {
  std::string processed;
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

  double dtime;

  std::string s;
  std::string result;
  std::string line;
  while (getline(std::cin, line)) {
    s += line + "\n";
  }

  dtime = dclock();
  for (int i = 0; i < repeats; i++) {
    result = normalizeText(s);
  }
  dtime = dclock() - dtime;

  std::cout << "Time: " << dtime << "\n";
  fflush(stdout);

  return 0;
}
