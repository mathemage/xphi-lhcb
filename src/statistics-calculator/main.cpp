 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 17-02-2016

   * Last Modified : Wed 17 Feb 2016 03:15:40 PM CET

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
using namespace std;

#define ALL(A)     (A).begin(), (A).end()

int main() {
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
  if (n > 1) {
    double mean = accumulate(ALL(data), 0.0) / n;
    cout << "mean = " << mean << "\t";
    double stddev = 0.0;
    for (auto & x : data) {
      stddev += (x - mean) * (x - mean);
    }
    stddev /= n - 1;
    stddev = sqrt(stddev);
    cout << "standard deviation = " << stddev << "\t";
    cout << "min = " << *min_element(ALL(data)) << "\t";
    cout << "max = " << *max_element(ALL(data)) << "\t";
    cout << endl;
  }

  return 0;
}
