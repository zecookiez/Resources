#include <bits/stdc++.h>
#include "../../../../Content/C++/graph/cycles/DirectedCycle.h"
#include "../../../../Content/C++/graph/representations/StaticGraph.h"
using namespace std;

void test1() {
  mt19937_64 rng(0);
  int V = 2e6, E = 4e6;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = rng() % V, w = rng() % V;
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  DirectedCycle cycle(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v : cycle.cycle) checkSum = 31 * checkSum + v;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  mt19937_64 rng(0);
  int V = 4e6, E = 4e6;
  StaticGraph G(V);
  G.reserveDiEdges(E);
  for (int i = 0; i < E; i++) {
    int v = i, w = (i + 1) % V;
    G.addDiEdge(v, w);
  }
  G.build();
  const auto start_time = chrono::system_clock::now();
  DirectedCycle cycle(G);
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 Passed" << endl;
  cout << "  V: " << V << endl;
  cout << "  E: " << E << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  long long checkSum = 0;
  for (int v : cycle.cycle) checkSum = 31 * checkSum + v;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
