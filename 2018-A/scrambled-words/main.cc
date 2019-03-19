#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// static const auto _ = []() {
//   std::ios::sync_with_stdio(false);
//   std::cin.tie(nullptr);
//   return nullptr;
// }();

struct WordEntry {
  char head, tail;
  array<int, 26> histogram = {0};

  bool operator==(const WordEntry &rhs) {
    return head == rhs.head && tail == rhs.tail && histogram == rhs.histogram;
  }
};

WordEntry buildWordEntry(const string &word, int begin, int end) {
  WordEntry res;
  res.head = word[begin];
  res.tail = word[end - 1];
  for (int i = begin + 1; i < end - 1; i++) {
    res.histogram[word[i] - 'a']++;
  }
  return res;
}

WordEntry buildWordEntry(const string &word) {
  return buildWordEntry(word, 0, word.length());
}

int solveBF(string &S, vector<string> &words) {
  // BF sol: for every word, find occurance in S
  // Complexity: O(LN)
  int count = 0;
  for (auto &word : words) {
    auto wordEnt = buildWordEntry(word);
    // sliding window
    if (S.length() >= word.length()) {
      auto currentEnt = buildWordEntry(S, 0, word.length());
      // move forward
      bool found = false;
      for (int i = word.length(); i < S.length(); i++) {
        // check first
        if (currentEnt == wordEnt) {
          found = true;
          break;
        }
        // update later
        char newHead = S[i - word.length() + 1];
        currentEnt.head = newHead;
        currentEnt.histogram[newHead - 'a']--;
        currentEnt.histogram[currentEnt.tail - 'a']++;
        currentEnt.tail = S[i];
      }
      // last update if not found
      if (!found && currentEnt == wordEnt) {
        found = true;
      }
      if (found)
        ++count;
    }
  }
  return count;
}

string genText(char S1, char S2, int64_t N, int64_t A, int64_t B, int64_t C,
               int64_t D) {
  string res;
  res.push_back(S1);
  res.push_back(S2);
  int64_t xi_1 = S2, xi_2 = S1;
  for (int i = 3; i <= N; i++) {
    int64_t xi = (A * xi_1 + B * xi_2 + C) % D;
    res.push_back((char)(97 + (xi % 26)));
    xi_2 = xi_1;
    xi_1 = xi;
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int L;
    cin >> L;
    // read newline
    getchar();
    vector<string> words;
    while (true) {
      // read one word
      string s;
      char c;
      while ((c = getchar()) && c != ' ' && c != '\n') {
        s.push_back(c);
      }
      words.push_back(move(s));
      if (c == '\n')
        break;
    }
    cout << "word size: " << words.size() << endl;
    char S1, S2;
    S1 = getchar();
    getchar();
    S2 = getchar();
    int N, A, B, C, D;
    cin >> N >> A >> B >> C >> D;
    string S = genText(S1, S2, N, A, B, C, D);
    cout << "Case #" << t << ": " << solveBF(S, words) << endl;
  }
  return 0;
}