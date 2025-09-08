#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

vector<bool> readFile(string filename, int &rows, int &cols) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file");
    }

    string line;
    vector<vector<char>> tempcopy;
    int i = 0;

    while (getline(file, line)) {
        tempcopy.push_back({});
        for (char elt : line) {
            tempcopy[i].push_back(elt);
        }
        i++;
    }

    rows = tempcopy.size();
    cols = tempcopy[0].size();

    vector<bool> chair(rows * cols, false);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (tempcopy[i][j] == 'L') {
                chair[i * cols + j] = true;
            }
        }
    }

    return chair;
}

int neighbours(int i, int j, const vector<bool>& chair, const vector<bool>& occupied, int rows, int cols) {
    int n = 0;
    int directions[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

    for (auto& dir : directions) {
        int x = i + dir[0];
        int y = j + dir[1];

        while (x >= 0 && x < rows && y >= 0 && y < cols) {
            if (chair[x * cols + y]) {  // Found the first chair in this direction
                if (occupied[x * cols + y]) {  // Check if this chair is occupied
                    n++;
                }
                break;  // Stop searching in this direction
            }
            x += dir[0];
            y += dir[1];
        }
    }

    if (i == 1 && j == 1) {
        cout << i << " " << j << "   n - " << n << " occupied - " << occupied[i * cols + j] << endl;
    } 

    return n;
}

void act(vector<bool>& occupied, vector<bool>& changes, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = i * cols + j;
            bool result = occupied[idx] ^ changes[idx];
            occupied[idx] = result;
            changes[idx] = false;
        }
    }
}

void disp(const vector<bool>& occupied, const vector<bool>& chair, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (occupied[i * cols + j]) {
                cout << '#';
            } else if (chair[i * cols + j]) {
                cout << 'L';
            } else {
                cout << '.';
            }
        }
        cout << endl;
    }
    cout << endl;
}

void dispb(const vector<bool>& occupied, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (occupied[i * cols + j]) {
                cout << '+';
            } else {
                cout << '.';
            }
        }
        cout << endl;
    }
    cout << endl;
}

int sim(const vector<bool>& chair, int rows, int cols) {
    vector<bool> occupied = chair;
    vector<bool> changes(rows * cols, false);

    int prevchanges = 1;

    while (prevchanges > 0) {
	// disp(occupied, chair, rows, cols);
	dispb(occupied, rows, cols);
        prevchanges = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int idx = i * cols + j;
                if (occupied[idx] && neighbours(i, j, occupied, chair, rows, cols) >= 5) {
                    changes[idx] = true;
                    prevchanges++;
                } else if (!occupied[idx] && chair[idx] && neighbours(i, j, occupied, chair, rows, cols) == 0) {
                    changes[idx] = true;
                    prevchanges++;
                }
            }
        }
        act(occupied, changes, rows, cols);
    }

    int seats = 0;
    for (int i = 0; i < rows * cols; i++) {
        if (occupied[i]) seats++;
    }
    return seats;
}

int main() {
    int rows, cols;
    vector<bool> chair = readFile("testinput.txt", rows, cols);

    cout << "Seats eventually are: " << endl << sim(chair, rows, cols) << endl;

    return 0;
}

