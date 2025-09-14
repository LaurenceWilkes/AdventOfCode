#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

void readMap(string filename, vector<vector<int>>& mat) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
	vector<int> lineVec;
	lineVec.clear();
        for (char c : line) {
            lineVec.push_back(c - '0');
        }
	mat.push_back(lineVec);
    }
} // readMap

void countHeads(vector<pair<int, int>> currentPath, set<pair<int, int>>& nines, const vector<vector<int>>& mat) {
    int y = mat.size();
    int x = mat.at(0).size();
    vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    pair<int, int> currentPos = currentPath.back();
    int currentVal = mat.at(currentPos.first).at(currentPos.second);
    for (auto d : dirs) {
	pair<int, int> next = {currentPos.first + d.first, currentPos.second + d.second};
        if (0 <= next.first && next.first < y && 0 <= next.second && next.second < x) {
	    int nextVal = mat.at(next.first).at(next.second);
	    if (mat.at(next.first).at(next.second) == currentVal + 1) {
	        if (nextVal == 9) {
	            nines.insert(next);
	        } else {
		    vector<pair<int, int>> nextPath = currentPath;
		    nextPath.push_back(next);
	            countHeads(nextPath, nines, mat);
	        }
	    }
        }
    }
} // countHeads

int countPaths(pair<int, int> currentPos, map<pair<int, int>, int>& mem, const vector<vector<int>>& mat) {
    if (mem.count(currentPos) > 0) {
        return mem[currentPos];
    }
    int y = mat.size();
    int x = mat.at(0).size();
    vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int sumVal = 0;

    int currentVal = mat.at(currentPos.first).at(currentPos.second);
    for (auto d : dirs) {
	pair<int, int> next = {currentPos.first + d.first, currentPos.second + d.second};
        if (0 <= next.first && next.first < y && 0 <= next.second && next.second < x) {
	    int nextVal = mat.at(next.first).at(next.second);
	    if (mat.at(next.first).at(next.second) == currentVal + 1) {
	        if (nextVal == 9) {
		    sumVal++;
	        } else {
	            sumVal += countPaths(next, mem, mat);
	        }
	    }
        }
    }
    mem[currentPos] = sumVal;
    return sumVal;
} // countHeads

int main() {
    vector<vector<int>> mat;
    readMap("input.txt", mat);

    int y = mat.size();
    int x = mat[0].size();

    // part 1
    int currentSum = 0;
    for (int i = 0; i < y; i++) {
	for (int j = 0; j < x; j++) {
	    if (mat[i][j] == 0) {
		set<pair<int, int>> nines;
		nines.clear();

		vector<pair<int, int>> cp = {{i, j}};
		countHeads(cp, nines, mat);
		currentSum += nines.size();
	    }
	}
    }
    cout << "The sum of reachable nines is: " << currentSum << endl;

    // part 2
    int currentSum2 = 0;
    for (int i = 0; i < y; i++) {
	for (int j = 0; j < x; j++) {
	    if (mat[i][j] == 0) {
		pair<int, int> currentPosition = {i, j};
		map<pair<int, int>, int> mem;
		currentSum2 += countPaths(currentPosition, mem, mat);
	    }
	}
    }
    cout << "The sum of ratings is: " << currentSum2 << endl;
    return 0;
} // main

