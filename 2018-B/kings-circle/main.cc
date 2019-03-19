#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

// algo: find all invalid triplet, minus by total combination
// to find invalid triplet, iterate every pair of point
// create a bounding box of them (two corner)
// count number of point that is strictly inside
// using 2D cumulative array (sum area table)

struct Point {
  int V, H;
  Point(int V = 0, int H = 0) : V(V), H(H) {}
};

vector<Point> genPoints(int N, int V1, int H1, int A, int B, int C, int D,
                        int E, int F, int M) {
  vector<Point> res;
  res.push_back({V1, H1});
  for (int i = 2; i <= N; i++) {
    int V = (A * V1 + B * H1 + C) % M;
    int H = (D * V1 + E * H1 + F) % M;
    res.push_back({V, H});
    V1 = V, H1 = H;
  }
  return res;
}

struct BoundingBox {
  Point topLeft, topRight, bottomLeft, bottomRight;
};

BoundingBox findInsideBoundingBox(const Point &p1, const Point &p2) {
  int width = abs(p1.V - p2.V);
  int finalV = min(p1.V, p2.V);
  int height = abs(p1.H - p2.H);
  int finalH = min(p1.H, p2.H);
  if (width > 0 && height > 0) {
    return {{finalV, finalH},
            {finalV + width - 1, finalH},
            {finalV, finalH + height - 1},
            {finalV + width - 1, finalH + height - 1}};
  } else {
    // zero-area bounding
    return BoundingBox();
  }
}

ostream &operator<<(ostream &out, const Point &p) {
  out << "(" << p.V << "," << p.H << ")";
  return out;
}

int64_t solve(vector<Point> &points, int M) {
  int64_t N = points.size();
  int64_t totalComb = N * (N - 1) * (N - 2) / 6;
  // build cumulative table
  vector<vector<int>> areaTable(M, vector<int>(M, 0));
  for (auto &p : points) {
    areaTable[p.V][p.H]++;
  }
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      // add from left and top
      if (j > 0)
        areaTable[i][j] += areaTable[i][j - 1];
      if (i > 0)
        areaTable[i][j] += areaTable[i - 1][j];
      if (i > 0 && j > 0)
        areaTable[i][j] -= areaTable[i - 1][j - 1];
    }
  }
  // helper
  auto lookupAreaTable = [&](const Point &p) { return areaTable[p.V][p.H]; };
  // find invalid triplet, iterate every pair of point
  int64_t invalidCount = 0;
  for (int i = 0; i < points.size() - 1; i++) {
    for (int j = i + 1; j < points.size(); j++) {
      auto box = findInsideBoundingBox(points[i], points[j]);
      invalidCount +=
          lookupAreaTable(box.bottomRight) - lookupAreaTable(box.bottomLeft) -
          lookupAreaTable(box.topRight) + lookupAreaTable(box.topLeft);
    }
  }
  return totalComb - invalidCount;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N, V1, H1, A, B, C, D, E, F, M;
    cin >> N >> V1 >> H1 >> A >> B >> C >> D >> E >> F >> M;
    vector<Point> points = genPoints(N, V1, H1, A, B, C, D, E, F, M);
    cout << "Case #" << t << ": " << solve(points, M) << endl;
  }
  return 0;
}