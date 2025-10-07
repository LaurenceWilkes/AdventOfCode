#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <set>
#include <map>

using namespace std;

vector<int> readInput(string filename) {
    ifstream f(filename);

    string line;
    vector<int> outVec;
    while (getline(f, line)) {
        outVec.push_back(stoi(line));
    }
    return outVec;
} // readInput

// void mix(int& sec, int val) {sec = sec ^ val;} // mix
// void prune(int& sec) {sec &= 16777215;} // prune
// 
// int step1(int& sec) {
//     mix(sec, sec << 6);
//     prune(sec);
//     mix(sec, sec >> 5);
//     prune(sec);
//     mix(sec, sec << 11);
//     prune(sec);
// }

void step(int& secret) {
    secret ^= secret << 6;
    secret &= 16777215;
    secret ^= secret >> 5;
    secret &= 16777215;
    secret ^= secret << 11;
    secret &= 16777215;
}

long long secretSum(vector<int> secrets) {
    int iterations = 2000;
    long long output = 0;
    for (int i = 0; i < secrets.size(); i++) {
	for (int j = 0; j < iterations; j++) {
	    step(secrets[i]);
	}
	output += secrets[i];
    }
    return output;
} // secretSum	

void cycleDiffs(int newDiff, array<int, 4>& diffs) {
    diffs[0] = diffs[1];
    diffs[1] = diffs[2];
    diffs[2] = diffs[3];
    diffs[3] = newDiff;
} // cycleDiffs

int produceCosts(vector<int> secrets) {
    map<array<int, 4>, int> diffVal;
    set<array<int, 4>> seen;
    int currentMax = 0;

    int iterations = 2000;
    for (int i = 0; i < secrets.size(); i++) {
	int prev = secrets[i] % 10;
	array<int, 4> diffs;
	seen.clear();
	for (int j = 1; j <= iterations; j++) {
	    step(secrets[i]);
	    int val = secrets[i] % 10;
	    cycleDiffs(val - prev, diffs);
	    if (j > 3) {
		if (seen.find(diffs) == seen.end()) {
		    seen.insert(diffs);
		    if (diffVal.count(diffs)) {
			diffVal[diffs] += val;
		    } else {
		        diffVal[diffs] = val;
		    }
		    if (diffVal[diffs] > currentMax) {currentMax = diffVal[diffs];}
		}
	    }
	    prev = val;
	}
    }
    return currentMax;
} // produceCosts

int main() {
    vector<int> secrets = readInput("input.txt");
    long long output = secretSum(secrets);
    cout << "Part 1: " << output << endl;
    int maxVal = produceCosts(secrets);
    cout << "Part 2: " << maxVal << endl;
    return 0;
} // main



