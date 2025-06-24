#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;
const int INF = numeric_limits<int>::max() / 2;

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <start_city> <dest1> [dest2 ...]\n";
        return 1;
    }

    // 1) City list & index
    vector<string> cityList = {
      "Delhi","Mumbai","Hong Kong","Tokyo","Bangkok","Singapore",
      "Sydney (Australia)","Melbourne (Australia)","Dubai","Cairo",
      "Moscow","Stockholm","Seoul","Canada","San Francisco (USA)",
      "Washington DC (USA)","LA (USA)","Chicago (USA)","New York (USA)",
      "Cape Town (South Africa)","Rio (Brazil)","Santiago","Mexico City",
      "Peru","Miami (USA)","London","Berlin (Germany)","Rome","Paris",
      "Spain","Havana"
    };
    int N = cityList.size();
    unordered_map<string,int> idx;
    for (int i = 0; i < N; i++) idx[cityList[i]] = i;

    // 2) Build costMat & nextHop
    vector<vector<int>> costMat(N, vector<int>(N, INF));
    vector<vector<int>> nextHop(N, vector<int>(N, -1));
    auto add = [&](const string &a, const string &b, int c) {
      int i = idx[a], j = idx[b];
      costMat[i][j] = min(costMat[i][j], c);
      costMat[j][i] = min(costMat[j][i], c);
      nextHop[i][j] = j;
      nextHop[j][i] = i;
    };

    // — your 86 add(...) calls —
    add("Delhi","Dubai",7500);
    add("Delhi","Mumbai",1400);
    add("Delhi","Bangkok",6300);
    add("Delhi","Moscow",20000);
    add("Delhi","London",25500);

    add("Mumbai","Hong Kong",17000);
    add("Mumbai","Cape Town (South Africa)",40000);
    add("Mumbai","London",24500);

    add("Dubai","Cairo",13000);
    add("Dubai","New York (USA)",61000);

    add("Cairo","London",13000);
    add("Moscow","Stockholm",6900);
    add("Stockholm","London",3500);
    add("Stockholm","Berlin (Germany)",5300);

    add("Hong Kong","Tokyo",9350);
    add("Bangkok","Singapore",5500);
    add("Singapore","Melbourne (Australia)",47000);
    add("Singapore","Sydney (Australia)",19600);

    add("Sydney (Australia)","Melbourne (Australia)",3500);

    add("Tokyo","Sydney (Australia)",66000);
    add("Tokyo","Seoul",9200);
    add("Tokyo","Canada",32350);
    add("Tokyo","New York (USA)",29300);
    add("Tokyo","LA (USA)",25000);
    add("Tokyo","San Francisco (USA)",25000);

    add("Canada","LA (USA)",8500);
    add("Canada","San Francisco (USA)",8000);
    add("Canada","Chicago (USA)",7700);
    add("Canada","Washington DC (USA)",9100);

    add("London","Rio (Brazil)",19200);
    add("London","Washington DC (USA)",21000);
    add("London","New York (USA)",10000);
    add("London","Paris",2800);
    add("London","Rome",1750);
    add("London","Berlin (Germany)",1400);

    add("Rome","Cape Town (South Africa)",19800);
    add("Rome","Paris",1000);
    add("Rome","Berlin (Germany)",4400);

    add("Paris","Berlin (Germany)",3500);
    add("Paris","Cape Town (South Africa)",21300);
    add("Paris","New York (USA)",13500);
    add("Paris","Washington DC (USA)",15500);

    add("Cape Town (South Africa)","Rio (Brazil)",52500);
    add("Cape Town (South Africa)","Washington DC (USA)",27400);
    add("Cape Town (South Africa)","New York (USA)",27800);
    //add("Cape Town (South Africa)","Dubai",22000); // comment out if undesired

    add("Rio (Brazil)","Santiago",7500);
    add("Rio (Brazil)","Spain",16500);

    add("Santiago","Peru",4400);
    add("Santiago","Mexico City",1700);

    add("Spain","Peru",19900);
    add("Spain","London",3700);
    add("Spain","Berlin (Germany)",1000);
    add("Spain","Paris",2500);
    add("Spain","Rome",1250);
    add("Spain","Dubai",14450);
    add("Spain","Delhi",17800);
    add("Spain","New York (USA)",10500);

    add("Mexico City","New York (USA)",11000);
    add("Mexico City","LA (USA)",7300);
    add("Mexico City","Washington DC (USA)",19300);
    add("Mexico City","Chicago (USA)",8700);
    add("Mexico City","Peru",8250);
    add("Mexico City","Havana",12750);

    add("Peru","Miami (USA)",6500);
    add("Peru","Washington DC (USA)",7500);
    add("Peru","Havana",21000);
    add("Peru","LA (USA)",20000);
    add("Peru","San Francisco (USA)",18000);

    add("New York (USA)","Delhi",26000);
    add("New York (USA)","Chicago (USA)",2500);
    add("New York (USA)","Washington DC (USA)",5500);
    add("New York (USA)","San Francisco (USA)",6400);
    add("New York (USA)","LA (USA)",4200);
    add("New York (USA)","Miami (USA)",2500);
    add("New York (USA)","Canada",5200);

    add("Chicago (USA)","Delhi",26500);
    add("Chicago (USA)","LA (USA)",2750);
    add("Chicago (USA)","San Francisco (USA)",4500);
    add("Chicago (USA)","Washington DC (USA)",2250);

    add("LA (USA)","Washington DC (USA)",4000);
    add("LA (USA)","San Francisco (USA)",2600);
    add("LA (USA)","Miami (USA)",3750);
    add("LA (USA)","Delhi",24000);
    add("LA (USA)","Dubai",31500);
    add("LA (USA)","San Francisco (USA)",7000);

    // 3) Floyd–Warshall multi-leg shortest paths
    for (int i = 0; i < N; i++) {
      costMat[i][i] = 0;
      nextHop[i][i] = i;
    }
    for (int k = 0; k < N; k++)
      for (int i = 0; i < N; i++) if (costMat[i][k] < INF)
        for (int j = 0; j < N; j++) {
          int via = costMat[i][k] + costMat[k][j];
          if (via < costMat[i][j]) {
            costMat[i][j] = via;
            nextHop[i][j] = nextHop[i][k];
          }
        }

    // 4) Parse start & destinations
    string start = argv[1];
    if (!idx.count(start)) { cerr << "Unknown start city\n"; return 1; }
    vector<int> cities;
    for (int i = 2; i < argc; i++) {
      string d = argv[i];
      if (!idx.count(d)) { cerr << "Unknown city: " << d << "\n"; return 1; }
      cities.push_back(idx[d]);
    }

    // 5) Build reduced K-node graph
    int K = (int)cities.size() + 1;
    vector<int> mapToFull(K);
    mapToFull[0] = idx[start];
    for (int i = 1; i < K; i++) mapToFull[i] = cities[i-1];

    vector<vector<int>> c2(K, vector<int>(K, INF));
    for (int i = 0; i < K; i++)
      for (int j = 0; j < K; j++)
        c2[i][j] = costMat[ mapToFull[i] ][ mapToFull[j] ];

    // 6) Bitmask DP (never revisit 0 mid-tour)
    int FULL = 1<<K;
    vector<vector<int>> dp(FULL, vector<int>(K, INF)), parent(FULL, vector<int>(K,-1));
    dp[1][0] = 0;
    for (int mask = 0; mask < FULL; mask++) {
      for (int u = 0; u < K; u++) {
        if (!(mask & (1<<u)) || dp[mask][u] >= INF) continue;
        for (int v = 1; v < K; v++) {            // v=1…K-1 only
          if (mask & (1<<v)) continue;
          int nm = mask | (1<<v);
          int nd = dp[mask][u] + c2[u][v];
          if (nd < dp[nm][v]) {
            dp[nm][v] = nd;
            parent[nm][v] = u;
          }
        }
      }
    }

    // 7) Close the tour
    int best = INF, endAt = -1;
    for (int u = 1; u < K; u++) {
      int cost = dp[FULL-1][u] + c2[u][0];
      if (cost < best) {
        best = cost;
        endAt = u;
      }
    }
    if (best >= INF/2) {
      cout << "No feasible tour found\n";
      return 0;
    }

    // 8) Reconstruct reduced path
    vector<int> reduced;
    int mask = FULL-1, cur = endAt;
    while (mask) {
      reduced.push_back(cur);
      int p = parent[mask][cur];
      mask ^= (1<<cur);
      cur = p;
    }
    reduced.push_back(0);
    reverse(reduced.begin(), reduced.end());

    // 9) **Correct** expansion into fullPath
    vector<int> fullPath;
    // 9a) start at city 0
    fullPath.push_back(mapToFull[0]);
    // 9b) for each next reduced node, walk via nextHop
    for (int i = 1; i < (int)reduced.size(); i++) {
      int target = mapToFull[ reduced[i] ];
      int x = fullPath.back();
      while (x != target) {
        x = nextHop[x][target];
        fullPath.push_back(x);
      }
    }

    // 10) Print
    cout << "Optimal route: ";
    for (int ix : fullPath) cout << cityList[ix] << " -> ";
    cout << cityList[mapToFull[0]] << "\n";
    cout << "Total cost: " << best << "\n";
    return 0;
}
