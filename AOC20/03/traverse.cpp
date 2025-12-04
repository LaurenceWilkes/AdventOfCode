#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::vector<bool>> readFileToBooleanArray(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<bool>> boolArray;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return boolArray;  // Return an empty array on error
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<bool> row;
        for (char c : line) {
            if (c == '#') {
                row.push_back(true);
            } else if (c == '.') {
                row.push_back(false);
            }
        }
        boolArray.push_back(row);
    }
    
    file.close();
    return boolArray;
}

int traverseArray(int x, int y, std::vector<std::vector<bool>> boolArray) {
    int height = boolArray.size();
    int width = boolArray[0].size();
    int i = 0;
    int j = 0;

    long count = 0;

    while (j < height) {
	if (boolArray[j][i]) {
	    count++; 
	}  
	j += y;
	i = (i+x) % width;
    }

    return count;
}

int main() {
    std::string filename = "input.txt"; 
    std::vector<std::vector<bool>> boolArray = readFileToBooleanArray(filename);

    long slope_1_1 = traverseArray(1, 1, boolArray);
    long slope_3_1 = traverseArray(3, 1, boolArray);
    long slope_5_1 = traverseArray(5, 1, boolArray);
    long slope_7_1 = traverseArray(7, 1, boolArray);
    long slope_1_2 = traverseArray(1, 2, boolArray);

    std::cout << "Traversing with (1, 1) gives: " << slope_1_1 << std::endl;
    std::cout << "Traversing with (3, 1) gives: " << slope_3_1 << std::endl;
    std::cout << "Traversing with (5, 1) gives: " << slope_5_1 << std::endl;
    std::cout << "Traversing with (7, 1) gives: " << slope_7_1 << std::endl;
    std::cout << "Traversing with (1, 2) gives: " << slope_1_2 << std::endl;

    std::cout << "Their product is: " << slope_1_1 * slope_3_1 * slope_5_1 * slope_7_1 * slope_1_2 << std::endl;

    return 0;
}

