#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

// Directions for moving up, down, left, right
const vector<Point> directions = {
    Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)
};

// Diagonal directions: top-left, top-right, bottom-left, bottom-right
const vector<Point> diagonalDirections = {
    Point(-1, -1), Point(-1, 1), Point(1, 1), Point(1, -1)
};

// Check if the point is within bounds
bool inBounds(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// DFS to find all plots in the same region and mark them
void floodFill(int x, int y, int rows, int cols, vector<vector<char>>& grid, vector<vector<int>>& region, int regionId) {
    stack<Point> stk;
    stk.push(Point(x, y));
    region[x][y] = regionId;

    while (!stk.empty()) {
        Point p = stk.top();
        stk.pop();

        // Explore neighbors
        for (const Point& dir : directions) {
            int nx = p.x + dir.x;
            int ny = p.y + dir.y;

            if (inBounds(nx, ny, rows, cols) && grid[nx][ny] == grid[p.x][p.y] && region[nx][ny] == -1) {
                region[nx][ny] = regionId;
                stk.push(Point(nx, ny));
            }
        }
    }
}

// Count the number of corners for a given plot (inside the shape)
int countCorners(int x, int y, int rows, int cols, vector<vector<int>>& region, int regionId) {
    int adjacentCount = 0;

    // Check all four directions and count how many are in the same region
    vector<bool> isAdjacent(4, false);
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i].x;
        int ny = y + directions[i].y;

        if (inBounds(nx, ny, rows, cols) && region[nx][ny] == regionId) {
            isAdjacent[i] = true;
            adjacentCount++;
        }
    }

    // Based on the number of adjacent tiles, calculate the number of corners
    if (adjacentCount == 0) {
        return 4;  // No adjacent tiles, 4 corners exposed
    } else if (adjacentCount == 1) {
        return 2;  // One adjacent tile, 2 corners exposed
    } else if (adjacentCount == 2) {
        // Check if the two adjacent tiles are opposite or next to each other
        if ((isAdjacent[0] && isAdjacent[1]) || (isAdjacent[2] && isAdjacent[3])) {
            return 0;  // Two opposite adjacent tiles, no corners exposed
        } else {
            return 1;  // Two adjacent tiles next to each other, 1 corner exposed
        }
    } else if (adjacentCount == 3) {
        return 0;  // Three adjacent tiles, no corners exposed
    }

    return 0;  // Default case (shouldn't reach here)
}

// Check for reflex corners: counting the corners for non-set tiles
int countReflexCorners(int x, int y, int rows, int cols, vector<vector<int>>& region, int regionId) {
    int cornerCount = 0;

    // Check the four main directions and diagonals
    vector<Point> adjacentTiles = {
        Point(x - 1, y), Point(x + 1, y), Point(x, y - 1), Point(x, y + 1)
    };
    vector<Point> diagonalTiles = {
        Point(x - 1, y - 1), Point(x - 1, y + 1), Point(x + 1, y + 1), Point(x + 1, y - 1)
    };

    // Check if the current tile is not in the region
    if (region[x][y] != regionId) {
        // Top-left corner
        if (inBounds(x - 1, y, rows, cols) && inBounds(x, y - 1, rows, cols) && inBounds(x - 1, y - 1, rows, cols)) {
            if (region[x - 1][y] == regionId && region[x][y - 1] == regionId && region[x - 1][y - 1] == regionId) {
                cornerCount++;
            }
        }

        // Top-right corner
        if (inBounds(x - 1, y, rows, cols) && inBounds(x, y + 1, rows, cols) && inBounds(x - 1, y + 1, rows, cols)) {
            if (region[x - 1][y] == regionId && region[x][y + 1] == regionId && region[x - 1][y + 1] == regionId) {
                cornerCount++;
            }
        }

        // Bottom-right corner
        if (inBounds(x + 1, y, rows, cols) && inBounds(x, y + 1, rows, cols) && inBounds(x + 1, y + 1, rows, cols)) {
            if (region[x + 1][y] == regionId && region[x][y + 1] == regionId && region[x + 1][y + 1] == regionId) {
                cornerCount++;
            }
        }

        // Bottom-left corner
        if (inBounds(x + 1, y, rows, cols) && inBounds(x, y - 1, rows, cols) && inBounds(x + 1, y - 1, rows, cols)) {
            if (region[x + 1][y] == regionId && region[x][y - 1] == regionId && region[x + 1][y - 1] == regionId) {
                cornerCount++;
            }
        }
    }

    return cornerCount;
}

int main() {
    ifstream inputFile("input.txt");
    string line;
    vector<vector<char>> grid;

    while (getline(inputFile, line)) {
        grid.push_back(vector<char>(line.begin(), line.end()));
    }

    int rows = grid.size();
    int cols = grid[0].size();

    // Region ID grid to store region identifiers
    vector<vector<int>> region(rows, vector<int>(cols, -1));
    int regionId = 0;

    // Step 1: Flood fill to find all regions and mark them
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (region[i][j] == -1) {
                floodFill(i, j, rows, cols, grid, region, regionId++);
            }
        }
    }

    // Step 2: Calculate the total price
    int totalPrice = 0;

    // Keep track of region area and corners
    for (int id = 0; id < regionId; id++) {
        int area = 0;
        int corners = 0;

        // Loop through the grid and calculate area and corners for each region
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (region[i][j] == id) {
                    area++;
                    corners += countCorners(i, j, rows, cols, region, id);
                } else {
                    corners += countReflexCorners(i, j, rows, cols, region, id);
                }
            }
        }

        // Step 3: Price for the region
        totalPrice += area * corners;
    }

    cout << "Total Price: " << totalPrice << endl;

    return 0;
}

