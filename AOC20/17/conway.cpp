#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>

using namespace std;

struct Point {
    int x;
    int y;
    int z;
    int w;

    Point(int x = 0, int y = 0, int z = 0, int w = 0)
        : x(x), y(y), z(z), w(w) {}

    bool operator==(const Point& other) const {
	return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator<(const Point& other) const {
	if (x != other.x) return x < other.x;
	if (y != other.y) return y < other.y;
	if (z != other.z) return z < other.z;
	return w < other.w;
    }

    int neighboursIn(const vector<Point>& points) const {
        int count = 0;
        for (const auto& p : points) {
            // Check each coordinate difference is at most 1
            if (abs(x - p.x) <= 1 &&
                abs(y - p.y) <= 1 &&
                abs(z - p.z) <= 1 &&
                abs(w - p.w) <= 1 &&
                (x != p.x || y != p.y || z != p.z || w != p.w)) {  // Exclude the point itself
                ++count;
            }
        }
        return count;
    }

    set<Point> neighbourSet() {
	vector<int> vals = {-1, 0, 1};
	set<Point> neigh;
	for (auto a : vals) {
	    for (auto b : vals) {
	        for (auto c : vals) {
		    for (auto d : vals) {
			neigh.insert(Point(x + a, y + b, z + c, w + d));
		    }
	        }
	    }
	}
	return neigh;
    }
};

vector<Point> readFile(string filename) {
    ifstream file(filename);

    string line;
    vector<Point> points;

    int i = 0;
    int j = 0;

    while (getline(file, line)) {
	j = 0;
	for (char c : line) {
	    if (c == '#') {
	        points.push_back(Point(i, j, 0, 0));
	    }
	    j++;
	}
	i++;
    }
    return points;
} // readFile

void simStep(vector<Point>& currentPoints) {
    vector<Point> newPoints;
    set<Point> pointsToCheck;

    for (auto cp : currentPoints) {
	for (auto np : cp.neighbourSet()) {
	    pointsToCheck.insert(np);
	}
    }

    for (auto p : pointsToCheck) {
	bool incp = false;
	for (auto cp : currentPoints) {
	    if (p == cp) {
	        incp = true;
		break;
	    }
	}
	int nicp = p.neighboursIn(currentPoints);
	if (nicp == 3 || (incp && nicp == 2)) {
	    newPoints.push_back(p);
	}
    }
    currentPoints = newPoints;
} // simStep

int main() {
    vector<Point> currentPoints = readFile("input.txt");

    for (int i = 0; i < 6; i++) {
        simStep(currentPoints);
    }

    cout << "Number of points after 6 steps: " << currentPoints.size();

    return 0;
} // main

