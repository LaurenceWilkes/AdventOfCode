// read_file.h
#ifndef READ_FILE_H  // Include guard to prevent multiple inclusion
#define READ_FILE_H

#include <vector>   // To use std::vector
#include <string>   // To use std::string

using IntVector = std::vector<int>;
using CharVector = std::vector<char>;
using StringVector = std::vector<std::string>;

// Function declaration: this function reads a file and returns a vector of integers
std::tuple<IntVector, IntVector, CharVector, StringVector> readFile(const std::string& filename);

#endif  // READ_FILE_H
