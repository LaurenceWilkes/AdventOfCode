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
		    walls.insert(make_pair(i + 1, j));
		} else if (c == 'O') {
		    boxes.insert(make_pair(i, j));
		} else if (c == '@') {
		    robot = make_pair(i, j);
		}
		i++;
		i++;
	    }
	    j++;
	}
    }
} // readFile

bool moveRobotRight(const set<Point>& walls, set<Point>& boxes, Point& robot) {
    Point d = {1, 0};
    Point current = robot;
    while (true) {
	current.first += d.first;
	current.second += d.second;
	if (walls.count(current) > 0) {
	    return false;
	} else if (boxes.count(current) > 0) {
	    current.first += d.first;
	    current.second += d.second;
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
	    boxes.insert(prev);
	    prev.first -= d.first;
	    prev.second -= d.second;
	    boxes.erase(prev);
	    current = prev;
	}
    }
    return true;
} // moveRobot

bool moveRobotLeft(const set<Point>& walls, set<Point>& boxes, Point& robot) {
    Point d = {-1, 0};
    Point current = robot;
    while (true) {
	current.first += d.first;
	current.second += d.second;
	if (walls.count(current) > 0) {
	    return false;
	} else {
	    current.first += d.first;
	    current.second += d.second;
	    if (boxes.count(current) > 0) {
		continute;
	    }
	    current.first -= d.first;
	    current.second -= d.second;
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
	    prev.first -= d.first;
	    prev.second -= d.second;
	    current = prev;
	}
    }
    return true;
} // moveRobot

bool moveRobotUp(const set<Point>& walls, set<Point>& boxes, Point& robot) {
    Point d = {0, -1};
    Point current = robot;
    Point currentl = robot;
    set<Point> boxset;
    while (true) {
	current.first += d.first;
	current.second += d.second;
	currentl.first += d.first;
	currentl.second += d.second;
	currentl.first -= 1;
	if (walls.count(current) > 0) {
	    return false;
	} else {
	    current.first += d.first;
	    current.second += d.second;
	    if (boxes.count(current) > 0) {
		continute;
	    }
	    current.first -= d.first;
	    current.second -= d.second;
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
	    prev.first -= d.first;
	    prev.second -= d.second;
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




