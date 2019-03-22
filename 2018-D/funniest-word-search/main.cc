#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int gcd(int a, int b) {
  if (a < b)
    swap(a, b);
  while (b) {
    int temp = a % b;
    a = b;
    b = temp;
  }
  return a;
}

namespace Small {

struct Histogram {
  array<int, 26> data = {0};
  Histogram &operator+=(const Histogram &rhs) {
    for (int i = 0; i < 26; i++) {
      data[i] += rhs.data[i];
    }
    return *this;
  }

  Histogram &operator-=(const Histogram &rhs) {
    for (int i = 0; i < 26; i++) {
      data[i] -= rhs.data[i];
    }
    return *this;
  }
};

pair<int, int> makeFunValue(int top, int down) {
  // TODO: simplify
  if (top == 0) {
    down = 1;
  } else {
    int common = gcd(top, down);
    top /= common;
    down /= common;
  }
  return make_pair(top, down);
}

string solve(int R, int C, vector<vector<char>> &grid, vector<string> words) {
  // create RxC sum area table, each entry
  vector<vector<Histogram>> table(R, vector<Histogram>(C));
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      table[i][j].data[grid[i][j] - 'A']++;
      if (i > 0) {
        table[i][j] += table[i - 1][j];
      }
      if (j > 0) {
        table[i][j] += table[i][j - 1];
      }
      if (i > 0 && j > 0) {
        table[i][j] -= table[i - 1][j - 1];
      }
      // cout << "histogram table " << i << ", " << j << endl;
      // for (int w = 0; w < 26; w++) {
      //   if (table[i][j].data[w] > 0) {
      //     cout << (char)('A' + w) << ": " << table[i][j].data[w] << endl;
      //   }
      // }
    }
  }
  Histogram wordCount;
  for (auto &word : words) {
    wordCount.data[word[0] - 'A']++;
  }
  // cout << "wordCount table" << endl;
  // for (int w = 0; w < 26; w++) {
  //   if (wordCount.data[w] > 0) {
  //     cout << (char)('A' + w) << ": " << wordCount.data[w] << endl;
  //   }
  // }
  // create fun value to count
  pair<int, int> maxFunData = make_pair(-1, -1);
  double maxFunValue = -1;
  int maxFunCount = 0;
  for (int firstRow = 0; firstRow < R; firstRow++) {
    for (int lastRow = firstRow; lastRow < R; lastRow++) {
      for (int firstCol = 0; firstCol < C; firstCol++) {
        for (int lastCol = firstCol; lastCol < C; lastCol++) {
          Histogram current = table[lastRow][lastCol];
          if (firstRow > 0) {
            current -= table[firstRow - 1][lastCol];
          }
          if (firstCol > 0) {
            current -= table[lastRow][firstCol - 1];
          }
          if (firstRow > 0 && firstCol > 0) {
            current += table[firstRow - 1][firstCol - 1];
          }
          int fun1 = 0;
          for (int i = 0; i < 26; i++) {
            fun1 += wordCount.data[i] * current.data[i] * 4;
          }
          int fun2 = lastRow - firstRow + lastCol - firstCol + 2;
          auto funData = makeFunValue(fun1, fun2);
          if (funData == maxFunData) {
            ++maxFunCount;
          } else {
            double funValue = (double)funData.first / funData.second;
            if (funValue > maxFunValue) {
              maxFunValue = funValue;
              maxFunData = funData;
              maxFunCount = 1;
            }
          }
        }
      }
    }
  }
  // TODO: parse
  return to_string(maxFunData.first) + "/" + to_string(maxFunData.second) +
         " " + to_string(maxFunCount);
}
}; // namespace Small

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int R, C, W;
    cin >> R >> C >> W;
    vector<vector<char>> grid(R, vector<char>(C));
    string str;
    getchar();
    for (int i = 0; i < R; i++) {
      getline(cin, str);
      for (int j = 0; j < C; j++) {
        grid[i][j] = str[j];
      }
    }
    vector<string> words;
    for (int i = 0; i < W; i++) {
      getline(cin, str);
      words.emplace_back(move(str));
    }
    cout << "Case #" << t << ": " << Small::solve(R, C, grid, words) << endl;
  }
  return 0;
}