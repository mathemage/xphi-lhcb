 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 17-02-2016

   * Last Modified : Tue 08 Mar 2016 05:45:34 PM CET

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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
  if (n > 0) {
    nth_element(data.begin(), data.begin() + n / 2, data.end());
    double median = *(data.begin() + n / 2);
    if (n % 2 == 0) {
      nth_element(data.begin(), data.begin() + n / 2 - 1, data.end());
      median += *(data.begin() + n / 2 - 1);
      median /= 2;
    }
    
    cout << median << endl;
  }

  return 0;
}
