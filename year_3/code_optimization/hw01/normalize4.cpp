#include <sys/time.h>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

static double gtod_ref_time_sec = 0.0;

std::string normalizeText(const std::string& input) {
  std::string processed;
  processed.resize(input.size());
  
  std::transform(input.begin(), input.end(), processed.begin(), [](unsigned char ch) -> char {
      if (std::isspace(ch)) return ' ';
      if (ch >= 32 && ch <= 126) return std::ispunct(ch) ? ',' : static_cast<char>(std::tolower(ch));
      return ch;
  });
  
  std::string output;
  output.reserve(processed.size());
  std::unique_copy(processed.begin(), processed.end(),
                    std::back_inserter(output),
                    [](char a, char b) { return a == ' ' && b == ' '; });

  std::istringstream iss(output);
  std::ostringstream oss;
  std::unique_copy(
      std::istream_iterator<std::string>(iss),
      std::istream_iterator<std::string>(),
      std::ostream_iterator<std::string>(oss, " ")
  );

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

  std::cout << dtime << "\n";
  // std::cout << "Result: " << result << "\n";
  fflush(stdout);

  return 0;
}
