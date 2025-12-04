#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

void processBlock(vector<string> block, vector<array<int, 5>>& locks, vector<array<int, 5>>& keys) {
    array<int, 5> a;
    a.fill(0);

    bool isKey  = (block.back() == "#####");
    if (isKey) reverse(block.begin(), block.end());

    for (int i = 1; i < 7; i++) {
	for (int j = 0; j < 5; j++) {
	    if (block[i][j] == '#') {a[j]++;}
	} 
    }

    if (isKey) keys.push_back(a);
    else locks.push_back(a);
}

void readInput(string filename, vector<array<int, 5>>& locks, vector<array<int, 5>>& keys) {
    ifstream f(filename);
    string line;
    vector<string> block;

    while (getline(f, line)) {
        if (line.empty()) {
            processBlock(block, locks, keys);
            block.clear();
        } else {
            block.push_back(line);
        }
    }
    processBlock(block, locks, keys); // last block
}

int main() {
    vector<array<int, 5>> locks, keys;
    readInput("input.txt", locks, keys);

    int combinations = 0;
    for (auto l : locks) {
        for (auto k : keys) {
	    bool failFlag = false;
	    for (int i = 0; i < 5; i++) {
	        if (l[i] + k[i] > 5) {failFlag = true; break;}
	    }
	    if (failFlag) {continue;} else {
	        combinations++;
	    }
        }
    }
    cout << "Combinations: " << combinations << endl;
}
