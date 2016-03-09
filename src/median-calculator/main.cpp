 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 17-02-2016

   * Last Modified : Wed 09 Mar 2016 03:50:46 PM CET

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <getopt.h>

using namespace std;

#define ALL(A)     (A).begin(), (A).end()

int main(int argc, char *argv[]) {
  bool use_uncertainty = false;
  const string delimiter = "\t";
  /* ------------------------------------------------------------------------ */
  /* PARSING ARGUMENTS */
  int opt;

  const string help_msg = "Usage: %s [args]\n"
                          "\n"
                          " -u, --uncertainty \t calculate the maximum of (median - min) and (max - median)\n"
                          " -h, --help \t\t show list of command-line options\n"
                          "\n";

  // test with:
  //    ./median-calculator-icpc --uncertainty
  //    ./median-calculator-icpc --help
  const struct option longopts[] = {
    {"uncertainty", no_argument, 0, 'u'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "uh", longopts, &option_index)) != -1) {
    switch (opt) {
      case 0:
        fprintf(stderr, help_msg.c_str(), argv[0]);
        exit(EXIT_FAILURE);
      case 'u':
        use_uncertainty = true;
        break;
      case 'h':
        fprintf(stderr, help_msg.c_str(), argv[0]);
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, help_msg.c_str(), argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  /* ------------------------------------------------------------------------ */

  vector<double> data;
  double x;
  while (true) {
    cin >> x;
    if (!cin.eof()) {
      data.emplace_back(x);
    } else {
      break;
    }
  }

  int n = data.size();
  if (n > 0) {
    nth_element(data.begin(), data.begin() + n / 2, data.end());
    double median = *(data.begin() + n / 2);
    if (n % 2 == 0) {
      nth_element(data.begin(), data.begin() + n / 2 - 1, data.end());
      median += *(data.begin() + n / 2 - 1);
      median /= 2;
    }
    
    if (use_uncertainty) {
      cout << max(median - *min_element(ALL(data)), *max_element(ALL(data)) - median);
    } else {
      cout << median;
    }
    cout << delimiter;
  }

  return 0;
}
