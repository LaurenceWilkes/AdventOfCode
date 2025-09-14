#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <set>
#include <string>

using namespace std;

typedef pair<int, int> Point;

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

void readFile(string filename, vector<Point>& positions, vector<Point>& velocities) {
    ifstream file(filename);

    string line;
    regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");

    while (getline(file, line)) {
	smatch match;
	if (regex_match(line, match, pattern)) {
	    Point pos = {stoi(match[1]), stoi(match[2])};
	    Point vel= {stoi(match[3]), stoi(match[4])};

	    positions.push_back(pos);
	    velocities.push_back(vel);
	}
    }
} // readFile

Point predictPosition(int steps, Point pos, Point vel, int x, int y) {
    int nf = positive_modulo(pos.first + vel.first * steps, x);
    int ns = positive_modulo(pos.second + vel.second * steps, y);
    return make_pair(nf, ns);
} // predictPosition

void updatePosition(int steps, vector<Point>& positions, vector<Point>& velocities, int x, int y) {
    for (int i = 0; i < positions.size(); i++) {
	positions[i] = predictPosition(steps, positions[i], velocities[i], x, y);
    }
} // updatePosition

void drawPositions(vector<Point>& positions, int x, int y) {
    vector<vector<bool>> grid(y, vector<bool>(x, false));
    for (auto p : positions) {
        grid[p.second][p.first] = true;
    }
    for (int j = 0; j < y; j++) {
	for (int i = 0; i < x; i++) {
	    if (grid[j][i]) {
		cout << "#";
	    } else {
	        cout << ".";
	    }
	}
	cout << endl;
    }
} // drawPositions

int simulateQuads(int steps, vector<Point>& positions, vector<Point>& velocities, int x, int y) {
    vector<int> quads(4, 0);
    int hx = x/2;
    int hy = y/2;
    for (int i = 0; i < positions.size(); i++) {
	Point finalPos = predictPosition(steps, positions[i], velocities[i], x, y);
	if (finalPos.first < hx && finalPos.second < hy) {
	    quads[1]++;
	} else if (finalPos.first < hx && finalPos.second > hy) {
	    quads[2]++;
	} else if (finalPos.first > hx && finalPos.second > hy) {
	    quads[3]++;
	} else if (finalPos.first > hx && finalPos.second < hy) {
	    quads[4]++;
	}
    }
    return quads[1] * quads[2] * quads[3] * quads[4];
} // simulate

bool testPositions(const vector<Point>& positions) {
    set<Point> setPositions(positions.begin(), positions.end());
    for (auto p : setPositions) {
	int qual = setPositions.count(make_pair(p.first + 1, p.second)) +
		   setPositions.count(make_pair(p.first, p.second + 1)) +
		   setPositions.count(make_pair(p.first - 1, p.second)) +
                   setPositions.count(make_pair(p.first, p.second - 1)) +
		   setPositions.count(make_pair(p.first + 1, p.second + 1)) +
                   setPositions.count(make_pair(p.first + 1, p.second - 1)) +
                   setPositions.count(make_pair(p.first - 1, p.second + 1)) +
                   setPositions.count(make_pair(p.first - 1, p.second - 1)) +
		   setPositions.count(make_pair(p.first + 2, p.second)) +
                   setPositions.count(make_pair(p.first, p.second + 2)) +
                   setPositions.count(make_pair(p.first - 2, p.second)) +
                   setPositions.count(make_pair(p.first, p.second - 2)) +
		   setPositions.count(make_pair(p.first + 2, p.second + 2)) +
                   setPositions.count(make_pair(p.first + 2, p.second - 2)) +
                   setPositions.count(make_pair(p.first - 2, p.second + 2)) +
                   setPositions.count(make_pair(p.first - 2, p.second - 2));
	if (qual >= 12) {
	    return true;
	}
    }
    return false;
} // testPositions


int main() {
    vector<Point> positions, velocities;
    readFile("input.txt", positions, velocities);
    int x = 101;
    int y = 103;

    int quadProd = simulateQuads(100, positions, velocities, x, y);

    cout << "The safety factor is: " << quadProd << endl;

    for (int i = 1; i < 10000; i++) {
	updatePosition(1, positions, velocities, x, y);
	if (testPositions(positions)) {
	    cout << "After " << i << " steps, we have:" << endl;
	    drawPositions(positions, x, y);
	    cout << endl;
	    break;
	}
    }

//    int i = 851;
//    cout << "After " << i << " steps, we have:" << endl;
//    updatePosition(i, positions, velocities, x, y);
//    drawPositions(positions, x, y);
//    cout << endl;

    return 0;
} // main
