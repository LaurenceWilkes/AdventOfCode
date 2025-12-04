// read_file.h
#ifndef READ_FILE_H  // Include guard to prevent multiple inclusion
#define READ_FILE_H

#include <vector>   // To use std::vector
#include <string>   // To use std::string

// Function declaration: this function reads a file and returns a vector of integers
std::vector<int> readFile(const std::string& filename);

#endif  // READ_FILE_H
