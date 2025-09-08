// read_file.cpp
#include "read_file.h"  // Include the header file
#include <fstream>      // For file handling
#include <regex>
#include <string>

using IntVector = std::vector<int>;
using CharVector = std::vector<char>;
using StringVector = std::vector<std::string>;


// Function definition: read integers from a file and return them as a vector
std::tuple<IntVector, IntVector, CharVector, StringVector> readFile(const std::string& filename) {
    std::ifstream file(filename);  // Open the file
    IntVector startranges;
    IntVector endranges;
    CharVector chars;
    StringVector passwords;

    std::string line;              // String to store each line

    // Check if file is open
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file.");
    }

    std::regex pattern(R"((\d+)-(\d+) (\w): (\w+))"); // regex pattern
    std::smatch match; // to temporarily store the match

    // Read the file line by line
    while (std::getline(file, line)) {
	std::regex_match(line, match, pattern);
	startranges.push_back(std::stoi(match[1]));
	endranges.push_back(std::stoi(match[2]));
	chars.push_back(match[3].str()[0]);
	passwords.push_back(match[4].str());
    }

    file.close();  // Close the file

    return std::make_tuple(startranges, endranges, chars, passwords);
}

