#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <map>

using namespace std;

using Point = pair<int, int>;

void readFile(const string &filename, vector<Point>& byteVec) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int a, b;
        char comma;
        if (ss >> a >> comma >> b) {
            byteVec.emplace_back(a, b);
        }
    }
} // readFile
    

int Dijkstra(int ss, set<Point>& badLocs) {
    map<Point, int> dist;
    set<Point> visited;

    vector<Point> dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    using State = pair<int, Point>;
    priority_queue<State, vector<State>, greater<State>> pq;

    Point start = {0, 0};
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
	pq.pop();
	if (visited.count(u)) continue;
	visited.insert(u);
	if (u.first == ss && u.second == ss) {
	    return d;
	}
	for (auto dir : dirs) {
	    Point nu = {u.first + dir.first, u.second + dir.second};
	    if (badLocs.find(nu) == badLocs.end() 
		    && 0 <= nu.first && nu.first <= ss 
		    && 0 <= nu.second && nu.second <= ss) {
		int nd = d + 1;
		if (!dist.count(nu) || nd < dist[nu]) {
		    dist[nu] = nd;
		    pq.push({nd, nu});
		}
	    }
	}
    }
    return -1;
} // Dijkstra

int sim(const vector<Point>& byteVec, int till) {
    set<Point> badLocs;
    for (int i = 0; i < till; i++) {
        badLocs.insert(byteVec[i]);
    }
    return Dijkstra(70, badLocs);
} // sim

Point search(vector<Point>& byteVec) {
    int lower = 1024;
    int upper = byteVec.size();
    int problemCount = 1000;
    while (problemCount > 0) {
        int centre = (upper + lower) / 2;
	if (centre == lower) {
	    if (sim(byteVec, lower) == -1 || sim(byteVec, upper) != -1) {cout << "error: issue with code" << endl;}
	    return byteVec[lower];
	}
	int bestDist = sim(byteVec, centre);
	if (bestDist == -1) {
	    upper = centre;
	} else {
	    lower = centre;
	}
	problemCount--;
    }
    return {-1, -1};
}  

Point lazySearch(vector<Point>& byteVec) {
    int lower = 1024;
    int upper = byteVec.size();
    int problemCount = 1000;
    for (int i = lower; i < upper; i++) {
        if (sim(byteVec, i) == -1) {return byteVec[i];}
    }
    return {-1, -1};
}  

int main() {
    vector<Point> byteVec;    
    readFile("input.txt", byteVec);
    int bestDist = sim(byteVec, 1024);
    cout << "Shortest distance for 1024 is: " << bestDist << endl;
    Point problem = search(byteVec);
    cout << "Problem point is: " << problem.first << "," << problem.second << endl;

    return 0;
} // main
