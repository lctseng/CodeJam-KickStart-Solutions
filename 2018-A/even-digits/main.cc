#include <algorithm>
#include <iostream>
using namespace std;

long long int findExp(long long int n) {
  long long int exp = 1;
  while (n / exp >= 10) {
    exp *= 10;
  }
  return exp;
}

long long int genLowerBound(long long int expN) {
  long long int res = 0;
  while (expN >= 10) {
    res = res * 10 + 8;
    expN /= 10;
  }
  return res;
}

long long int solve(long long int n) {
  long long int res = 0;
  // find the largest odd digit
  // ex: 2140, find 100
  // try go up and go down
  // ex to 2200 or 2088
  long long int expN = findExp(n);
  long long int validN = n;
  int oddDigit, prevDigit = 0;
  while (expN > 0) {
    oddDigit = validN / expN;
    if (oddDigit & 1)
      break;
    prevDigit = oddDigit;
    validN %= expN;
    expN /= 10;
  }
  if (expN) {
    // oddDigit contains that odd digit
    // find bound
    long long int upperRes = n;
    if (prevDigit & 1 || oddDigit < 9) {
      // find upper when: oddDigit < 9 or prevDigit is odd
      upperRes = (oddDigit + 1) * expN - validN;
    }
    // find lower
    // if expN is 1000, then next lower is 888 + (oddDigit - 1) * expN
    long long int lowerBound = genLowerBound(expN) + (oddDigit - 1) * expN;
    long long int lowRes = validN - lowerBound;
    res = min(lowRes, upperRes);
  } else {
    // all even
    res = 0;
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    long long int n;
    cin >> n;
    long long int res = solve(n);
    cout << "Case #" << t << ": " << res << endl;
  }
  return 0;
}