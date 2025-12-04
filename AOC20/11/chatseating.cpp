#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Directions for visibility
const vector<pair<int, int>> directions = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},          {0, 1},
    {1, -1}, {1, 0}, {1, 1}
};

// Function to count visible occupied seats
int countVisibleOccupied(const vector<string>& layout, int row, int col) {
    int count = 0;
    for (const auto& dir : directions) {
        int r = row, c = col;
        while (true) {
            r += dir.first;
            c += dir.second;
            if (r < 0 || r >= layout.size() || c < 0 || c >= layout[0].size()) {
                break; // Out of bounds
            }
            if (layout[r][c] == '#') {
                count++;
                break; // Found an occupied seat
            }
            if (layout[r][c] == 'L') {
                break; // Found an empty seat
            }
        }
    }
    return count;
}

// Function to simulate the seating process
int simulateSeating(vector<string>& layout) {
    vector<string> newLayout = layout;
    bool changed = true;

    while (changed) {
        changed = false;
        for (int row = 0; row < layout.size(); row++) {
            for (int col = 0; col < layout[0].size(); col++) {
                if (layout[row][col] == 'L' && countVisibleOccupied(layout, row, col) == 0) {
                    newLayout[row][col] = '#'; // Seat becomes occupied
                    changed = true;
                } else if (layout[row][col] == '#' && countVisibleOccupied(layout, row, col) >= 5) {
                    newLayout[row][col] = 'L'; // Seat becomes empty
                    changed = true;
                }
            }
        }
        layout = newLayout; // Update the layout for the next iteration
    }

    // Count occupied seats
    int occupiedCount = 0;
    for (const auto& row : layout) {
        for (char seat : row) {
            if (seat == '#') {
                occupiedCount++;
            }
        }
    }

    return occupiedCount;
}

int main() {
    // Read the seating layout from input.txt
    vector<string> layout;
    ifstream inputFile("testinput.txt");
    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 1; // Exit if the file cannot be opened
    }

    string line;
    while (getline(inputFile, line)) {
        layout.push_back(line);
    }
    inputFile.close();

    int occupiedSeats = simulateSeating(layout);
    cout << "Occupied seats: " << occupiedSeats << endl;

    return 0;
}

