// read_file.cpp
#include "read_file.h"  // Include the header file
#include <fstream>      // For file handling
#include <sstream>      // For string stream

// Function definition: read integers from a file and return them as a vector
std::vector<int> readFile(const std::string& filename) {
    std::ifstream file(filename);  // Open the file
    std::vector<int> numbers;      // Vector to store the numbers
    std::string line;              // String to store each line

    // Check if file is open
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file.");
    }

    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);  // Use stringstream to convert string to int
        int number;
        ss >> number;                // Extract the integer from the line
        numbers.push_back(number);   // Add the number to the vector
    }

    file.close();  // Close the file

    return numbers;  // Return the vector of numbers
}

