// This one works!
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

typedef pair<int, int> Point; // For position (x, y)

// Structure to represent the Warehouse
class Warehouse {
public:
    set<Point> walls;  // Set to store positions of walls
    set<Point> boxes;  // Set to store positions of boxes
    Point robot;       // Position of the robot

    // Function to read the input file and initialize the warehouse state
    void readFile(string filename, vector<char>& movements) {
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
    }

    bool isBox(Point& pos) {
	if (boxes.count(pos) > 0) {
	    return true;
	}
	Point posNext = {pos.first - 1, pos.second};
	if (boxes.count(posNext) > 0) {
	    pos = posNext;
	    return true;
	}
        return false;
    } // isBox

    bool freeVert(int d, Point pos) {
	if (pos == robot) {
	    Point a = {pos.first, pos.second + d};
	    return freeVert(d, a);
	} else if (isBox(pos)) {
	    Point a1 = {pos.first, pos.second + d};
	    Point a2 = {pos.first + 1, pos.second + d};
	    return freeVert(d, a1) && freeVert(d, a2);
	} else if (walls.count(pos) > 0) {
	    return false;
	} else {
	    return true;
	}
    } // freeVert

    void moveVert(int d, Point pos) {
	if (pos == robot) {
	    Point a = {pos.first, pos.second + d};
	    moveVert(d, a);
	    robot = a;
	} else if (isBox(pos)) {
	    Point a1 = {pos.first, pos.second + d};
	    Point a2 = {pos.first + 1, pos.second + d};
	    moveVert(d, a1);
	    moveVert(d, a2);
	    boxes.erase(pos);
	    boxes.insert(a1);
	}
    } // boxUp

    bool moveRight(Point pos) {
	if (pos == robot) {
	    Point a = {pos.first + 1, pos.second};
	    if (moveRight(a)) {
		robot = a;
		return true;
	    }
	} else if (walls.count(pos) > 0) {
	    return false;
	} else if (isBox(pos)) {
	    Point a1 = {pos.first + 1, pos.second};
	    Point a2 = {pos.first + 2, pos.second};
	    if (moveRight(a2)) {
		boxes.erase(pos);
		boxes.insert(a1);
		return true;
	    }
	} else {
	    return true;
	}
	return false;
    } // moveRight

    bool moveLeft(Point pos) {
	if (pos == robot) {
	    Point a = {pos.first - 1, pos.second};
	    if (moveLeft(a)) {
		robot = a;
		return true;
	    }
	} else if (walls.count(pos) > 0) {
	    return false;
	} else if (isBox(pos)) {
	    Point a1 = {pos.first - 1, pos.second};
	    if (moveLeft(a1)) {
		boxes.erase(pos);
		boxes.insert(a1);
		return true;
	    }
	} else {
	    return true;
	}
	return false;
    } // moveRight

    void printState(char move, int x, int y) {
        cout << "After movement: " << move << endl;
        for (int j = 0; j < y; j++) {
            for (int i = 0; i < x; i++) {
                Point p = {i, j};
                if (walls.count(p)) {
                    cout << "#";
                } else if (boxes.count(p) > 0) {
                    cout << "[]";
		    i++;
                } else if (p == robot) {
                    cout << "@";
                } else {
                    cout << ".";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    // Function to process the movement instructions
    void followPath(const vector<char>& movements) {
        for (char m : movements) {
            switch (m) {
                case '^': if (freeVert(-1, robot)) moveVert(-1, robot); break;  // Up
                case 'v': if (freeVert(1, robot)) moveVert(1, robot); break;   // Down
                case '<': moveLeft(robot); break;  // Left
                case '>': moveRight(robot); break;   // Right
            }
        }
    }

    // Function to calculate the GPS sum for all boxes
    int calculateCoord() {
        int sumGPS = 0;
        for (auto b : boxes) {
            sumGPS += b.first + 100 * b.second;
        }
        return sumGPS;
    }
};

int main() {
    vector<char> movements;
    Warehouse warehouse;

    // Read the input and initialize the warehouse
    warehouse.readFile("input.txt", movements);

    // Process the movement path
    warehouse.followPath(movements);

    // Calculate and output the sum of the GPS coordinates
    int sumGPS = warehouse.calculateCoord();
    cout << "The GPS boxes position is: " << sumGPS << endl;

    return 0;
}

