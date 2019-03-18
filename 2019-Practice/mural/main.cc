#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int solve(vector<uint8_t> &wall) {
  int windowSize = (wall.size() + 1) / 2;
  // find max sum with window size
  // find first sum
  int current = 0;
  for (int i = 0; i < windowSize; i++) {
    current += wall[i];
  }
  int maxValue = current;
  for (int i = windowSize; i < wall.size(); i++) {
    // consider wall[i], remove wall[i - windowSize]
    current += wall[i] - wall[i - windowSize];
    if (current > maxValue)
      maxValue = current;
  }
  return maxValue;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int n;
    cin >> n;
    vector<uint8_t> wall;
    for (int i = 0; i < n; i++) {
      char c;
      cin >> c;
      wall.push_back(c - '0');
    }
    cout << "Case #" << t << ": " << solve(wall) << endl;
  }
  return 0;
}