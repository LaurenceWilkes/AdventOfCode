#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>


using namespace std;

const vector<string> seaMonster = {
    "                  # ",
    "#    ##    ##    ###",
    " #  #  #  #  #  #   "
};

struct Tile {
    long id;                       // Unique tile ID
    vector<string> data;          // Full tile grid (for the interior)
    vector<string> edges;         // Edges stored as [top, right, bottom, left]
    int rotation_state = 0;       // Current rotation state (0, 90, 180, 270 degrees)
    bool is_mirrored = false;     // Tracks if the tile is horizontally mirrored

    // Constructor
    Tile(long id, const vector<string>& data)
        : id(id), data(data), edges(4) {
        update_edges();
    }

    // Update edges based on current data
    void update_edges() {
        edges[0] = data.front(); // Top
        edges[2] = data.back();  // Bottom

        string left, right;
        for (const auto& row : data) {
            left += row[0];
            right += row.back();
        }
        edges[3] = left;  // Left
        edges[1] = right; // Right
    }

    // Rotate edges 90 degrees clockwise
    void rotate_edges() {
	reverse(edges[1].begin(), edges[1].end());
	reverse(edges[3].begin(), edges[3].end());
        rotate(edges.rbegin(), edges.rbegin() + 1, edges.rend());
        if (!is_mirrored) {
	    ++rotation_state %= 4;
	} else {
	    rotation_state = (rotation_state + 3) % 4;
	}
    }

    // Mirror edges horizontally
    void mirror_edges() {
        swap(edges[1], edges[3]); // Swap right and left
        reverse(edges[0].begin(), edges[0].end()); // Reverse top
        reverse(edges[2].begin(), edges[2].end()); // Reverse bottom
        is_mirrored = !is_mirrored;
    }

    // Lazily transform the full data based on current edge state
    vector<string> get_transformed_data() {
        vector<string> transformed = data;

        // Apply rotation
        for (int i = 0; i < rotation_state; ++i) {
            transformed = rotate_data_clockwise(transformed);
        }

        // Apply mirroring if needed
        if (is_mirrored) {
            for (auto& row : transformed) {
                reverse(row.begin(), row.end());
            }
        }

        return transformed;
    }

    // Debug print for edges and data
    void print_tile() const {
        cout << "Tile ID: " << id << endl;
        cout << "Edges: Top: " << edges[0] << ", Right: " << edges[1]
             << ", Bottom: " << edges[2] << ", Left: " << edges[3] << endl;
    }

    void print_data(const vector<string>& grid) const {
        for (const auto& row : grid) {
            cout << row << endl;
        }
    }

private:
    // Helper function to rotate a grid 90 degrees clockwise
    vector<string> rotate_data_clockwise(const vector<string>& grid) {
        int n = grid.size();
        vector<string> rotated(n, string(n, ' '));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                rotated[j][n - i - 1] = grid[i][j];
            }
        }
        return rotated;
    }
};



// Function to parse the input
void parse_input(const string &filename, vector<long> &tile_ids, vector<vector<string>> &tile_data) {
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    long current_tile_id = -1;
    vector<string> current_tile;

    while (getline(input_file, line)) {
        if (line.empty()) {
            // End of current tile
            if (current_tile_id != -1) {
                tile_ids.push_back(current_tile_id);
                tile_data.push_back(current_tile);
                current_tile.clear();
            }
            current_tile_id = -1;
        } else if (line.find("Tile") == 0) {
            // Start of a new tile
            current_tile_id = stoi(line.substr(5, line.find(':') - 5));
        } else {
            // Tile data row
            current_tile.push_back(line);
        }
    }

    // Add the last tile if the file doesn't end with a blank line
    if (current_tile_id != -1) {
        tile_ids.push_back(current_tile_id);
        tile_data.push_back(current_tile);
    }

    input_file.close();
}

bool edgeMatch(string edge, const Tile& tile) {
    string redge = string(edge.rbegin(), edge.rend());
    for (string tileEdge : tile.edges) {
        if (redge == tileEdge || edge == tileEdge) {
            return true;
        }
    }
    return false;
} // edgeMatch

bool isCorner(int pos, vector<Tile>& tiles) {
    for (int j = 0; j < 4; j++) {
	bool changeFlag = true;
	string currentLeftEdge = (tiles[pos].edges)[3];
	string currentTopEdge = (tiles[pos].edges)[0];
	for (int i = 0; i < tiles.size(); i++) {
	    if (i != pos) {
		if (edgeMatch(currentLeftEdge, tiles[i])) {
		    changeFlag = false;
		    break;
		}
		if (edgeMatch(currentTopEdge, tiles[i])) {
		    changeFlag = false;
		    break;
		}
	    }
	}
	if (changeFlag) {
	    return true;
	}
	tiles[pos].rotate_edges();
    }
    return false;
} // isCorner

int findCorner(vector<Tile>& tiles) {
    for (int pos = 0; pos < tiles.size(); pos++) {
        if (isCorner(pos, tiles)) {
            return pos;
        }
    }
    return -1;
} // findCorner

vector<int> findCorners(vector<Tile>& tiles) {
    vector<int> corners;
    for (int pos = 0; pos < tiles.size(); pos++) {
        if (isCorner(pos, tiles)) {
	    corners.push_back(pos);
        }
    }
    return corners;
} // findCorner

// Part 2
//
bool matchTile(char dir, string edge, Tile& tile) {
    int directionEdge = 3;
    if (dir == 'r') {
	directionEdge = 3; // the left edge
    } else {
        directionEdge = 0; // the up edge
    }
    for (int rot = 0; rot < 4; rot++) {
//	cout << directionEdge << " - " << dir << " edge at this point is " << tile.edges[directionEdge]
//	     << " the matching edge is " << edge << endl;
        if (edge == tile.edges[directionEdge]) {
            return true;
        }
	tile.rotate_edges();
    }

    tile.mirror_edges();

    for (int rot = 0; rot < 4; rot++) {
//	cout << directionEdge << " - " << dir << " edge at this point is " << tile.edges[directionEdge]
//	     << " the matching edge is " << edge << endl;
        if (edge == tile.edges[directionEdge]) {
            return true;
        }
	tile.rotate_edges();
    }

    return false;

} // matchTile

vector<int> matchRight(int startPos, map<int, bool>& visited, vector<Tile>& tiles) {
    vector<int> outIndices = {startPos};
    int currentIndex = startPos;
    bool stopFlag = false;
    int directionEdge = 1; // the right edge

    while (!stopFlag) {
	stopFlag = true;
	string edge = (tiles[currentIndex].edges)[directionEdge];
	for (int i = 0; i < tiles.size(); i++) {
	    if (!visited[i]) {
		// cout << "checking right from " << tiles[currentIndex].id << " to " << tiles[i].id << endl; ////
		if (matchTile('r', edge, tiles[i])) {
		    visited[i] = true;
		    currentIndex = i;
		    stopFlag = false;
		    outIndices.push_back(i);
		    break;
		}
	    }
	}
	// cout << endl; ////
    }
    return outIndices;
} // matchInDir

int matchDown(int startPos, map<int, bool>& visited, vector<Tile>& tiles) {
    int directionEdge = 2; // the down edge
    string edge = (tiles[startPos].edges)[directionEdge];
    for (int i = 0; i < tiles.size(); i++) {
	if (!visited[i]) {
	    // cout << "checking down from " << tiles[startPos].id << " to " << tiles[i].id << endl; ////
	    if (matchTile('d', edge, tiles[i])) {
		visited[i] = true;
		return i;
	    }
	}
    }
    // cout << endl; ////
    return -1;
} // matchDown

void printPositionMatrix(const vector<vector<int>>& positionMatrix, const vector<Tile>& tiles) {
    cout << "Position Matrix:" << endl;
    for (const auto& row : positionMatrix) {
        for (int tileIdx : row) {
            if (tileIdx != -1) {
                cout << tiles[tileIdx].id << "\t"; // Print tile ID
            } else {
                cout << "X\t"; // Indicate missing tiles
            }
        }
        cout << endl;
    }
}

vector<vector<int>> populateMatrix(int start, vector<Tile>& tiles) {
    int completed = 0;
    vector<vector<int>> outputMatrix;
    int currentStart = start;
    map<int, bool> visited;
    visited[currentStart] = true;
    while (currentStart != -1) {
	// cout << "Current start is " << currentStart << " with id: " << tiles[currentStart].id << endl;
	vector<int> currentRow = matchRight(currentStart, visited, tiles);
	outputMatrix.push_back(currentRow);
	currentStart = matchDown(currentStart, visited, tiles);
    }
    printPositionMatrix(outputMatrix, tiles);
    return outputMatrix;
} // populateImage

vector<vector<char>> assembleImage(const vector<vector<int>>& positionMatrix, vector<Tile>& tiles) {
    vector<vector<char>> finalImage;

    int tileSize = tiles[0].data.size() - 2;
    int finalRowSize = tileSize * positionMatrix.size();
    finalImage.resize(finalRowSize);

    int currentInitial = 0;

    for (const auto& row : positionMatrix) {

        for (int tileIdx : row) {
            vector<string> transformedTile = tiles[tileIdx].get_transformed_data(); // Get data from tile

	    // Remove edges from data
            vector<string> innerTile;
            for (int i = 1; i < transformedTile.size() - 1; ++i) {
                innerTile.push_back(transformedTile[i].substr(1, tileSize));
            }

	    // Include data in final image
	    for (int i = 0; i < innerTile.size(); i++) {
		for (char c : innerTile[i]) {
		    finalImage[currentInitial + i].push_back(c);
		}
	    }
        }

	currentInitial += tileSize;
    }

    return finalImage;
}

bool isSeaMonster(const vector<vector<char>>& image, int row, int col) {
    for (int r = 0; r < seaMonster.size(); ++r) {
        for (int c = 0; c < seaMonster[r].size(); ++c) {
            if (seaMonster[r][c] == '#' && image[row + r][col + c] != '#') {
                return false;
            }
        }
    }
    return true;
}

void markSeaMonster(vector<vector<char>>& image, int row, int col) {
    for (int r = 0; r < seaMonster.size(); ++r) {
        for (int c = 0; c < seaMonster[r].size(); ++c) {
            if (seaMonster[r][c] == '#') {
                image[row + r][col + c] = 'O';
            }
        }
    }
}

vector<vector<char>> rotate(const vector<vector<char>>& image) {
    int n = image.size(), m = image[0].size();
    vector<vector<char>> rotated(m, vector<char>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            rotated[j][n - 1 - i] = image[i][j];
    return rotated;
}

vector<vector<char>> flip(const vector<vector<char>>& image) {
    vector<vector<char>> flipped = image;
    for (auto& row : flipped) {
        reverse(row.begin(), row.end());
    }
    return flipped;
}

int countHash(const vector<vector<char>>& image) {
    int countNo = 0;
    for (const auto& row : image) {
        countNo += count(row.begin(), row.end(), '#');
    }
    return countNo;
}

int findSeaMonsters(vector<vector<char>> image) {
    for (int flipCount = 0; flipCount < 2; ++flipCount) {
        for (int rotation = 0; rotation < 4; ++rotation) {
            int seaMonsterCount = 0;
            for (int row = 0; row <= image.size() - seaMonster.size(); ++row) {
                for (int col = 0; col <= image[0].size() - seaMonster[0].size(); ++col) {
                    if (isSeaMonster(image, row, col)) {
                        markSeaMonster(image, row, col);
                        ++seaMonsterCount;
                    }
                }
            }
            if (seaMonsterCount > 0) {
                // Return the roughness of the waters
                return countHash(image);
            }
            image = rotate(image);
        }
        image = flip(image);
    }
    return -1; // No sea monsters found
}

vector<vector<char>> readImage(const string& filename) {
    ifstream inputFile(filename);
    vector<vector<char>> image;
    string line;

    while (getline(inputFile, line)) {
        vector<char> row(line.begin(), line.end());
        image.push_back(row);
    }

    inputFile.close();
    return image;
}

// Function to display the assembled image
void displayImage(const vector<vector<char>>& image) {
    for (const auto& row : image) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
}

int main() {
    string filename = "input.txt";
    vector<long> tile_ids;
    vector<vector<string>> tile_data;

    parse_input(filename, tile_ids, tile_data);

    vector<Tile> tiles;

    for (int i = 0; i < tile_ids.size(); i++) {
        tiles.push_back(Tile(tile_ids[i], tile_data[i]));
    }

    // Part 1
    vector<int> corners = findCorners(tiles);

    long prod = 1;

    for (int corner : corners) {
        prod *= tiles[corner].id;
    }

    cout << "Product of corner ids is: " << prod << endl;

    // Part 2

    cout << endl;

    int initialPos = findCorner(tiles);

    cout << "Corner found" << endl;

    vector<vector<int>> tilePositions = populateMatrix(initialPos, tiles);

    cout << "Tile positions made" << endl;

    vector<vector<char>> image = assembleImage(tilePositions, tiles);

    cout << "Image assembled" << endl;
    // vector<vector<char>> image = readImage("testinput.txt");

    // displayImage(image);
    // cout << "done displaying" << endl;

    int roughness = findSeaMonsters(image);

    cout << "Water roughness: " << roughness << endl;


    return 0;
}
