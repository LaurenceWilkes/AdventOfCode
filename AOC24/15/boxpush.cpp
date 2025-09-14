#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

typedef pair<int, int> Point;

void readFile(string filename, set<Point>& walls, set<Point>& boxes, Point& robot, vector<char>& movements) {
    ifstream file(filename);
    string line;
    bool readingMovements = false;
    int j = 0;
    while (getline(file, line)) {
	if (line.empty()) {
	    readingMovements = true;
	}
	if (readingMovements) {
	    for (char c : line) {
		movements.push_back(c);
	    }
	} else {
	    int i = 0;
	    for (char c : line) {
		if (c == '#') {
		    walls.insert(make_pair(i, j));
		} else if (c == 'O') {
		    boxes.insert(make_pair(i, j));
		} else if (c == '@') {
		    robot = make_pair(i, j);
		}
		i++;
	    }
	    j++;
	}
    }
} // readFile

bool moveRobot(char m, const set<Point>& walls, set<Point>& boxes, Point& robot) {
    vector<Point> dir = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    Point d;
    if (m == '^') {
	d = dir[0];
    } else if (m == '>') {
        d = dir[1];
    } else if (m == 'v') {
        d = dir[2];
    } else if (m == '<') {
        d = dir[3];
    } else {
        cout << "problem";
    }
    Point current = robot;
    while (true) {
	current.first += d.first;
	current.second += d.second;
	if (walls.count(current) > 0) {
	    return false;
	} else if (boxes.count(current) > 0) {
	    continue;
	} else {
	    break;
	}
    }
    Point prev = current;
    while (true) {
	prev.first -= d.first;
	prev.second -= d.second;
	if (prev == robot) {
	    robot = current;
	    break;
	} else {
	    boxes.insert(current);
	    boxes.erase(prev);
	    current = prev;
	}
    }
    return true;
} // moveRobot

void followPath(set<Point>& walls, set<Point>& boxes, Point& robot, vector<char>& movements) {
    for (char m : movements) {
        moveRobot(m, walls, boxes, robot);
    }
} // followPath

int calculateCoord(set<Point>& boxes) {
    int sumGPS = 0;
    for (auto b : boxes) {
        sumGPS += b.first + 100 * b.second;
    }
    return sumGPS;
} // calculateCoord

int main() {
    set<Point> walls, boxes;
    Point robot;
    vector<char> movements;
    readFile("input.txt", walls, boxes, robot, movements);

    followPath(walls, boxes, robot, movements);

    int sumGPS = calculateCoord(boxes);

    cout << "The GPS boxes position is: " << sumGPS << endl;

    return 0;
} // main




