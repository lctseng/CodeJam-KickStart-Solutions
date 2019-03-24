#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

#define LC(i) (2 * i)
#define RC(i) (2 * i + 1)
#define PARENT(i) (i >> 1)
#define MID(l, h) ((l + (h - l) / 2))

struct Interval {
  int left, right;
  int length() const { return right - left + 1; }
};

class SegmentTree {
public:
  SegmentTree(vector<int> &nums) {
    dataLen = nums.size();
    if (!dataLen)
      return;
    // find the nearest pow of 2
    int pow = 1;
    while (pow < dataLen)
      pow *= 2;
    tree.resize(pow * 2);
    build(nums, 0, dataLen - 1, 1);
  }

  SegmentTree(int initVal, int dataLen) : dataLen(dataLen) {
    // find the nearest pow of 2
    int pow = 1;
    while (pow < dataLen)
      pow *= 2;
    tree.resize(pow * 2, 0); // resize to all zeros
  }

  int query(int low, int high) { return query(low, high, 0, dataLen - 1, 1); }

  void update(int i, int val) { update(i, val, 0, dataLen - 1, 1); }

  void update(int low, int high, int val) {
    update(low, high, val, 0, dataLen - 1, 1);
  }

private:
  int query(int qlow, int qhigh, int low, int high, int pos) {
    // printf("Query call: low = %d, high = %d , pos = %d\n", low, high, pos);
    if (qlow <= low && qhigh >= high) {
      // fully overlaps
      return tree[pos];
    } else if (qlow > high || qhigh < low) {
      // no overlaps
      return 0;
    } else {
      // partially overlaps
      int mid = MID(low, high);
      return query(qlow, qhigh, low, mid, LC(pos)) +
             query(qlow, qhigh, mid + 1, high, RC(pos));
    }
  }

  // range update
  void update(int qlow, int qhigh, int val, int low, int high, int pos) {
    // printf("Update call: low = %d, high = %d , pos = %d\n", low, high, pos);
    if (low == high) {
      tree[pos] = val;
    } else {
      // partially overlaps
      int mid = MID(low, high);
      update(qlow, qhigh, val, low, mid, LC(pos));
      update(qlow, qhigh, val, mid + 1, high, RC(pos));
      tree[pos] = tree[LC(pos)] + tree[RC(pos)];
    }
  }

  // single value update
  void update(int i, int val, int low, int high, int pos) {
    // printf("update i = %d val = %d low = %d high = %d pos = %d",i, val, low,
    // high, pos); cout << endl;
    if (low == high) {
      tree[pos] = val;
    } else {
      int mid = MID(low, high);
      if (i <= mid)
        update(i, val, low, mid, LC(pos));
      else
        update(i, val, mid + 1, high, RC(pos));
      tree[pos] = tree[LC(pos)] + tree[RC(pos)];
    }
  }

  void build(vector<int> &nums, int low, int high, int pos) {
    if (low == high) {
      tree[pos] = nums[low];
    } else {
      int mid = MID(low, high);
      build(nums, low, mid, LC(pos));
      build(nums, mid + 1, high, RC(pos));
      tree[pos] = tree[LC(pos)] + tree[RC(pos)];
    }
  }

  vector<int> tree;
  int dataLen;
};

namespace Small {
int solve(int N, int Q, vector<Interval> &bookings) {
  // algo
  // sort according to size
  // collect endpoints
  // build segTree, all 0, range 0 to N-1 (N elements)
  // from small to large, query occupied within range
  // minK = min(minK, interval.length() - current_occupied);
  // set<int> endpointSet;
  // for(auto& interval : bookings){
  //   endpointSet.insert(interval.left);
  //   endpointSet.insert(interval.right);
  // }
  // unordered_map<int, int> endpointMap;
  // vector<int> compressedEndpoints;
  // int compressedN = 0;
  // for(int endpoint : endpointSet){
  //   endpointMap[endpoint] = compressedN++;
  //   compressedEndpoints.push_back(endpoint);
  // }
  //
  sort(bookings.begin(), bookings.end(),
       [](const Interval &a, const Interval &b) {
         return a.length() < b.length();
       });
  int minK = bookings[0].length();
  SegmentTree tree(0, N);
  for (int q = 0; q < bookings.size(); q++) {
    // cout << q << endl;
    auto &interval = bookings[q];
    // query
    int occupied = tree.query(interval.left, interval.right);
    // printf("book interval: %d %d, occupied: %d\n", interval.left,
    //       interval.right, occupied);
    minK = min(minK, interval.length() - occupied);
    // update
    for (int i = interval.left; i <= interval.right; i++) {
      tree.update(i, 1);
    }
  }
  return minK;
}
} // namespace Small

namespace Large {
int solve(int N, int Q, vector<Interval> &bookings) {
  // TODO
  return 0;
}
} // namespace Large

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N, Q;
    cin >> N >> Q;
    vector<Interval> bookings(Q);
    for (int i = 0; i < Q; i++) {
      cin >> bookings[i].left >> bookings[i].right;
      bookings[i].left--;  // 0 to N-1
      bookings[i].right--; // 0 to N-1
    }
    cout << "Case #" << t << ": " << Small::solve(N, Q, bookings) << endl;
  }
  return 0;
}