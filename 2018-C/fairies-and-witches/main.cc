#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

// algo: backtracking all combination
// set size must >= 33
// when get and combination, find max index,
// determine whether all edges except max sum is large than max

int BFcount;
vector<int> BFedgeBuilder;
vector<int> BFnodeUsedCounter;

bool BFcheckPolygon() {
  // check the set of edges in the builder can form the polygon
  int maxIdx = -1;
  int maxEdge = -1;
  for (int i = 0; i < BFedgeBuilder.size(); i++) {
    if (BFedgeBuilder[i] > maxEdge) {
      maxEdge = BFedgeBuilder[i];
      maxIdx = i;
    }
  }
  int remainSum = 0;
  for (int i = 0; i < BFedgeBuilder.size(); i++) {
    if (i != maxIdx) {
      remainSum += BFedgeBuilder[i];
    }
  }
  return remainSum > maxEdge;
}

// choose current node from 1 to N
void BFbacktracking(int N, vector<list<pair<int, int>>> &edges,
                    int currentNode) {
  if (currentNode > N) {
    // we are out-of-nodes
    // TODO: check
    if (BFcheckPolygon()) {
      ++BFcount;
    }
  } else {
    // whether to use currentNode
    if (BFnodeUsedCounter[currentNode] == 0) {
      // can use this node
      // choose any edge
      BFnodeUsedCounter[currentNode]++;
      for (auto &destPair : edges[currentNode]) {
        int dest = destPair.first;
        if (BFnodeUsedCounter[dest] == 0) {
          // cout << "choose edge: " << currentNode << " -> " << dest << endl;
          // use this edge
          BFnodeUsedCounter[dest]++;
          BFedgeBuilder.push_back(destPair.second);
          // BT
          BFbacktracking(N, edges, currentNode + 1);
          // restore
          BFedgeBuilder.pop_back();
          BFnodeUsedCounter[dest]--;
        }
      }
      BFnodeUsedCounter[currentNode]--;
    }
    // not use this node
    // pruning: only remaining edge + currentEdge >= 3
    if (BFedgeBuilder.size() + (N - currentNode) >= 3) {
      BFbacktracking(N, edges, currentNode + 1);
    }
  }
}

int solveBF(int N, vector<list<pair<int, int>>> &edges) {
  BFcount = 0;
  BFedgeBuilder.clear();
  BFnodeUsedCounter.assign(N + 1, 0);
  BFbacktracking(N, edges, 1);
  return BFcount;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N;
    cin >> N;
    vector<list<pair<int, int>>> edges(N + 1);
    for (int i = 1; i <= N; i++) {
      auto &tgr = edges[i];
      int w;
      for (int j = 1; j <= N; j++) {
        cin >> w;
        if (w > 0 && j > i) {
          tgr.push_back(make_pair(j, w));
          // cout << "edge : " << i << " -> " << j << ": " << w << endl;
        }
      }
    }
    cout << "Case #" << t << ": " << solveBF(N, edges) << endl;
  }
  return 0;
}