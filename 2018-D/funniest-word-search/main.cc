#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

inline bool funValueEqual(int top1, int bottom1, int top2, int bottom2) {
  return top1 * bottom2 == top2 * bottom1;
}
inline bool funValueGreater(int top1, int bottom1, int top2, int bottom2) {
  return top1 * bottom2 > top2 * bottom1;
}

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

pair<int, int> makeFunData(int top, int down) {
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

pair<int, int> rationalReduce(const pair<int, int> &input) {
  return makeFunData(input.first, input.second);
}

bool operator>(const pair<int, int> &lhs, const pair<int, int> &rhs) {
  return lhs.first * rhs.second > rhs.first * lhs.second;
}

bool funEqual(const pair<int, int> &lhs, const pair<int, int> &rhs) {
  return lhs.first * rhs.second == rhs.first * lhs.second;
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

string solve(int R, int C, vector<string> &grid, vector<string> &words) {
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
    }
  }
  Histogram wordCount;
  for (auto &word : words) {
    wordCount.data[word[0] - 'A']++;
  }
  // create fun value to count
  pair<int, int> maxFunData = make_pair(-1, 1);
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
          auto funData = makeFunData(fun1, fun2);
          if (funData == maxFunData) {
            ++maxFunCount;
          } else {
            if (funData > maxFunData) {
              maxFunData = funData;
              maxFunCount = 1;
            }
          }
        }
      }
    }
  }
  // parse
  return to_string(maxFunData.first) + "/" + to_string(maxFunData.second) +
         " " + to_string(maxFunCount);
}

string solveAlternative(int R, int C, vector<string> &grid,
                        vector<string> &words) {

  // for small case, trim words
  words.resize(min(26, (int)words.size()));
  // create colSum data
  vector<vector<int>> colSum(R, vector<int>(C));
  for (int j = 0; j < C; j++) {
    for (int i1 = 0; i1 < R; i1++) {
      for (auto &word : words) {
        if (word[0] == grid[i1][j]) {
          colSum[i1][j] += 4;
          break;
        }
      }
      if (i1 > 0)
        colSum[i1][j] += colSum[i1 - 1][j];
    }
  }
  // colSum[i1][j] : sum of column length from [0, j] to [i1, j]
  // iterate through all subgrid
  pair<int, int> maxFunData = make_pair(-1, 1);
  int maxFunCount = 0;
  for (int firstRow = 0; firstRow < R; firstRow++) {
    for (int lastRow = firstRow; lastRow < R; lastRow++) {
      for (int firstCol = 0; firstCol < C; firstCol++) {
        int prevFun = 0;
        for (int lastCol = firstCol; lastCol < C; lastCol++) {
          int fun1 = prevFun;
          fun1 += colSum[lastRow][lastCol];
          if (firstRow > 0)
            fun1 -= colSum[firstRow - 1][lastCol];
          prevFun = fun1;
          int fun2 = lastRow - firstRow + lastCol - firstCol + 2;
          auto funData = makeFunData(fun1, fun2);
          if (funData == maxFunData) {
            ++maxFunCount;
          } else {
            if (funData > maxFunData) {
              maxFunData = funData;
              maxFunCount = 1;
            }
          }
        }
      }
    }
  }
  // parse
  return to_string(maxFunData.first) + "/" + to_string(maxFunData.second) +
         " " + to_string(maxFunCount);
}

}; // namespace Small

namespace Large {
string solve(int R, int C, vector<string> &grid, vector<string> &words) {
  // algo:
  // step 1:
  // build word lookup: group by length
  // also add reverse word, so we only consider going right and going down
  int maxK = max(R, C);
  vector<vector<string>> wordLookup(maxK + 1);
  for (auto &word : words) {
    // skip too long words
    if (word.length() <= maxK) {
      // push reversed words
      wordLookup[word.length()].emplace_back(word.rbegin(), word.rend());
      // destory :P
      wordLookup[word.length()].emplace_back(move(word));
    }
  }
  // step 2:
  // build right[i][j][k] and down[i][j][k]
  // means total lengh of word start at i,j, with length <= k
  // going right and down respectively
  int ***tableRCK[2];
  for (int x = 0; x < 2; x++) {
    tableRCK[x] = new int **[R];
    for (int i = 0; i < R; i++) {
      tableRCK[x][i] = new int *[C];
      for (int j = 0; j < C; j++) {
        tableRCK[x][i][j] = new int[maxK + 1]{0};
      }
    }
  }
  auto right = tableRCK[0];
  auto down = tableRCK[1];
  // populate right
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      int currentMaxK = min(maxK, C - j);
      for (int k = 1; k <= currentMaxK; k++) {
        // inherit prev v
        right[i][j][k] = right[i][j][k - 1];
        // start from i,j , is there word with length k exists?
        for (auto &word : wordLookup[k]) {
          // try this word
          bool match = true;
          for (int wi = 0; wi < word.length(); wi++) {
            if (grid[i][j + wi] != word[wi]) {
              match = false;
              break;
            }
          }
          if (match) {
            right[i][j][k] += k;
          }
        }
      }
    }
  }
  // populate down
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      int currentMaxK = min(maxK, R - i);
      for (int k = 1; k <= currentMaxK; k++) {
        // inherit prev v
        down[i][j][k] = down[i][j][k - 1];
        // start from i,j , is there word with length k exists?
        for (auto &word : wordLookup[k]) {
          // try this word
          bool match = true;
          for (int wi = 0; wi < word.length(); wi++) {
            if (grid[i + wi][j] != word[wi]) {
              match = false;
              break;
            }
          }
          if (match) {
            down[i][j][k] += k;
          }
        }
      }
    }
  }
  // step 3:
  // build rightColSum/downRowSum: same as Small::solveAlternative
  // but build for all possible k value
  // process row/col respectively
  auto rightColSum = tableRCK[0];
  auto downRowSum = tableRCK[1];
  // col
  for (int j = 0; j < C; j++) {
    for (int i = 1; i < R; i++) {
      for (int k = 1; k <= maxK; k++) {
        rightColSum[i][j][k] += rightColSum[i - 1][j][k];
      }
    }
  }
  // row
  for (int i = 0; i < R; i++) {
    downRowSum[i][0] = down[i][0];
    for (int j = 1; j < C; j++) {
      for (int k = 1; k <= maxK; k++) {
        downRowSum[i][j][k] += downRowSum[i][j - 1][k];
      }
    }
  }
  // step 4:
  // build total(R2C2), iterate all combination of cols and rows
  int ****totals = new int ***[R];
  for (int i1 = 0; i1 < R; i1++) {
    totals[i1] = new int **[C];
    for (int j1 = 0; j1 < C; j1++) {
      totals[i1][j1] = new int *[R];
      for (int i2 = 0; i2 < R; i2++) {
        totals[i1][j1][i2] = new int[C];
      }
    }
  }
  // during iteration, keep max fun value
  // two pass:
  // consider right only
  // consider down only
  // for pass 1: go right
  // for every pair of rows, iterate column from j1 to j2
  // where j2 start from last
  // if j1 == j2,
  // --> total [i1,j1,i2,j2] = rightColSum[i2,j2,j2-j1+1] -
  //     rightColSum[i1-1,j2,j2-j1+1]
  // for j1 < j2, plus prev value: [i1,j1 + 1,i2,j2]
  for (int i1 = 0; i1 < R; i1++) {
    for (int i2 = i1; i2 < R; i2++) {
      // start from last col
      for (int j2 = C - 1; j2 >= 0; j2--) {
        int fun1 = 0;
        for (int j1 = j2; j1 >= 0; j1--) {
          // compute this
          fun1 += rightColSum[i2][j1][j2 - j1 + 1];
          if (i1 > 0)
            fun1 -= rightColSum[i1 - 1][j1][j2 - j1 + 1];
          totals[i1][j1][i2][j2] = fun1;
        }
      }
    }
  }

  // for pass 2: go down, use downRowSum, start from every pair of cols
  // iterate row from i1 to i2, i2 start from last row
  int maxFun1 = -1, maxFun2 = 1;
  int maxFunCount = 0;
  for (int j1 = 0; j1 < C; j1++) {
    for (int j2 = j1; j2 < C; j2++) {
      // start from last row
      for (int i2 = R - 1; i2 >= 0; i2--) {
        int fun1 = 0;
        for (int i1 = i2; i1 >= 0; i1--) {
          // compute this
          fun1 += downRowSum[i1][j2][i2 - i1 + 1];
          if (j1 > 0)
            fun1 -= downRowSum[i1][j1 - 1][i2 - i1 + 1];
          //
          int totalFun1 = totals[i1][j1][i2][j2] + fun1;
          int fun2 = i2 - i1 + j2 - j1 + 2;
          if (funValueEqual(totalFun1, fun2, maxFun1, maxFun2)) {
            ++maxFunCount;
          } else {
            if (funValueGreater(totalFun1, fun2, maxFun1, maxFun2)) {
              maxFun1 = totalFun1;
              maxFun2 = fun2;
              maxFunCount = 1;
            }
          }
        }
      }
    }
  }
  // cleanup
  for (int i1 = 0; i1 < R; i1++) {
    for (int j1 = 0; j1 < C; j1++) {
      for (int i2 = 0; i2 < R; i2++) {
        delete[] totals[i1][j1][i2];
      }
      delete[] totals[i1][j1];
    }
    delete[] totals[i1];
  }
  delete[] totals;
  for (int x = 0; x < 2; x++) {
    for (int i = 0; i < R; i++) {
      for (int j = 0; j < C; j++) {
        delete[] tableRCK[x][i][j];
      }
      delete[] tableRCK[x][i];
    }
    delete[] tableRCK[x];
  }
  // parse
  auto maxFunData = makeFunData(maxFun1, maxFun2);
  return to_string(maxFunData.first) + "/" + to_string(maxFunData.second) +
         " " + to_string(maxFunCount);
}
} // namespace Large

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int R, C, W;
    cin >> R >> C >> W;
    vector<string> grid(R);
    getchar();
    for (int i = 0; i < R; i++) {
      getline(cin, grid[i]);
    }
    string str;
    vector<string> words(W);
    for (int i = 0; i < W; i++) {
      getline(cin, words[i]);
    }
    cout << "Case #" << t << ": " << Large::solve(R, C, grid, words) << endl;
  }
  return 0;
}