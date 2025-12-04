#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//int readAndMultiply(string filename) {
//    ifstream file(filename);
//
//    string line;
//
//    regex mulPattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
//
//    int currentSum = 0;
//
//    while (getline(file, line)) {
//	smatch match;
//        while (regex_search(line, match, mulPattern)) {
//	    int firstInt = stoi(match[1]);
//	    int secondInt = stoi(match[2]);
//	    //cout << firstInt << " ";
//	    currentSum += firstInt * secondInt;
//
//	    line = match.suffix().str();
//        }
//    }
//    return currentSum;
//} // readAndMultiply

int readAndMultiply(string filename) {
    ifstream file(filename);

    string line;

    regex mulPattern(R"(mul\((\d{1,3}),(\d{1,3})\)|(do|don't)\(\))");

    int currentSum = 0;

    bool currentlyReading = true;

    while (getline(file, line)) {
	smatch match;
        while (regex_search(line, match, mulPattern)) {
	    if (currentlyReading && match[1].matched) {
		int firstInt = stoi(match[1]);
		int secondInt = stoi(match[2]);
		currentSum += firstInt * secondInt;
	    } else if (match[3].matched) {
	        if (match[3] == "don't") {
	            currentlyReading = false;
	        } else {
	            currentlyReading = true;
	        }
	    }
	    line = match.suffix().str();
        }
    }
    return currentSum;
} // readAndMultiply

int main() {
    int outInt = readAndMultiply("input.txt");

    cout << "The sum of the products is: " << outInt << endl;
    return 0;
} // main
