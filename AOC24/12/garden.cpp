#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>

using namespace std;

void readFile(string filename, vector<vector<char>>& data) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
        vector<char> currentLine;
	currentLine.clear();

	for (char c : line) {
	    currentLine.push_back(c);
	}
	data.push_back(currentLine);
    }
} // readFile

void countArea(vector<vector<char>>& data, map<char, int>& fenceArea) {
    int x = data[0].size();
    int y = data.size();
    for (int i = 0; i < x; i++) {
	for (int j = 0; j < y; j++) {
	    fenceArea[data[j][i]]++;
	}
    }
} // createMap

void countPerimeter(vector<vector<char>>& data, map<char, int>& fenceLength) {
    int x = data[0].size();
    int y = data.size();
    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
	    // left
	    if (i == 0) {
	        fenceLength[data[j][i]]++;
	    } else if (data[j][i-1] != data[j][i]) {
	        fenceLength[data[j][i]]++;
	        fenceLength[data[j][i-1]]++;
	    }
	    // up
	    if (j == 0) {
	        fenceLength[data[j][i]]++;
	    } else if (data[j-1][i] != data[j][i]) {
	        fenceLength[data[j][i]]++;
	        fenceLength[data[j-1][i]]++;
	    }
	}
    }
    for (int j = 0; j < y; j++) {
	fenceLength[data[j][x-1]]++;
    }
    for (int i = 0; i < x; i++) {
	fenceLength[data[y-1][i]]++;
    }
} // countPerimeter

int main() {
    vector<vector<char>> data;
    readFile("testinput2.txt", data);
    map<char, int> fenceArea;
    countArea(data, fenceArea);
    map<char, int> fenceLength;
    countPerimeter(data, fenceLength);

    int totalSum = 0;
    for (const auto &[key, value] : fenceArea) {
	cout << "region - " << key << ", has area: " << value << " and fence length: " << fenceLength[key] << endl;
        totalSum += value * fenceLength[key];
    }

    cout << "The cost of fencing is: " << totalSum << endl;

    return 0;
} // main
