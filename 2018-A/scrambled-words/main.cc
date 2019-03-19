#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
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
  int length;
};

bool operator==(const WordEntry &lhs, const WordEntry &rhs) {
  return lhs.head == rhs.head && lhs.tail == rhs.tail &&
         lhs.histogram == rhs.histogram;
}

namespace std {
template <> struct hash<WordEntry> {
  size_t operator()(const WordEntry &ent) const {
    size_t res = (hash<char>{}(ent.head) << 1) ^ hash<char>{}(ent.tail);
    for (int i = 0; i < 26; i++) {
      res ^= (hash<int>{}(ent.histogram[i])) << i;
    }
    return res;
  }
};
} // namespace std

WordEntry buildWordEntry(const string &word, int begin, int end) {
  WordEntry res;
  res.length = end - begin;
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

int solve(string &S, vector<string> &words) {
  // group by length
  unordered_map<int, unordered_map<WordEntry, int>> lengthToMap;
  for (auto &word : words) {
    auto &map = lengthToMap[word.length()];
    map[buildWordEntry(word)]++;
  }
  // for every possible length, iterate N
  int totalCount = 0;
  for (auto &lengthToMapData : lengthToMap) {
    int len = lengthToMapData.first;
    auto &currentDict = lengthToMapData.second;
    if (S.length() >= len) {
      // test for entry with len
      auto currentEnt = buildWordEntry(S, 0, len);
      // move forward
      for (int i = len; i <= S.length(); i++) {
        // check first
        auto it = currentDict.find(currentEnt);
        if (it != currentDict.end()) {
          totalCount += it->second;
          currentDict.erase(it);
        }
        // update later
        if (i < S.length()) {
          char newHead = S[i - len + 1];
          currentEnt.head = newHead;
          currentEnt.histogram[newHead - 'a']--;
          currentEnt.histogram[currentEnt.tail - 'a']++;
          currentEnt.tail = S[i];
        }
      }
      // no last update needed
    }
  }
  return totalCount;
}

int solveBF(string &S, vector<string> &words) {
  // BF sol: for every word, find occurance in S
  // Complexity: O(LN)
  // convert all words into map
  unordered_map<WordEntry, int> wordFreq;
  for (auto &word : words) {
    wordFreq[buildWordEntry(word)]++;
  }
  // cout << "Valid histogram size: " << wordFreq.size() << endl;
  //
  int totalCount = 0;
  for (auto &dataPair : wordFreq) {
    auto &wordEnt = dataPair.first;
    int count = dataPair.second;
    // sliding window
    if (S.length() >= wordEnt.length) {
      auto currentEnt = buildWordEntry(S, 0, wordEnt.length);
      // move forward
      bool found = false;
      for (int i = wordEnt.length; i < S.length(); i++) {
        // check first
        if (currentEnt == wordEnt) {
          found = true;
          break;
        }
        // update later
        char newHead = S[i - wordEnt.length + 1];
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
        totalCount += count;
    }
  }
  return totalCount;
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
    // cout << "word size: " << words.size() << endl;
    char S1, S2;
    S1 = getchar();
    getchar();
    S2 = getchar();
    int N, A, B, C, D;
    cin >> N >> A >> B >> C >> D;
    string S = genText(S1, S2, N, A, B, C, D);
    cout << "Case #" << t << ": " << solve(S, words) << endl;
  }
  return 0;
}