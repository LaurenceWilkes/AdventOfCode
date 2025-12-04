#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>


using namespace std;

int readfile(string filename, map<pair<int, int>, bool>& locs) {
    ifstream f(filename);
    string line; 

    while (getline(f, line)) {
	int n = line.size();
	int ci = 0;
	pair<int, int> loc = {0, 0};
	while (ci < n) {
	    pair<int, int> dir;
	    if (line[ci] == 'n') {
		ci++;
		if (line[ci] == 'e') {dir = make_pair(0, 1);} else {dir = make_pair(-1, 1);}
	    } else if (line[ci] == 's') {
		ci++;
		if (line[ci] == 'e') {dir = make_pair(1, -1);} else {dir = make_pair(0, -1);}
	    } else if (line[ci] == 'e') {
		dir = make_pair(1, 0);
	    } else {
		dir = make_pair(-1, 0);
	    }
	    ci++;

	    loc.first += dir.first;
	    loc.second += dir.second;
	}
//	cout << loc.first << " " << loc.second << endl;
	if (locs.count(loc)) {locs[loc] = !locs[loc];} else {locs[loc] = true;}
    }

    int count = 0;
    for (const auto &kv : locs) {
        if (kv.second) {++count;}
    }
    return count;
}

bool checkTile(const pair<int, int>& tile, const map<pair<int, int>, bool>& locs) {
    int count = 0;
    vector<pair<int, int>> dirs = {{0, 1}, {-1, 1}, {1, -1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& d : dirs) {
        pair<int, int> nt = {tile.first + d.first, tile.second + d.second};
	if (auto itr = locs.find(nt); itr != locs.end()) {count += (int) itr -> second;}
    }
    auto itr = locs.find(tile);
    if (itr != locs.end() && itr -> second) {
	return (count == 0 || count > 2);
    } else {
        return (count == 2);
    }
} // checkTile

int daySim(map<pair<int, int>, bool>& locs) {
    set<pair<int, int>> toFlip;
    static const pair<int, int> dirs[6] = {{0, 1}, {-1, 1}, {1, -1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto &kv : locs) {
        if (kv.second) {
	    pair<int, int> current = kv.first;
	    if (checkTile(current, locs)) {toFlip.insert(current);}
	    for (auto [dx, dy] : dirs) {
		pair<int, int> adj = {current.first + dx, current.second + dy};
	        if (checkTile(adj, locs)) {toFlip.insert(adj);}
	    }
	}
    }
    set<pair<int, int>>::iterator itr;
    for (itr = toFlip.begin(); itr != toFlip.end(); itr++) {
	if (locs.find(*itr) != locs.end()) {locs[*itr] = !locs[*itr];} else {locs[*itr] = true;}
    }
    int count = 0;
    for (const auto &kv : locs) {
        if (kv.second) {++count;}
    }
    return count;
} // daySim

int main() {
    map<pair<int, int>, bool> locs;
    int count = readfile("input.txt", locs);
    cout << "Count is: " << count << endl;

    for (int i = 1; i <= 100; i++) {
        int count = daySim(locs);
	if (i % 10 == 0) {cout << "Intermediate count after day " << i << " is: " << count << endl;}
    } 

    return 0;
} // main
