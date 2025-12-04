#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

pair<int, int> readData(string filename, vector<vector<char>>& dataMat, set<pair<int, int>>& hashMap) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
	vector<char> lineVec;
	lineVec.clear();
        for (char c : line) {
	    lineVec.push_back(c);
        }
	dataMat.push_back(lineVec);
    }

    int cols = dataMat[0].size();
    int rows = dataMat.size();
    pair<int, int> guardPos;

    for (int j = 0; j < cols; j++) {
	for (int i = 0; i < rows; i++) {
	    if (dataMat[i][j] == '#') {
	        hashMap.insert(make_pair(i, j));
	    } else if (dataMat[i][j] == '^') {
	        guardPos = make_pair(i, j);
		dataMat[i][j] = '.';
	    }
	}
    }

    return guardPos;
} // readData

int simPatrol(pair<int, int> gp, const set<pair<int, int>>& hashMap, int rows, int cols) {
    vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    int cd = 0;
    set<pair<int, int>> visited;

    while (gp.first >= 0 && gp.first < rows && gp.second >= 0 && gp.second < cols) {
	visited.insert(gp);
	pair<int, int> next = {gp.first + dirs[cd].first, gp.second + dirs[cd].second};
	while (hashMap.count(next) > 0) {
	    ++cd %= 4;
	    next = make_pair(gp.first + dirs[cd].first, gp.second + dirs[cd].second);
	}
	gp.first += dirs[cd].first;
	gp.second += dirs[cd].second;
    }

    return visited.size();

} // simPatrol

bool isLoop(pair<int, int> gp, const set<pair<int, int>>& hashMap, int rows, int cols) {
    vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    int cd = 0;
    set<pair<pair<int, int>, int>> visited;

    while (gp.first >= 0 && gp.first < rows && gp.second >= 0 && gp.second < cols) {
	if (visited.count(make_pair(gp, cd)) > 0) {
	    return true;
	}
	visited.insert(make_pair(gp, cd));
	pair<int, int> next = {gp.first + dirs[cd].first, gp.second + dirs[cd].second};
	while (hashMap.count(next) > 0) {
	    ++cd %= 4;
	    next = make_pair(gp.first + dirs[cd].first, gp.second + dirs[cd].second);
	}
	gp.first += dirs[cd].first;
	gp.second += dirs[cd].second;
    }

    return false;
} // simPatrol

int main() {
    vector<vector<char>> dataMat;
    set<pair<int, int>> hashMap;
    pair<int, int> guardPos = readData("input.txt", dataMat, hashMap);

    int cols = dataMat[0].size();
    int rows = dataMat.size();

    // part 1
    int visited = simPatrol(guardPos, hashMap, rows, cols);

    cout << "positions visited: " << visited << endl;

    // part 2
    int extra = 0;

    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
	    pair<int, int> ij = {i, j};
	    if (hashMap.count(ij) == 0 && ij != guardPos) {
		hashMap.insert(ij);
		if (isLoop(guardPos, hashMap, rows, cols)) extra++;
		hashMap.erase(ij);
	    }
        }
    }

    cout << "possible extra loops: " << extra << endl;

    return 0;
} // main
